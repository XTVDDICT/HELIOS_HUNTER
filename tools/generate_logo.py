"""Build compact PNG assets for the HELIOS_HUNTER screen."""

from __future__ import annotations

import io
import sys
from pathlib import Path

from PIL import Image, ImageDraw


ASSETS = (
    ("HELIOS_MAIN", "heliospool-main.png", (136, 98), False),
    ("CHTA_LOGO", "chta.png", (72, 72), False),
    ("WJK_LOGO", "wjk.png", (72, 72), True),
    ("DGB_LOGO", "dgb.png", (72, 72), False),
    ("BCH_LOGO", "bch.png", (72, 72), False),
    ("BTC_LOGO", "btc.png", (72, 72), False),
    ("FIX_LOGO", "fix.png", (72, 72), False),
    ("XEC_LOGO", "xec.png", (72, 72), False),
)


def screen_png(path: Path, size: tuple[int, int], circle: bool) -> bytes:
    source = Image.open(path).convert("RGBA")
    if path.name == "heliospool-main.png":
        pixels = []
        for red, green, blue, alpha in source.getdata():
            if red < 32 and green < 32 and blue < 38:
                pixels.append((0, 0, 0, 0))
            else:
                pixels.append((red, green, blue, alpha))
        source.putdata(pixels)
    source.thumbnail(size, Image.Resampling.LANCZOS)
    layer = Image.new("RGBA", size, (0, 0, 0, 0))
    x = (size[0] - source.width) // 2
    y = (size[1] - source.height) // 2
    layer.alpha_composite(source, (x, y))

    if circle:
        mask = Image.new("L", size, 0)
        ImageDraw.Draw(mask).ellipse((1, 1, size[0] - 2, size[1] - 2), fill=255)
        layer.putalpha(Image.composite(layer.getchannel("A"), mask, mask))

    matte = Image.new("RGB", size, "black")
    matte.paste(layer, mask=layer.getchannel("A"))
    buffer = io.BytesIO()
    matte.save(buffer, "PNG", optimize=True, compress_level=9)
    return buffer.getvalue()


def array_lines(data: bytes) -> str:
    rows = []
    for offset in range(0, len(data), 16):
        values = ", ".join(f"0x{value:02X}" for value in data[offset : offset + 16])
        rows.append(f"    {values},")
    return "\n".join(rows)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: generate_logo.py ASSET_DIRECTORY OUTPUT_HEADER")

    asset_dir = Path(sys.argv[1])
    output_path = Path(sys.argv[2])
    blocks = [
        "#pragma once",
        "",
        "#include <Arduino.h>",
        "",
        "// Generated from assets/ by tools/generate_logo.py.",
    ]
    for name, filename, size, circle in ASSETS:
        data = screen_png(asset_dir / filename, size, circle)
        blocks.extend(
            (
                f"constexpr uint16_t {name}_WIDTH = {size[0]};",
                f"constexpr uint16_t {name}_HEIGHT = {size[1]};",
                f"const uint8_t {name}[] PROGMEM = {{",
                array_lines(data),
                "};",
                f"constexpr size_t {name}_SIZE = sizeof({name});",
                "",
            )
        )
        print(f"{name}: {len(data)} bytes")

    output_path.write_text("\n".join(blocks), encoding="ascii", newline="\n")


if __name__ == "__main__":
    main()
