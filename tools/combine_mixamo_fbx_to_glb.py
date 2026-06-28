#!/usr/bin/env python3
"""
Convert Mixamo FBX animation files to GLB and combine them into a single character GLB.

Requires FBX2glTF binary for the FBX -> raw GLB step, then Blender merges them.

Run inside Blender:
    blender --background --python tools/combine_mixamo_fbx_to_glb.py -- \\
        --input-dir path/to/fbx_files \\
        --output path/to/output.glb \\
        --fbx2gltf path/to/FBX2glTF \\
        [--tmp /tmp/fbx_work]
"""

import argparse
import pathlib
import shutil
import subprocess
import sys
import tempfile

import bpy


ANIMATIONS = [
    ("PINGUIN BROADCAST.fbx", "broadcast"),
    ("PINGUIN DEAD.fbx", "dead"),
    ("PINGUIN EJECT.fbx", "eject"),
    ("PINGUIN INCANTATION.fbx", "incantation"),
    ("PINGUIN LAYIN EGG.fbx", "laying_egg"),
    ("PINGUIN LEVEL UP.fbx", "level_up"),
    ("PINGUIN WALK.fbx", "walk"),
]


def parse_args():
    argv = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    parser = argparse.ArgumentParser(description="Combine Mixamo FBX animations into one GLB")
    parser.add_argument("--input-dir", required=True, type=pathlib.Path,
                        help="Directory containing the FBX animation files")
    parser.add_argument("--output", required=True, type=pathlib.Path,
                        help="Output combined GLB file path")
    parser.add_argument("--fbx2gltf", required=True, type=pathlib.Path,
                        help="Path to the FBX2glTF binary")
    parser.add_argument("--tmp", type=pathlib.Path, default=None,
                        help="Temp directory for intermediate GLBs (default: system temp)")
    return parser.parse_args(argv)


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


def convert_fbx_to_raw_glb(fbx2gltf, input_dir, filename, tmp_dir):
    fbx_path = input_dir / filename
    output_prefix = tmp_dir / fbx_path.stem.replace(" ", "_")
    output_path = output_prefix.with_suffix(".glb")

    subprocess.run(
        [
            str(fbx2gltf),
            "--binary",
            "--skinning-weights", "4",
            "--normalize-weights", "1",
            "--input", str(fbx_path),
            "--output", str(output_prefix),
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


def prepare_raw_glbs(fbx2gltf, input_dir, tmp_dir):
    return [
        (convert_fbx_to_raw_glb(fbx2gltf, input_dir, filename, tmp_dir), action_name)
        for filename, action_name in ANIMATIONS
    ]


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


def export_combined(output):
    output.parent.mkdir(parents=True, exist_ok=True)
    bpy.ops.export_scene.gltf(
        filepath=str(output),
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
    print(f"EXPORTED {output}")


def main():
    args = parse_args()

    if not args.fbx2gltf.exists():
        raise RuntimeError(f"missing converter: {args.fbx2gltf}")

    use_tmp = args.tmp is not None
    tmp_dir = args.tmp if use_tmp else pathlib.Path(tempfile.mkdtemp(prefix="fbx2gltf_"))

    try:
        tmp_dir.mkdir(parents=True, exist_ok=True)
        clear_scene()
        raw_glbs = prepare_raw_glbs(args.fbx2gltf, args.input_dir, tmp_dir)
        combine_actions(raw_glbs)
        export_combined(args.output)
    finally:
        if not use_tmp:
            shutil.rmtree(tmp_dir, ignore_errors=True)


if __name__ == "__main__":
    main()
