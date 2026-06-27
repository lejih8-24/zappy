# GUI Animations

## Principe

- Les animations du joueur viennent de `player.glb` dans le pack actif.
- Le fichier doit contenir plusieurs tracks dans un seul GLB. (`player.glb`)
- Le `manifest.json` associe chaque état à un index d'animation.
- Si une animation manque, le GUI utilise `walk` comme fallback.
- `idle` n'a pas de track dédiée : il affiche la frame `0` du `walk`.

## États supportés

| État | Event serveur | Comportement |
|---|---|---|
| `idle` | aucun | joueur immobile |
| `walk` | `ppo` avec changement de case | joué pendant le déplacement interpolé |
| `dead` | `pdi` | joué une fois, puis le joueur est supprimé |
| `incantation` | `pic` | reste actif jusqu'à `pie` |
| `broadcast` | `pbc` | animation courte, puis retour idle/walk |
| `laying_egg` | `pfk` | animation courte, puis retour idle/walk |
| `level_up` | `plv` | animation courte, puis retour idle/walk |
| `eject` | `pex` | animation courte, puis retour idle/walk |

## Walk et déplacement

- `ppo` met à jour la position logique du joueur.
- Si la case change, le rendu interpole entre l'ancienne position visible et la nouvelle.
- La durée est basée sur le temps serveur : `max(7 / timeUnit, 0.20s)`.
- Si `timeUnit <= 0`, la durée fallback est `0.35s`.
- Une rotation seule ne déclenche pas `walk`.
- Sur les bords, la map est torique : le rendu choisit le chemin visuel le plus court.

## Priorités

- Les animations spéciales ont priorité sur `walk`.
- Exemple : un joueur en `incantation` peut changer de position logique, mais l'animation visible reste `incantation`.
- Quand une animation courte se termine, le joueur revient à `walk` s'il bouge encore, sinon `idle`.
- `dead` est final : après l'animation, le joueur sort du `GameState`.

## Manifest

Exemple pour un pack avec 7 animations :

```json
"animations": {
  "broadcast": 0,
  "dead": 1,
  "eject": 2,
  "incantation": 3,
  "laying_egg": 4,
  "level_up": 5,
  "walk": 6
}
```

Les indices doivent correspondre à l'ordre réel des animations dans `player.glb`.
