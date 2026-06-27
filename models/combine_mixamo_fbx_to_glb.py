import pathlib
import shutil
import subprocess

import bpy


ROOT = pathlib.Path(__file__).resolve().parent
PROJECT_ROOT = ROOT.parent
OUT = ROOT / "glb"
TMP = pathlib.Path("/tmp/zappy_penguin_fbx2gltf")
FBX2GLTF = PROJECT_ROOT / "tools" / "FBX2glTF"
COMBINED_OUTPUT = OUT / "penguin_mixamo_player.glb"

# Order must match gui/packs/penguin/manifest.json.
ANIMATIONS = [
    ("PINGUIN BROADCAST.fbx", "broadcast"),
    ("PINGUIN DEAD.fbx", "dead"),
    ("PINGUIN EJECT.fbx", "eject"),
    ("PINGUIN INCANTATION.fbx", "incantation"),
    ("PINGUIN LAYIN EGG.fbx", "laying_egg"),
    ("PINGUIN LEVEL UP.fbx", "level_up"),
    ("PINGUIN WALK.fbx", "walk"),
]


def clear_scene():
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete()
    for collection in (
        bpy.data.actions,
        bpy.data.meshes,
        bpy.data.armatures,
        bpy.data.materials,
        bpy.data.images,
        bpy.data.textures,
    ):
        for item in list(collection):
            collection.remove(item)


def find_armature(objects):
    for obj in objects:
        if obj.type == "ARMATURE":
            return obj
    raise RuntimeError("no armature found")


def convert_fbx_to_raw_glb(filename):
    if not FBX2GLTF.exists():
        raise RuntimeError(f"missing converter: {FBX2GLTF}")

    fbx_path = ROOT / filename
    output_prefix = TMP / fbx_path.stem.replace(" ", "_")
    output_path = output_prefix.with_suffix(".glb")

    subprocess.run(
        [
            str(FBX2GLTF),
            "--binary",
            "--skinning-weights",
            "4",
            "--normalize-weights",
            "1",
            "--input",
            str(fbx_path),
            "--output",
            str(output_prefix),
        ],
        check=True,
    )
    if not output_path.exists():
        raise RuntimeError(f"FBX2glTF did not create {output_path}")
    return output_path


def import_glb(path):
    previous_objects = set(bpy.context.scene.objects)
    previous_actions = set(bpy.data.actions)

    bpy.ops.import_scene.gltf(filepath=str(path))

    new_objects = [obj for obj in bpy.context.scene.objects if obj not in previous_objects]
    new_actions = [action for action in bpy.data.actions if action not in previous_actions]
    return find_armature(new_objects), new_objects, new_actions


def rename_imported_action(armature, imported_actions, action_name):
    if armature.animation_data and armature.animation_data.action:
        action = armature.animation_data.action
    elif len(imported_actions) == 1:
        action = imported_actions[0]
    else:
        raise RuntimeError(f"{armature.name}: no unique imported action")

    action.name = action_name
    action.use_fake_user = True
    return action


def remove_objects(objects):
    for obj in objects:
        bpy.data.objects.remove(obj, do_unlink=True)


def prepare_raw_glbs():
    if TMP.exists():
        shutil.rmtree(TMP)
    TMP.mkdir(parents=True)
    return [(convert_fbx_to_raw_glb(filename), action_name) for filename, action_name in ANIMATIONS]


def combine_actions(raw_glbs):
    base_path, base_action_name = raw_glbs[0]
    base_armature, _, base_actions = import_glb(base_path)
    base_action = rename_imported_action(base_armature, base_actions, base_action_name)
    base_armature.animation_data.action = base_action

    for path, action_name in raw_glbs[1:]:
        armature, imported_objects, imported_actions = import_glb(path)
        action = rename_imported_action(armature, imported_actions, action_name)
        remove_objects(imported_objects)
        print(f"IMPORTED {path.name} -> {action.name}")

    # Ensure deterministic export and keep all actions even if they are not active.
    for _, action_name in ANIMATIONS:
        action = bpy.data.actions.get(action_name)
        if action is None:
            raise RuntimeError(f"missing action: {action_name}")
        action.use_fake_user = True
    base_armature.animation_data.action = None

    for track in list(base_armature.animation_data.nla_tracks):
        base_armature.animation_data.nla_tracks.remove(track)
    for _, action_name in ANIMATIONS:
        action = bpy.data.actions[action_name]
        track = base_armature.animation_data.nla_tracks.new()
        track.name = action_name
        strip = track.strips.new(action_name, int(action.frame_range[0]), action)
        strip.name = action_name


def export_combined_player():
    OUT.mkdir(exist_ok=True)
    bpy.ops.export_scene.gltf(
        filepath=str(COMBINED_OUTPUT),
        export_format="GLB",
        export_animations=True,
        export_animation_mode="NLA_TRACKS",
        export_merge_animation="NONE",
        export_skins=True,
        export_yup=True,
        export_apply=False,
        export_optimize_animation_size=False,
        export_all_influences=False,
    )
    print(f"EXPORTED {COMBINED_OUTPUT}")


def main():
    clear_scene()
    raw_glbs = prepare_raw_glbs()
    combine_actions(raw_glbs)
    export_combined_player()


if __name__ == "__main__":
    main()
