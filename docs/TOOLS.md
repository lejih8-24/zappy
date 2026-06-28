# Tools

Asset pipeline scripts live under `tools/`. All scripts require **Blender 3.6+** (with the built-in FBX importer add-on enabled) and are run headlessly via `blender --background`.

---

## convert_fbx_to_glb.py

Retargets a set of Mixamo FBX animation files onto a custom character GLB and exports a single combined GLB with all animation tracks.

### How it works

1. Loads the base character GLB (`--player-source`).
2. For each FBX animation file, imports the Mixamo rig into a separate scene context, computes per-bone local rotation deltas, and applies them to matching bones on the character rig using per-bone strength weights.
3. Keyframes are baked at full source frame rate with linear interpolation.
4. All retargeted actions are exported into a single GLB as separate animation tracks.

The bone map and animation list are defined as constants in the script. Edit them to adapt to a different character rig.

### Usage

```sh
blender --background --python tools/convert_fbx_to_glb.py -- \
    --input-dir  models/ \
    --player-source models/player.glb \
    --output gui/packs/penguin/player.glb
```

### Arguments

| Argument | Required | Description |
|---|---|---|
| `--input-dir` | yes | Directory containing the `.fbx` source animation files |
| `--player-source` | yes | Path to the base character GLB to retarget onto |
| `--output` | yes | Output path for the combined GLB |

### Default animation list

The script expects the following FBX filenames in `--input-dir`. Edit the `ANIMATIONS` constant to match your filenames and the action names expected by the pack manifest.

| FBX file | Action name (in output GLB) |
|---|---|
| `PINGUIN BROADCAST.fbx` | `broadcast` |
| `PINGUIN DEAD.fbx` | `death` |
| `PINGUIN EJECT.fbx` | `eject` |
| `PINGUIN INCANTATION.fbx` | `incantation` |
| `PINGUIN LAYIN EGG.fbx` | `laying_egg` |
| `PINGUIN LEVEL UP.fbx` | `level_up` |
| `PINGUIN WALK.fbx` | `walk` |

### Bone map

The `BONE_MAP` constant controls which Mixamo bones drive which character bones and at what strength (0.0 = no effect, 1.0 = full source rotation):

```python
("target_bone_name", "mixamorig:SourceBone", strength)
```

Lower strengths are used for stylised characters where full humanoid motion is too aggressive (e.g. 0.12 for the spine on a penguin).

---

## combine_mixamo_fbx_to_glb.py

Converts Mixamo FBX animation files to GLB using the **FBX2glTF** binary and merges them into a single character GLB via Blender's NLA track system.

Use this when the character rig is already Mixamo-compatible and no retargeting is needed. For custom rigs, use `convert_fbx_to_glb.py` instead.

### How it works

1. Each FBX file is converted to a raw GLB with `FBX2glTF --binary`.
2. The first animation is imported into Blender as the base armature.
3. Remaining animations are imported and their actions are transferred onto the base armature.
4. All actions are pushed into NLA tracks and exported as a single combined GLB.

### Requirements

The **FBX2glTF** binary must be downloaded separately. Get it from the official repository release page and place it anywhere accessible, then pass its path via `--fbx2gltf`.

### Usage

```sh
blender --background --python tools/combine_mixamo_fbx_to_glb.py -- \
    --input-dir  models/ \
    --output gui/packs/penguin/player.glb \
    --fbx2gltf tools/FBX2glTF
```

### Arguments

| Argument | Required | Description |
|---|---|---|
| `--input-dir` | yes | Directory containing the `.fbx` source animation files |
| `--output` | yes | Output path for the combined GLB |
| `--fbx2gltf` | yes | Path to the FBX2glTF binary |
| `--tmp` | no | Directory for intermediate per-animation GLBs. Defaults to a system temp directory that is cleaned up automatically. |

### Default animation list

Same filenames as `convert_fbx_to_glb.py`. Edit the `ANIMATIONS` constant to match your pack.

---

## repack_jpg_glb_images_as_png.py

Rewrites the embedded images inside a GLB from JPEG to PNG. Some GLBs exported from Mixamo or other tools use JPEG textures, which raylib cannot always decode correctly at runtime. This script fixes those files in-place.

### Usage

```sh
python3 tools/repack_jpg_glb_images_as_png.py path/to/model.glb
```

No additional dependencies beyond the Python standard library.

---

## mock_server.py

A minimal Zappy server mock that streams pre-recorded protocol messages over TCP. Used for GUI development without a running game server.

### Usage

```sh
python3 tools/mock_server.py
```

Listens on `localhost:4242` by default. Connect the GUI with `./zappy_gui -p 4242`.

---

## Track indices and the manifest

The order of animation tracks in the exported GLB determines the index used in the pack `manifest.json` `animations` block. After generating a new `player.glb`, open it in Blender (or inspect with `gltf-transform`) to confirm the track order, then update the manifest accordingly.

See [docs/GUI/PACKS.md](GUI/PACKS.md) for the full pack and manifest reference.
