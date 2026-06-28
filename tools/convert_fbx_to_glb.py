#!/usr/bin/env python3
"""
Retarget Mixamo animations from FBX files onto a custom character GLB and export a combined GLB.

Run inside Blender:
    blender --background --python tools/convert_fbx_to_glb.py -- \\
        --input-dir path/to/fbx_files \\
        --player-source path/to/base_character.glb \\
        --output path/to/output.glb

Blender passes everything after '--' to the script as sys.argv.
"""

import argparse
import pathlib
import sys

import bpy
from io_scene_fbx import import_fbx as fbx_module
from mathutils import Quaternion


ANIMATIONS = [
    ("PINGUIN BROADCAST.fbx", "broadcast"),
    ("PINGUIN DEAD.fbx", "death"),
    ("PINGUIN EJECT.fbx", "eject"),
    ("PINGUIN INCANTATION.fbx", "incantation"),
    ("PINGUIN LAYIN EGG.fbx", "laying_egg"),
    ("PINGUIN LEVEL UP.fbx", "level_up"),
    ("PINGUIN WALK.fbx", "walk"),
]

# Retarget Mixamo humanoid deltas onto the custom penguin rig.
# Strength controls how much of the source rotation is applied (1.0 = full).
BONE_MAP = [
    ("Armature_Colonne", "mixamorig:Spine", 0.12),
    ("Armature_tronc", "mixamorig:Spine2", 0.16),
    ("Armature_tete", "mixamorig:Head", 0.18),
    ("Armature_clavi_D", "mixamorig:RightShoulder", 0.25),
    ("Armature_aile_D", "mixamorig:RightArm", 0.38),
    ("Armature_ailebout_D", "mixamorig:RightForeArm", 0.32),
    ("Armature_clavi_G", "mixamorig:LeftShoulder", 0.25),
    ("Armature_aile_G", "mixamorig:LeftArm", 0.38),
    ("Armature_ailebout_G", "mixamorig:LeftForeArm", 0.32),
    ("Armature_femur_D", "mixamorig:RightUpLeg", 0.22),
    ("Armature_cheville_D", "mixamorig:RightLeg", 0.30),
    ("Armature_patte_D", "mixamorig:RightFoot", 0.18),
    ("Armature_femur_G", "mixamorig:LeftUpLeg", 0.22),
    ("Armature_cheville_G", "mixamorig:LeftLeg", 0.30),
    ("Armature_patte_G", "mixamorig:LeftFoot", 0.18),
]

# Blender exports Z-up scenes to glTF Y-up. This offset keeps the retargeted
# idle/walk pose visually grounded on the tile surface after export.
PLAYER_VERTICAL_OFFSET = 1.2


def parse_args():
    argv = sys.argv[sys.argv.index("--") + 1:] if "--" in sys.argv else []
    parser = argparse.ArgumentParser(description="Retarget FBX animations onto a GLB character")
    parser.add_argument("--input-dir", required=True, type=pathlib.Path,
                        help="Directory containing the FBX animation files")
    parser.add_argument("--player-source", required=True, type=pathlib.Path,
                        help="Base character GLB to retarget onto")
    parser.add_argument("--output", required=True, type=pathlib.Path,
                        help="Output combined GLB file path")
    return parser.parse_args(argv)


class ImportOperator:
    def report(self, level, message):
        print(f"FBX {level}: {message}")


def patch_fbx_light_import():
    def blen_read_light(fbx_tmpl, fbx_obj, settings):
        elem_name_utf8 = fbx_module.elem_name_ensure_class(fbx_obj, b"NodeAttribute")
        lamp = bpy.data.lights.new(name=elem_name_utf8, type="POINT")
        return lamp

    fbx_module.blen_read_light = blen_read_light


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


def find_armature(objects=None):
    candidates = objects if objects is not None else bpy.context.scene.objects
    for obj in candidates:
        if obj.type == "ARMATURE":
            return obj
    raise RuntimeError("no armature found")


def uses_armature(mesh, armature):
    if mesh.parent == armature:
        return True
    return any(m.type == "ARMATURE" and m.object == armature for m in mesh.modifiers)


def remove_non_player_objects(armature):
    for obj in list(bpy.context.scene.objects):
        if obj == armature:
            continue
        if obj.type != "MESH" or not uses_armature(obj, armature):
            bpy.data.objects.remove(obj, do_unlink=True)


def import_base_player(player_source):
    bpy.ops.import_scene.gltf(filepath=str(player_source))
    armature = find_armature()
    armature.name = "PlayerArmature"
    armature.data.name = "PlayerRig"
    armature.location.z += PLAYER_VERTICAL_OFFSET
    remove_non_player_objects(armature)
    for pose_bone in armature.pose.bones:
        pose_bone.rotation_mode = "QUATERNION"
    return armature


