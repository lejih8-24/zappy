# HUD du GUI

Le HUD est un panneau transparent à droite de l'écran.

- `H` : affiche/cache le HUD.
- `flèche gauche/droite` : change de page.
- `flèche haut/bas` : scroll la page courante.

## Pages

- `Overview` : map, temps, joueurs, œufs, équipes, incantations et gagnant.
- `Teams` : joueurs par équipe, niveaux et min/max.
- `Players` : joueurs groupés par équipe avec id, niveau, position, orientation et flags.
- `Resources` : ressources sur la map, dans les inventaires joueurs et total global.

## Couleurs

- `Overview` : labels colorés, valeurs en blanc, `Running` vert, gagnant jaune.
- `Teams` / `Players` : couleur cyclique par équipe.
- `Players` : flags `I` violet, `E` orange, `B` bleu ciel.
- `Resources` : mêmes couleurs que les ressources sur la map.

Exemple niveaux :

```text
TeamA: 3 players
  levels: L1:2 L3:1
```
