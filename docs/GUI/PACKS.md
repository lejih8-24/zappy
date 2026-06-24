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

## Fallback Chain

Each asset is resolved independently through three levels:

```
1. packs/<your-pack>/<asset>.glb   <- look here first
        | not found
2. packs/green_man/<asset>.glb     <- fall back to base GLB pack
        | not found
3. Primitive (cube / sphere)       <- always available
```

`green_man` is the base GLB pack. Any custom pack that is missing an asset will automatically inherit it from `green_man`. If `green_man` also does not have it, a primitive shape is used.

**Examples:**

| Scenario | Player | Egg |
|----------|--------|-----|
| No `--pack` flag | Primitive cube | Primitive sphere |
| `--pack green_man` (no egg.glb) | 3D green character | Primitive sphere |
| `--pack spongebob` (has player.glb, no egg.glb) | Spongebob character | green_man egg (or sphere if green_man has none) |
| `--pack spongebob` (has player.glb + egg.glb) | Spongebob character | Spongebob egg |

## manifest.json

Every pack must have a `manifest.json` at its root. This is what identifies a folder as a valid pack.

```json
{
  "name": "pack-name",
  "version": "1.0.0",
  "description": "Short description of the pack",
  "author": "Your Name",
  "animations": {
    "walk": 2
  }
}
```

| Field | Required | Description |
|-------|----------|-------------|
| `name` | yes | Must match the folder name exactly |
| `version` | yes | Semantic version string |
| `description` | yes | Short human-readable description |
| `author` | yes | Author or team name |
| `animations.walk` | no | Animation slot index for the walk cycle (default: 2) |

## Asset Files

All assets are GLB (binary glTF) files. Asset filenames are fixed:

| File | Entity | Fallback |
|------|--------|----------|
| `player.glb` | Player character | green_man player, then primitive cube |
| `egg.glb` | Egg | green_man egg, then primitive sphere |
| `tile.glb` | Map tile | Primitive cube |
| `food.glb` | Food resource | Primitive cube |
| `linemate.glb` | Linemate resource | Primitive cube |
| `deraumere.glb` | Deraumere resource | Primitive cube |
| `sibur.glb` | Sibur resource | Primitive cube |
| `mendiane.glb` | Mendiane resource | Primitive cube |
| `phiras.glb` | Phiras resource | Primitive cube |
| `thystame.glb` | Thystame resource | Primitive cube |

## Built-in Packs

### `default`

Renders everything with raylib primitives (cubes and spheres). No asset files needed - only `manifest.json`. This is what runs when no `--pack` flag is given.

### `green_man`

The base GLB pack. Ships with `player.glb`. Other assets fall back to primitives until added. All custom packs inherit missing assets from this pack.

## Using a Pack

Pass `--pack <name>` when launching the GUI:

```sh
./zappy_gui -p 4242                       # primitives only
./zappy_gui -p 4242 --pack green_man      # 3D character model
./zappy_gui -p 4242 --pack spongebob      # custom pack, inherits from green_man
```

## Creating a New Pack

1. Create a folder under `gui/packs/<your-pack-name>/`
2. Add a `manifest.json` with the fields above (name must match folder name)
3. Drop in any GLB assets using the fixed filenames listed above
4. Launch with `--pack <your-pack-name>`

You only need to provide the assets you want to override. Everything else is inherited from `green_man` or falls back to primitives automatically.
