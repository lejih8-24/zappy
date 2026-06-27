# GUI Pack System

Packs are visual themes for the Zappy GUI. Each pack lives in its own folder under `gui/packs/` and contains a manifest and optional 3D model assets.

## Folder Structure

```
gui/packs/
  <pack-name>/
    manifest.json       <- required
    player.glb          <- optional, single GLB with all animation tracks
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

Each asset is resolved independently through two levels:

```
1. packs/<your-pack>/<asset>.glb   <- look here first
        | not found
2. Primitive (cube / sphere)       <- always available
```

If an asset file is missing from your pack, that specific entity falls back to a primitive shape. Everything else in your pack still uses your 3D models.

**Examples:**

| Scenario | Player | Egg |
|----------|--------|-----|
| No `--pack` flag | Primitive cube | Primitive sphere |
| `--pack penguin` | 3D penguin character | 3D egg model |
| `--pack spongebob` (has player.glb, no egg.glb) | Spongebob character | Primitive sphere |
| `--pack spongebob` (has player.glb + egg.glb) | Spongebob character | Spongebob egg |

## manifest.json

Every pack must have a `manifest.json` at its root. This is what identifies a folder as a valid pack - if the folder does not exist or has no manifest, the GUI exits with an error.

```json
{
  "name": "pack-name",
  "version": "1.0.0",
  "description": "Short description of the pack",
  "author": "Your Name",
  "animations": {
    "broadcast": 0,
    "dead": 1,
    "eject": 2,
    "incantation": 3,
    "laying_egg": 4,
    "level_up": 5,
    "walk": 6
  },
  "playerScale": 1.0,
  "playerRotation": { "x": 0, "y": 0, "z": 0 },
  "playerTranslation": { "x": 0, "y": 0, "z": 0 },
  "playerLabelHeight": 2.5,
  "playerLabelScale": 140.0,
  "eggScale": 1.0,
  "eggRotation": { "x": 0, "y": 0, "z": 0 },
  "eggTranslation": { "x": 0, "y": 0, "z": 0 },
  "tileScale": 1.0,
  "tileRotation": { "x": 0, "y": 0, "z": 0 },
  "tileTranslation": { "x": 0, "y": 0, "z": 0 },
  "resourceScale": 1.0,
  "resourceRotation": { "x": 0, "y": 0, "z": 0 },
  "resourceTranslation": { "x": 0, "y": 0, "z": 0 },
  "backgroundColor": { "r": 0, "g": 82, "b": 172 }
}
```

| Field | Required | Default | Range | Description |
|-------|----------|---------|-------|-------------|
| `name` | yes | - | - | Must match the folder name exactly |
| `version` | yes | - | - | Semantic version string |
| `description` | yes | - | - | Short human-readable description |
| `author` | yes | - | - | Author or team name |
| `animations` | no | `{}` | - | Maps animation state names to track indices in `player.glb`. Keys: `broadcast`, `dead`, `eject`, `incantation`, `laying_egg`, `level_up`, `walk`. Any omitted state falls back to `idle` (frozen at frame 0 of the walk track). |
| `playerRotation` | no | `{0,0,0}` | any angle | Euler angles (degrees) to correct the player model orientation. Any axis can be omitted. |
| `playerScale` | no | 1.0 | 0.001 - 100.0 | Uniform scale applied to player models. Use when the GLB was exported at a different unit scale. |
| `playerTranslation` | no | `{0,0,0}` | any value | World-space XYZ offset applied to the player position after scale and rotation. |
| `playerLabelHeight` | no | 2.5 | 0.0 - 20.0 | World-space height at which the player name/level label is drawn above the model. |
| `playerLabelScale` | no | 140.0 | 10.0 - 500.0 | Label font size factor: `fontSize = clamp(playerLabelScale / cameraDistance, 8, 22)`. Higher = readable from further away. |
| `eggScale` | no | 1.0 | 0.000001 - 100.0 | Uniform scale applied to `egg.glb`. Use when the GLB was exported at a different unit scale. |
| `eggRotation` | no | `{0,0,0}` | any angle | Euler angles (degrees) to correct the egg model orientation. Any axis can be omitted. |
| `eggTranslation` | no | `{0,0,0}` | any value | World-space XYZ offset applied to the egg position after scale and rotation. Useful when the model origin is not at its base. |
| `tileScale` | no | 1.0 | 0.000001 - 100.0 | Uniform scale applied to `tile.glb`. |
| `tileRotation` | no | `{0,0,0}` | any angle | Euler angles (degrees) to correct the tile model orientation. Any axis can be omitted. |
| `tileTranslation` | no | `{0,0,0}` | any value | World-space XYZ offset applied to each tile position after scale and rotation. |
| `resourceScale` | no | 1.0 | 0.000001 - 100.0 | Uniform scale applied to all resource models (`food.glb`, `linemate.glb`, etc.). |
| `resourceRotation` | no | `{0,0,0}` | any angle | Euler angles (degrees) applied to all resource models. Any axis can be omitted. |
| `resourceTranslation` | no | `{0,0,0}` | any value | World-space XYZ offset applied to all resource positions after scale and rotation. |
| `backgroundColor` | no | `{0,82,172}` | 0-255 per channel | RGB background/sky color. Any channel can be omitted (defaults to DARKBLUE). |

Values outside the listed range are silently clamped to the nearest bound. Rotation angles are unclamped (any positive or negative degree value is valid).

## Player Animation States

Player animations are embedded as tracks in a single `player.glb` file. The manifest `animations` block maps state names to track indices in that file.

| State name | Triggered by | Duration | Notes |
|---|---|---|---|
| `idle` | Default (no activity) | permanent | Frozen at frame 0 - player stands still |
| `walk` | `ppo` (position update) | permanent | Loops while moving |
| `dead` | `pdi` (player death) | permanent | Stays until game ends |
| `incantation` | `pic` (incantation start) | until `pie` | Held until incantation ends |
| `broadcast` | `pbc` (broadcast) | 2s | Reverts to idle |
| `laying_egg` | `pfk` (fork/egg laying) | 1.5s | Reverts to idle |
| `level_up` | `plv` (level up) | 2.5s | Reverts to idle |
| `eject` | `pex` (expulsion) | 1s | Reverts to idle |

Any state not listed in the manifest `animations` block falls back to `idle` (frozen at frame 0 of the walk track).

If no `animations` block is present at all, all states render as idle.

## Asset Files

All assets are GLB (binary glTF) files. Asset filenames are fixed:

| File | Entity | Fallback |
|------|--------|----------|
| `player.glb` | Player character (all animation tracks) | Primitive cube |
| `egg.glb` | Egg | Primitive sphere |
| `tile.glb` | Map tile | Primitive cube |
| `food.glb` | Food resource | Primitive cube |
| `linemate.glb` | Linemate resource | Primitive cube |
| `deraumere.glb` | Deraumere resource | Primitive cube |
| `sibur.glb` | Sibur resource | Primitive cube |
| `mendiane.glb` | Mendiane resource | Primitive cube |
| `phiras.glb` | Phiras resource | Primitive cube |
| `thystame.glb` | Thystame resource | Primitive cube |

## Built-in Packs

### `green_man`

A GLB pack with a player model (7 animation tracks) and an egg model.

### `penguin`

A full GLB pack with player (7 animation tracks), egg, tile, and all 7 resource models (food, linemate, deraumere, sibur, mendiane, thystame + crystal as an extra). Background is black.

## Using a Pack

Pass `--pack <name>` when launching the GUI:

```sh
./zappy_gui -p 4242                       # primitives only
./zappy_gui -p 4242 --pack green_man      # 3D green man model
./zappy_gui -p 4242 --pack penguin        # 3D penguin model
./zappy_gui -p 4242 --pack spongebob      # custom pack
```

If the pack directory does not exist, the GUI prints an error and exits.

## Preparing 3D Models

All models must be in GLB (binary glTF) format. If you have FBX source files, use the conversion script:

```sh
python3 tools/convert_fbx_to_glb.py <input.fbx> <output.glb>
```

The script lives at `tools/convert_fbx_to_glb.py`. The original FBX source files are not tracked in the repository - only the converted GLBs in `models/glb/` are committed.

To combine multiple single-animation GLBs into one multi-track GLB for use as `player.glb`, open them in Blender, merge the armature actions into one file, and re-export. The track order in the exported GLB determines the indices used in the manifest `animations` block.

## Model Limitations

raylib enforces a hard limit of **65535 vertices per mesh**. Models that exceed this will trigger a fallback to primitives automatically - the GUI will not crash, but the 3D model will not render.

If your model falls back to a primitive cube unexpectedly, open it in Blender, add a **Decimate** modifier to the mesh, and reduce the ratio until the vertex count drops below 65535, then re-export as GLB.

## Creating a New Pack

1. Create a folder under `gui/packs/<your-pack-name>/`
2. Add a `manifest.json` with the fields above (name must match folder name)
3. Drop in any GLB assets using the fixed filenames listed above
4. Add an `animations` block mapping state names to track indices in your `player.glb`
5. Launch with `--pack <your-pack-name>`

You only need to provide the assets you want. Anything missing falls back to primitives.
