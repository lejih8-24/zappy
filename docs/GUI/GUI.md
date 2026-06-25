# Documentation GUI

## Lancement

- Le GUI se lance avec `-p port -h machine`.
- Il attend `WELCOME`, puis envoie `GRAPHIC`.
- Il lit les événements serveur à chaque frame.

## GameState

- `GameState` stocke l'état visible du jeu.
- Il contient la map, les tuiles, ressources, joueurs, œufs, équipes, temps et fin de partie.
- Les events réseau mettent à jour ce state.
- Le render lit seulement ce state.
- Le rendu est séparé entre `Window`, `GameCamera`, `Map` et `Hud`.

## Rendu 3D

- La map est affichée en grille de tuiles.
- Les ressources sont affichées par type avec une couleur et une position différente (fixe pour chaque ressource).
- Les joueurs sont affichés en cubes.
- Les œufs sont affichés en sphères.
- Le centre des tuiles reste libre pour éviter les superpositions.

## HUD

- Le HUD est un panneau à droite.
- `H` cache ou affiche le HUD.
- Flèches gauche/droite : changer de page HUD.
- Flèches haut/bas : scroll dans la page HUD.
- Il contient les pages `Overview`, `Teams`, `Players` et `Resources`.
- Détails : [HUD.md](HUD.md).

## Textes et scale

- Les textes UI changent de taille selon la hauteur de la fenêtre.
- La taille de base est de `900px` de haut.
- En dessous, la taille ne diminue pas.
- Au-dessus ou en fullscreen, les textes grossissent.
- Le HUD, ses textes et le crosshair utilisent aussi ce scale.

## Contrôles

- `WASD` : déplacer la position de la caméra.
- `déplacement souris` : changer la direction de la caméra.
- `R` : reset caméra.
- `F11` : fullscreen.
- `H` : afficher/cacher le HUD.
- `flèches gauche/droite` : changer de page HUD.
- `flèches haut/bas` : scroll dans la page HUD.
- `X` : fermer le GUI.
- `ESC` : lock caméra (permet d'avoir sa souris libre).
