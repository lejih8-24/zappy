from .bt_core import Node, NodeStatus
from CommandModel import LookCommand, ForwardCommand, TurnLeftCommand, TurnRightCommand, SetCommand, BroadcastCommand, \
    IncantationCommand, TakeCommand, ForkCommand, ConnectNbrCommand
from pathfinding import find_path_to_closest


class IsHungry(Node):
    """Évalue la faim avec un système d'hystérésis pour éviter l'effet Yo-Yo."""

    def tick(self, ai) -> NodeStatus:
        current_food = ai.states.inventory.get("food", 0)

        if ai.states.ready_for_incantation:
            return NodeStatus.FAILURE

        if ai.states.level == 8:
            return NodeStatus.SUCCESS

        if ai.states.is_master:
            panic_threshold = 14
            comfort_threshold = 21
        elif ai.states.last_master_id is not None:
            panic_threshold = 18
            comfort_threshold = 25
        else:
            panic_threshold = 35 if ai.states.level >= 2 else 15
            comfort_threshold = panic_threshold + 10

        if ai.states.is_hunting_food:
            if current_food >= comfort_threshold:
                ai.states.is_hunting_food = False
                return NodeStatus.FAILURE
            return NodeStatus.SUCCESS
        else:
            if current_food < panic_threshold:
                if ai.states.is_master:
                    ai.logger.Warn("[SURVIVAL] Urgence faim ! J'abandonne mon poste.")
                    msg = ai.comms.format_message("ALL", ai.states.level, "Master", "GROUPING", "ABORT")
                    ai.queue_command(BroadcastCommand(ai.id, msg))
                    ai.states.is_master = False
                    ai.states.master_wait_cycle = None

                ai.states.is_hunting_food = True
                return NodeStatus.SUCCESS

        return NodeStatus.FAILURE


class CanElevate(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.states.is_master:
            return NodeStatus.SUCCESS
        if ai.states.can_elevate():
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE


class HasMasterCall(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.states.last_master_id is not None or ai.states.ready_for_incantation:
            return NodeStatus.SUCCESS
        return NodeStatus.FAILURE

class ShouldReproduce(Node):
    """Vérifie si le drone a accumulé assez de réserves pour pondre en toute sécurité."""
    def tick(self, ai) -> NodeStatus:
        if ai.states.ready_for_incantation or ai.states.is_master or ai.states.last_master_id is not None:
            return NodeStatus.FAILURE

        if ai.states.inventory.get("food", 0) > 40 and (ai.cycle_count - ai.states.last_fork_cycle > 600):
            return NodeStatus.SUCCESS

        return NodeStatus.FAILURE


class ActionFork(Node):
    """Gère la reproduction de manière optimisée en vérifiant les slots disponibles."""

    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if ai.states.fork_step == "INIT":
            ai.logger.Info("[REPRODUCTION] Vérification des places (Connect_nbr)...")

            ai.queue_command(ConnectNbrCommand())
            ai.states.fork_step = "EVALUATE"
            return NodeStatus.RUNNING

        elif ai.states.fork_step == "EVALUATE":
            slots = ai.states.available_slots

            if slots > 0:
                ai.logger.Good(
                    f"[REPRODUCTION] {slots} place(s) libre(s) ! J'économise 42 ticks et j'invoque un renfort.")
                ai.launch_new_drone()
                ai.states.last_fork_cycle = ai.cycle_count
                ai.states.fork_step = "INIT"
                return NodeStatus.SUCCESS
            else:
                ai.logger.Info("[REPRODUCTION] Équipe complète. Je lance l'incubation (Fork)...")

                ai.queue_command(ForkCommand())
                ai.states.fork_step = "WAIT_FORK"
                return NodeStatus.RUNNING

        elif ai.states.fork_step == "WAIT_FORK":
            ai.states.last_fork_cycle = ai.cycle_count
            ai.states.fork_step = "INIT"
            return NodeStatus.SUCCESS

        return NodeStatus.FAILURE

class ActionSearchFood(Node):
    def tick(self, ai) -> NodeStatus:
        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        if ai.cycle_count % 400 == 0:
            current_food = ai.states.inventory.get('food', 0)
            if ai.states.is_master:
                target = 25
            elif ai.states.last_master_id is not None:
                target = 30
            else:
                target = 45 if ai.states.level >= 2 else 25
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

        # LOGIQUE DE TIMEOUT CORRIGÉE
        if ai.states.master_wait_cycle is None:
            ai.states.master_wait_cycle = ai.cycle_count
        else:
            if ai.cycle_count - ai.states.master_wait_cycle > 4500:
                ai.logger.Warn("[MASTER] Trop long (manque de pierres/joueurs) ! J'annule le rituel.")
                msg = ai.comms.format_message("ALL", ai.states.level, "Master", "GROUPING", "ABORT")
                ai.queue_command(BroadcastCommand(ai.id, msg))
                ai.states.is_master = False
                ai.states.master_wait_cycle = None
                return NodeStatus.FAILURE

        if required_players > 1:
            if players_on_tile < required_players:
                if ai.cycle_count - ai.states.last_call_cycle > 40:
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
            ai.states.master_wait_cycle = None
            return NodeStatus.RUNNING

        return NodeStatus.RUNNING


class ActionJoinMaster(Node):
    def tick(self, ai) -> NodeStatus:
        if ai.states.ready_for_incantation:
            return NodeStatus.SUCCESS

        if len(ai.pending_commands) > 0:
            return NodeStatus.RUNNING

        k = ai.states.master_direction

        if k is None:
            if ai.states.arrived_at_master:
                return NodeStatus.SUCCESS

            if ai.states.join_wait_cycle is None:
                ai.states.join_wait_cycle = ai.cycle_count
            elif ai.cycle_count - ai.states.join_wait_cycle > 800:
                ai.logger.Warn("[BT] Le signal du Master est perdu. J'abandonne.")
                ai.states.clear_master_call()
                ai.states.arrived_at_master = False
                ai.states.join_wait_cycle = None
                return NodeStatus.FAILURE
            return NodeStatus.RUNNING

        ai.states.join_wait_cycle = None
        ai.states.master_direction = None

        if k == 0:
            ai.logger.Info("[BT] Arrivé sur la case du Master.")
            ai.states.arrived_at_master = True
            return NodeStatus.SUCCESS

        ai.states.arrived_at_master = False

        if k in [1, 2, 8]:
            ai.queue_command(ForwardCommand())
        elif k in [3, 4, 5]:
            ai.queue_command(TurnLeftCommand())
        elif k in [6, 7]:
            ai.queue_command(TurnRightCommand())

        return NodeStatus.RUNNING


class ActionContributeStones(Node):
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

        if ai.states.wait_start_cycle is None:
            ai.states.wait_start_cycle = ai.cycle_count

        if ai.cycle_count - ai.states.wait_start_cycle > 1500:
            ai.logger.Warn("[BT] Le Master n'a pas toutes les pierres. J'annule le suivi pour retourner farmer.")
            ai.states.clear_master_call()
            ai.states.arrived_at_master = False
            ai.states.wait_start_cycle = None
            return NodeStatus.FAILURE

        if ai.cycle_count % 300 == 0:
            ai.logger.Info("[BT] Contribution terminée. En attente de l'incantation...")

        return NodeStatus.RUNNING