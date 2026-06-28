import json
import pathlib
import struct
from io import BytesIO

from PIL import Image


PACK = pathlib.Path("gui/packs/penguin")
BACKUP = PACK / "_jpeg_backup"
JSON_CHUNK = 0x4E4F534A
BIN_CHUNK = 0x004E4942
GLB_MAGIC = 0x46546C67


def pad4(data: bytes, value: bytes) -> bytes:
    return data + value * ((4 - len(data) % 4) % 4)


def convert_image_to_png(blob: bytes) -> bytes:
    image = Image.open(BytesIO(blob))
    output = BytesIO()
    image.save(output, format="PNG")
    return output.getvalue()


def read_glb(path: pathlib.Path):
    data = path.read_bytes()
    magic, version, _length = struct.unpack_from("<III", data, 0)
    if magic != GLB_MAGIC:
        raise ValueError(f"{path} is not a GLB file")

    chunks = []
    offset = 12
    while offset + 8 <= len(data):
        chunk_length, chunk_type = struct.unpack_from("<II", data, offset)
        offset += 8
        chunks.append((chunk_type, data[offset:offset + chunk_length]))
        offset += chunk_length
    json_data = next(chunk for chunk_type, chunk in chunks if chunk_type == JSON_CHUNK)
    bin_data = next(chunk for chunk_type, chunk in chunks if chunk_type == BIN_CHUNK)
    return json.loads(json_data.decode("utf-8")), bin_data


def write_glb(path: pathlib.Path, gltf: dict, bin_data: bytes):
    json_data = pad4(json.dumps(gltf, separators=(",", ":")).encode("utf-8"), b" ")
    bin_data = pad4(bin_data, b"\x00")
    total_length = 12 + 8 + len(json_data) + 8 + len(bin_data)

    path.write_bytes(
        struct.pack("<III", GLB_MAGIC, 2, total_length)
        + struct.pack("<II", len(json_data), JSON_CHUNK)
        + json_data
        + struct.pack("<II", len(bin_data), BIN_CHUNK)
        + bin_data
    )


def repack(path: pathlib.Path) -> bool:
    gltf, old_bin = read_glb(path)
    images = gltf.get("images", [])
    if not any(image.get("mimeType") == "image/jpeg" for image in images):
        return False

    old_views = gltf.get("bufferViews", [])
    new_bin = bytearray()
    view_mapping = {}
    new_views = []

    for index, view in enumerate(old_views):
        start = view.get("byteOffset", 0)
        end = start + view["byteLength"]
        blob = old_bin[start:end]
        image = next((img for img in images if img.get("bufferView") == index), None)

        if image and image.get("mimeType") == "image/jpeg":
            blob = convert_image_to_png(blob)
            image["mimeType"] = "image/png"

        offset = len(new_bin)
        new_bin.extend(blob)
        while len(new_bin) % 4:
            new_bin.append(0)

        new_view = {key: value for key, value in view.items() if key not in ("byteOffset", "byteLength")}
        new_view["byteOffset"] = offset
        new_view["byteLength"] = len(blob)
        view_mapping[index] = len(new_views)
        new_views.append(new_view)

    for image in images:
        if "bufferView" in image:
            image["bufferView"] = view_mapping[image["bufferView"]]

    gltf["bufferViews"] = new_views
    if gltf.get("buffers"):
        gltf["buffers"][0]["byteLength"] = len(new_bin)

    if not (BACKUP / path.name).exists():
        (BACKUP / path.name).write_bytes(path.read_bytes())
    write_glb(path, gltf, bytes(new_bin))
    return True


def main():
    BACKUP.mkdir(exist_ok=True)
    for path in sorted(PACK.glob("*.glb")):
        if repack(path):
            print(f"REPACKED {path.name}: JPEG images converted to PNG")
        else:
            print(f"SKIP {path.name}: no JPEG images")


if __name__ == "__main__":
    main()

