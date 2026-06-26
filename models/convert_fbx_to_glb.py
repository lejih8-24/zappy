import pathlib

import bpy
from io_scene_fbx import import_fbx


ROOT = pathlib.Path(__file__).resolve().parent
OUT = ROOT / "glb"
COMBINED_OUTPUT = OUT / "penguin_player.glb"
ANIMATIONS = {
    "PINGUIN WALK.fbx": "walk",
    "PINGUIN BROADCAST.fbx": "broadcast",
    "PINGUIN DEAD.fbx": "death",
    "PINGUIN EJECT.fbx": "eject",
    "PINGUIN INCANTATION.fbx": "incantation",
    "PINGUIN LAYIN EGG.fbx": "lay_egg",
    "PINGUIN LEVEL UP.fbx": "level_up",
}


class ImportOperator:
    def report(self, level, message):
        print(f"FBX {level}: {message}")


def patch_fbx_light_import():
    def blen_read_light(fbx_tmpl, fbx_obj, settings):
        import math

        elem_name_utf8 = import_fbx.elem_name_ensure_class(fbx_obj, b"NodeAttribute")
        fbx_props = (
            import_fbx.elem_find_first(fbx_obj, b"Properties70"),
            import_fbx.elem_find_first(fbx_tmpl, b"Properties70", import_fbx.fbx_elem_nil),
        )

        light_type = {
            0: "POINT",
            1: "SUN",
            2: "SPOT",
        }.get(import_fbx.elem_props_get_enum(fbx_props, b"LightType", 0), "POINT")
        lamp = bpy.data.lights.new(name=elem_name_utf8, type=light_type)

        if light_type == "SPOT":
            spot_size = import_fbx.elem_props_get_number(fbx_props, b"OuterAngle", None)
            if spot_size is None:
                spot_size = import_fbx.elem_props_get_number(fbx_props, b"Cone angle", 45.0)
            lamp.spot_size = math.radians(spot_size)

            spot_blend = import_fbx.elem_props_get_number(fbx_props, b"InnerAngle", None)
            if spot_blend is None:
                spot_blend = import_fbx.elem_props_get_number(fbx_props, b"HotSpot", 45.0)
            lamp.spot_blend = 1.0 - (spot_blend / spot_size)

        lamp.color = import_fbx.elem_props_get_color_rgb(fbx_props, b"Color", (1.0, 1.0, 1.0))
        lamp.energy = import_fbx.elem_props_get_number(fbx_props, b"Intensity", 100.0) / 100.0
        lamp.exposure = import_fbx.elem_props_get_number(fbx_props, b"Exposure", 0.0)
        lamp.use_shadow = import_fbx.elem_props_get_bool(fbx_props, b"CastShadow", True)
        if hasattr(lamp, "cycles") and hasattr(lamp.cycles, "cast_shadow"):
            lamp.cycles.cast_shadow = lamp.use_shadow
        if settings.use_custom_props:
            import_fbx.blen_read_custom_properties(fbx_obj, lamp, settings)
        return lamp

    import_fbx.blen_read_light = blen_read_light


def clear_scene():
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.object.delete()
    for collection in (
        bpy.data.meshes,
        bpy.data.armatures,
        bpy.data.actions,
        bpy.data.materials,
        bpy.data.images,
        bpy.data.textures,
    ):
        for item in list(collection):
            collection.remove(item)


def export_glb(output: pathlib.Path):
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
        export_all_influences=True,
    )


def export_fbx(path: pathlib.Path):
    clear_scene()
    import_fbx.load(ImportOperator(), bpy.context, filepath=str(path))

    output = OUT / f"{path.stem}.glb"
    export_glb(output)

    actions = [
        f"{action.name}:{int(action.frame_range[0])}-{int(action.frame_range[1])}"
        for action in bpy.data.actions
    ]
    meshes = [
        f"{mesh.name}:vertices={len(mesh.vertices)} faces={len(mesh.polygons)}"
        for mesh in bpy.data.meshes
    ]
    print(f"EXPORTED {path.name} -> {output}")
    print(f"  actions: {actions}")
    print(f"  meshes: {meshes}")


def find_armature():
    for obj in bpy.context.scene.objects:
        if obj.type == "ARMATURE":
            return obj
    return None


def import_action(path: pathlib.Path, name: str):
    previous_objects = set(bpy.context.scene.objects)
    previous_actions = set(bpy.data.actions)

    import_fbx.load(ImportOperator(), bpy.context, filepath=str(path))

    new_objects = [obj for obj in bpy.context.scene.objects if obj not in previous_objects]
    new_actions = [action for action in bpy.data.actions if action not in previous_actions]
    armature = next((obj for obj in new_objects if obj.type == "ARMATURE"), None)
    action = None

    if armature and armature.animation_data:
        action = armature.animation_data.action
    if action is None and new_actions:
        action = new_actions[0]
    if action is None:
        print(f"SKIP {path.name}: no animation action found")
        return None

    action.name = name
    action.use_fake_user = True

    for obj in new_objects:
        bpy.data.objects.remove(obj, do_unlink=True)
    return action


def export_combined_player():
    clear_scene()
    base_path = ROOT / "PINGUIN WALK.fbx"
    import_fbx.load(ImportOperator(), bpy.context, filepath=str(base_path))

    base_armature = find_armature()
    if base_armature is None:
        print("SKIP combined player: no base armature found")
        return

    if base_armature.animation_data and base_armature.animation_data.action:
        base_armature.animation_data.action.name = "walk"
        base_armature.animation_data.action.use_fake_user = True

    for filename, name in ANIMATIONS.items():
        if filename == "PINGUIN WALK.fbx":
            continue;
        import_action(ROOT / filename, name)

    export_glb(COMBINED_OUTPUT)
    print(f"EXPORTED combined player -> {COMBINED_OUTPUT}")
    print("  actions:", [(action.name, tuple(int(v) for v in action.frame_range)) for action in bpy.data.actions])


def main():
    patch_fbx_light_import()
    OUT.mkdir(exist_ok=True)
    for path in sorted(ROOT.glob("*.fbx")):
        export_fbx(path)
    export_combined_player()


if __name__ == "__main__":
    main()