def reset_pose(armature):
    for pose_bone in armature.pose.bones:
        pose_bone.location = (0.0, 0.0, 0.0)
        pose_bone.rotation_mode = "QUATERNION"
        pose_bone.rotation_quaternion = (1.0, 0.0, 0.0, 0.0)
        pose_bone.scale = (1.0, 1.0, 1.0)


def import_source_animation(path):
    previous_objects = set(bpy.context.scene.objects)
    previous_actions = set(bpy.data.actions)
    fbx_module.load(ImportOperator(), bpy.context, filepath=str(path))
    new_objects = [obj for obj in bpy.context.scene.objects if obj not in previous_objects]
    new_actions = [action for action in bpy.data.actions if action not in previous_actions]
    return find_armature(new_objects), new_objects, new_actions


def source_local_delta(source_armature, source_bone_name):
    source_bone = source_armature.data.bones[source_bone_name]
    source_pose_bone = source_armature.pose.bones[source_bone_name]
    if source_bone.parent:
        source_rest = source_bone.parent.matrix_local.inverted() @ source_bone.matrix_local
        source_pose = source_pose_bone.parent.matrix.inverted() @ source_pose_bone.matrix
    else:
        source_rest = source_bone.matrix_local.copy()
        source_pose = source_pose_bone.matrix.copy()
    return source_pose.to_quaternion() @ source_rest.to_quaternion().inverted()


def apply_delta_to_target(target_armature, target_bone_name, delta, strength):
    if delta.w < 0:
        delta.negate()
    pose_bone = target_armature.pose.bones[target_bone_name]
    pose_bone.rotation_quaternion = Quaternion((1.0, 0.0, 0.0, 0.0)).slerp(delta, strength)


def insert_target_keyframes(target_armature, frame):
    for target_bone_name, _, _ in BONE_MAP:
        target_armature.pose.bones[target_bone_name].keyframe_insert(
            data_path="rotation_quaternion",
            frame=frame,
        )


def set_linear_interpolation(action):
    curves = getattr(action, "fcurves", None)
    if curves is None:
        curves = []
        for layer in getattr(action, "layers", []):
            for strip in getattr(layer, "strips", []):
                for bag in getattr(strip, "channelbags", []):
                    curves.extend(getattr(bag, "fcurves", []))
    for curve in curves:
        for point in curve.keyframe_points:
            point.interpolation = "LINEAR"


def remove_imported_source(new_objects, new_actions):
    for obj in new_objects:
        bpy.data.objects.remove(obj, do_unlink=True)
    for action in new_actions:
        bpy.data.actions.remove(action, do_unlink=True)


def retarget_animation(target_armature, input_dir, fbx_name, action_name):
    source_armature, source_objects, source_actions = import_source_animation(input_dir / fbx_name)
    source_action = source_armature.animation_data.action if source_armature.animation_data else None
    if source_action is None:
        raise RuntimeError(f"{fbx_name}: no source action")

    start, end = (int(v) for v in source_action.frame_range)
    action = bpy.data.actions.new(action_name)
    action.use_fake_user = True
    target_armature.animation_data_create()
    target_armature.animation_data.action = action

    for frame in range(start, end + 1):
        bpy.context.scene.frame_set(frame)
        reset_pose(target_armature)
        bpy.context.view_layer.update()
        for target_bone_name, source_bone_name, strength in BONE_MAP:
            delta = source_local_delta(source_armature, source_bone_name)
            apply_delta_to_target(target_armature, target_bone_name, delta, strength)
        insert_target_keyframes(target_armature, frame)

    action.frame_start = start
    action.frame_end = end
    set_linear_interpolation(action)
    remove_imported_source(source_objects, source_actions)
    print(f"RETARGETED {fbx_name} -> {action_name} frames={start}-{end}")


def export_combined(target_armature, output):
    output.parent.mkdir(parents=True, exist_ok=True)
    target_armature.animation_data.action = bpy.data.actions.get("walk")
    bpy.ops.export_scene.gltf(
        filepath=str(output),
        export_format="GLB",
        export_animations=True,
        export_animation_mode="ACTIONS",
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
    patch_fbx_light_import()
    clear_scene()
    target_armature = import_base_player(args.player_source)

    for action in list(bpy.data.actions):
        bpy.data.actions.remove(action)
    reset_pose(target_armature)

    for fbx_name, action_name in ANIMATIONS:
        retarget_animation(target_armature, args.input_dir, fbx_name, action_name)

    export_combined(target_armature, args.output)


if __name__ == "__main__":
    main()
