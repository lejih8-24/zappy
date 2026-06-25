# GUI Pack System

Packs are visual themes for the Zappy GUI. Each pack lives in its own folder under `gui/packs/` and contains a manifest and optional 3D model assets.

## Folder Structure

```
gui/packs/
  <pack-name>/
    manifest.json       <- required
    player.glb          <- optional
    egg.glb             <- optional
    tile.glb            <- optional
    food.glb            <- optional
    linemate.glb        <- optional
    deraumere.glb       <- optional
    sibur.glb           <- optional
    mendiane.glb        <- optional
    phiras.glb          <- optional
    thystame.glb        <- optional
```

## manifest.json

Every pack must have a `manifest.json` at its root. This is what identifies a folder as a valid pack.

```json
{
  "name": "pack-name",
  "version": "1.0.0",
  "description": "Short description of the pack",
  "author": "Your Name"
}
```

| Field | Required | Description |
|-------|----------|-------------|
| `name` | yes | Must match the folder name exactly |
| `version` | yes | Semantic version string |
| `description` | yes | Short human-readable description |
| `author` | yes | Author or team name |

## Asset Files

All assets are GLB (binary glTF) files. Asset filenames are fixed - the pack system looks for specific names:

| File | Entity | Fallback |
|------|--------|----------|
| `player.glb` | Player character | Primitive cube |
| `egg.glb` | Egg | Primitive sphere |
| `tile.glb` | Map tile | Primitive cube |
| `food.glb` | Food resource | Primitive cube |
| `linemate.glb` | Linemate resource | Primitive cube |
| `deraumere.glb` | Deraumere resource | Primitive cube |
| `sibur.glb` | Sibur resource | Primitive cube |
| `mendiane.glb` | Mendiane resource | Primitive cube |
| `phiras.glb` | Phiras resource | Primitive cube |
| `thystame.glb` | Thystame resource | Primitive cube |

Any missing asset falls back to the `default` primitive renderer automatically. You do not need to provide all assets.

## Built-in Packs

### `default`

Renders everything with raylib primitives (cubes and spheres). No asset files needed - only `manifest.json`.

### `green_man`

Renders players with a 3D GLB character model. Other entities fall back to primitives until their assets are added.

## Using a Pack

Pass `--pack <name>` when launching the GUI:

```sh
./zappy_gui -p 4242 --pack green_man
./zappy_gui -p 4242 --pack default
```

The default pack is `green_man` if `--pack` is not specified.

## Creating a New Pack

1. Create a folder under `gui/packs/<your-pack-name>/`
2. Add a `manifest.json` with the fields above (name must match folder name)
3. Drop in any GLB assets you want to override using the fixed filenames listed above
4. Launch with `--pack <your-pack-name>`
