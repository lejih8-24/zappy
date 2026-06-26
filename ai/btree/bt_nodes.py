from .bt_core import Node, NodeStatus
from CommandModel import LookCommand, ForwardCommand, TurnLeftCommand, TurnRightCommand, SetCommand, BroadcastCommand, \
    IncantationCommand, TakeCommand, ForkCommand
from pathfinding import find_path_to_closest


class IsHungry(Node):
    """Évalue la faim du drone avec des seuils dynamiques selon son rôle."""

    def tick(self, ai) -> NodeStatus:
        current_food = ai.states.inventory.get("food", 0)

        if getattr(ai.states, 'ready_for_incantation', False):
            return NodeStatus.FAILURE

        if getattr(ai.states, 'is_master', False):
            safety_threshold = 18

        elif getattr(ai.states, 'last_master_id', None) is not None:
            safety_threshold = 22

        else:
            safety_threshold = 35 if ai.states.level >= 2 else 15

        if current_food < safety_threshold:
            return NodeStatus.SUCCESS

        return NodeStatus.FAILURE

class CanElevate(Node):
    """Vérifie si le drone possède toutes les ressources pour passer au niveau supérieur."""
    def tick(self, ai) -> NodeStatus:
        if ai.states.can_elevate():
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class HasMasterCall(Node):
    """Vérifie si le drone est engagé envers un Master."""
    def tick(self, ai) -> NodeStatus:
        if getattr(ai.states, 'last_master_id', None) is not None or getattr(ai.states, 'ready_for_incantation', False):
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class ActionSearchFood(Node):
    """Cherche et ramasse de la nourriture en priorité."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if ai.cycle_count % 400 == 0:
            current_food = ai.states.inventory.get('food', 0)

            if getattr(ai.states, 'is_master', False):
                target = 18
            elif getattr(ai.states, 'last_master_id', None) is not None:
                target = 22
            else:
                target = 35 if ai.states.level >= 2 else 15

            ai.logger.Info(f"[SURVIVAL] Recherche de nourriture en cours ({current_food}/{target})...")

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.states.vision_grid, "food")

        if path is not None:
            ai.navigator.move_instructions(path, "food")
        else:
            ai.navigator.move_instructions(None, None)

        return NodeStatus.RUNNING


class ActionFarmStones(Node):
    """Cherche et ramasse la pierre manquante pour l'élévation."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        target_mat = ai.states.get_needed_material()

        if not target_mat:
            return NodeStatus.SUCCESS

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        path = find_path_to_closest(ai.states.vision_grid, target_mat)

        if path is not None:
            ai.navigator.move_instructions(path, target_mat)
        else:
            ai.navigator.move_instructions(None, None)

        return NodeStatus.RUNNING


class ActionGroupAndIncant(Node):
    """Le chef d'orchestre : Regroupe l'équipe, prépare la case et lance l'incantation."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        ai.states.is_master = True

        if not ai.states.vision_grid:
            ai.queue_command(LookCommand())
            return NodeStatus.RUNNING

        floor = ai.states.vision_grid[0]
        players_on_tile = ai.states.count_player_case()
        rules = ai.states.elevation_rules[ai.states.level]
        required_players = rules["players"]

        if not hasattr(ai.states, 'master_wait_cycle'):
            ai.states.master_wait_cycle = ai.cycle_count

        if ai.cycle_count - ai.states.master_wait_cycle > 4500:
            ai.logger.Warn("[MASTER] Trop long (manque de pierres/joueurs) ! J'annule le rituel.")
            msg = ai.comms.format_message("ALL", ai.states.level, "Master", "ABORT", "")
            ai.queue_command(BroadcastCommand(ai.id, msg))
            ai.states.is_master = False
            del ai.states.master_wait_cycle
            return NodeStatus.FAILURE

        if required_players > 1:
            if players_on_tile < required_players:
                if ai.cycle_count - getattr(ai.states, 'last_call_cycle', 0) > 250:
                    msg = ai.comms.format_message("ALL", ai.states.level, "Master", "GROUPING", "INCANTATION_CALL")
                    ai.queue_command(BroadcastCommand(ai.id, msg))
                    ai.states.last_call_cycle = ai.cycle_count
                ai.states.vision_grid = None
                return NodeStatus.RUNNING

        for item, qty in floor.items():
            if item == "player" or item == "food": continue
            required_qty = rules["stones"].get(item, 0)

            if qty > required_qty:
                ai.queue_command(TakeCommand(item))
                ai.states.vision_grid = None
                ai.states.master_wait_cycle = ai.cycle_count
                return NodeStatus.RUNNING

        for stone, required_qty in rules["stones"].items():
            current_on_floor = floor.get(stone, 0)
            if current_on_floor < required_qty:
                if ai.states.inventory.get(stone, 0) > 0:
                    ai.queue_command(SetCommand(stone))
                    ai.states.inventory[stone] -= 1
                    ai.states.vision_grid = None
                    ai.states.master_wait_cycle = ai.cycle_count
                    return NodeStatus.RUNNING

        if ai.states.is_floor_perfect():
            if required_players > 1:
                msg = ai.comms.format_message("ALL", ai.states.level, "Master", "INCANTATION", "INCANTATION_STARTING")
                ai.queue_command(BroadcastCommand(ai.id, msg))

            ai.logger.Good(
                f"[MASTER] Case parfaite ! Lancement du rituel (Niveau {ai.states.level} -> {ai.states.level + 1}).")
            ai.queue_command(IncantationCommand())
            ai.states.ready_for_incantation = True

            if hasattr(ai.states, 'master_wait_cycle'):
                del ai.states.master_wait_cycle
            return NodeStatus.RUNNING

        return NodeStatus.RUNNING


class ActionJoinMaster(Node):
    """Fait avancer le drone vers la source du signal du Master avec mémoire."""

    def tick(self, ai) -> NodeStatus:
        if getattr(ai.states, 'ready_for_incantation', False):
            return NodeStatus.SUCCESS

        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        k = ai.states.master_direction

        if k is None:
            if getattr(ai.states, 'arrived_at_master', False):
                return NodeStatus.SUCCESS

            if not hasattr(ai.states, 'join_wait_cycle'):
                ai.states.join_wait_cycle = ai.cycle_count
            elif ai.cycle_count - ai.states.join_wait_cycle > 800:
                ai.logger.Warn("[BT] Le signal du Master est perdu. J'abandonne la recherche.")
                ai.states.clear_master_call()
                ai.states.arrived_at_master = False
                del ai.states.join_wait_cycle
                return NodeStatus.FAILURE

            return NodeStatus.RUNNING

        if hasattr(ai.states, 'join_wait_cycle'):
            del ai.states.join_wait_cycle

        ai.states.master_direction = None

        if k == 0:
            ai.logger.Info("[BT] Arrivé sur la case du Master.")
            ai.states.arrived_at_master = True
            return NodeStatus.SUCCESS

        ai.states.arrived_at_master = False

        if k in [1, 2, 8]:
            ai.queue_command(ForwardCommand())
        elif k in [3, 4]:
            ai.queue_command(TurnLeftCommand())
        elif k in [5, 6, 7]:
            ai.queue_command(TurnRightCommand())

        return NodeStatus.RUNNING


class ActionContributeStones(Node):
    """Vide l'inventaire des pierres sur la case du Master."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        dropped_something = False
        for item, qty in list(ai.states.inventory.items()):
            if item != "food" and qty > 0:
                ai.queue_command(SetCommand(item))
                ai.states.inventory[item] -= 1
                dropped_something = True
                break

        if dropped_something:
            ai.states.vision_grid = None
            ai.states.wait_start_cycle = ai.cycle_count
            return NodeStatus.RUNNING

        if not hasattr(ai.states, 'wait_start_cycle'):
            ai.states.wait_start_cycle = ai.cycle_count

        if ai.cycle_count - ai.states.wait_start_cycle > 1500:
            ai.logger.Warn("[BT] Le Master n'a pas toutes les pierres. J'annule le suivi pour retourner farmer.")
            ai.states.clear_master_call()
            del ai.states.wait_start_cycle
            return NodeStatus.FAILURE

        if ai.cycle_count % 300 == 0:
            ai.logger.Info("[BT] Contribution terminée. En attente de l'incantation...")

        return NodeStatus.RUNNING


class ShouldReproduce(Node):
    """Vérifie si le drone a accumulé assez de réserves pour pondre en toute sécurité."""

    def tick(self, ai) -> NodeStatus:
        if ai.states.inventory.get("food", 0) > 40:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class ActionFork(Node):
    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        ai.logger.Info("Réserves optimales atteintes ! Début de la ponte...")
        ai.queue_command(ForkCommand())

        return NodeStatus.RUNNING