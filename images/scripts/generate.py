from pathlib import Path
from string import ascii_letters
from typing import TYPE_CHECKING

from PIL import Image

if TYPE_CHECKING:
    from .images import RGB_PIXEL, RGBA_PIXEL, DataImage, Palette, rgb_to_565
else:
    from images import RGB_PIXEL, RGBA_PIXEL, DataImage, Palette, rgb_to_565


ROOT = Path(__file__).parent.parent.absolute()


CODE: list[str] = []


SUITS = ("heart", "diamond", "spade", "club")
NUMBERS = dict(enumerate(("ace", *map(str, range(2, 11)), "jack", "queen", "king"), 1))
LAYOUTS = {k: v for k, v in NUMBERS.items() if k <= 10}
PICTURES = {k: v for k, v in NUMBERS.items() if k > 10}


def compare_layout(
    patternp: RGB_PIXEL | RGBA_PIXEL, layoutp: RGB_PIXEL | RGBA_PIXEL
) -> bool:
    if len(patternp) == 3:
        patternp += (255,)
    if patternp[3] == 0:
        return True
    assert patternp[3] == 255
    if len(layoutp) == 3:
        layoutp += (255,)
    if layoutp[3] == 0:
        return False
    assert layoutp[3] == 255
    return patternp == layoutp


def calculate_layout(
    pattern: Image.Image, layout: Image.Image
) -> frozenset[tuple[int, int]]:
    assert pattern.mode in ("RGB", "RGBA")
    assert layout.mode in ("RGB", "RGBA")

    positions: set[tuple[int, int]] = set()
    for y in range(layout.height - pattern.height + 1):
        for x in range(layout.width - pattern.width + 1):
            if all(
                compare_layout(
                    pattern.getpixel((dx, dy)), layout.getpixel((x + dx, y + dy))
                )
                for dx in range(pattern.width)
                for dy in range(pattern.height)
            ):
                positions.add((x, y))
    return frozenset(positions)


# Transparent and black only
PALETTE_TB = Palette(((0, 0, 0, 0), (0, 0, 0, 255)))
# Transparent and red only
PALETTE_TR = Palette(((0, 0, 0, 0), (255, 0, 0, 255)))


PALETTE_TB_NAME = "PaletteTB"
PALETTE_TR_NAME = "PaletteTR"
CODE.append(
    f"const Palette {PALETTE_TB_NAME} = {PALETTE_TB.to_cpp_object(with_type=True)};\n"
    + f"const Palette {PALETTE_TR_NAME} = {PALETTE_TR.to_cpp_object(with_type=True)};\n"
)


CARD_SIZE = (36, 48)
CARDIM = Image.open(ROOT / "cardbg.png")
CARDBACKIM = Image.open(ROOT / "cardback.png")
CARDSHINEIM = Image.open(ROOT / "cardshine.png")
CARDSHINE_OFFSET = (-2, -2)

BGCOLOUR = (0, 192, 0)

SUITIM = [Image.open(ROOT / "suits" / (v + ".png")) for v in SUITS]
NUMBERIM = {k: Image.open(ROOT / "numbers" / (v + ".png")) for k, v in NUMBERS.items()}
LAYOUTIM = {k: Image.open(ROOT / "layouts" / (v + ".png")) for k, v in LAYOUTS.items()}
PICTUREIM: dict[int, Image.Image] = {}
try:
    PICTUREIM.update(
        {k: Image.open(ROOT / "pictures" / (v + ".png")) for k, v in LAYOUTS.items()}
    )
except FileNotFoundError:
    pass

PICTUREIM[11] = Image.open(ROOT / "cardback.png")


CARDBG_IMAGE_PALETTE_VAR = "CardBGPalette"
CARDBG_IMAGE_VAR = "CardBG"
CARDBGDI = DataImage(CARDIM).pack_image_part()
CODE.append(
    f"const Palette {CARDBG_IMAGE_PALETTE_VAR} = "
    + CARDBGDI.palette.to_cpp_object()
    + f";\nconst Image {CARDBG_IMAGE_VAR} = "
    + CARDBGDI.to_cpp_object(palette_name="&" + CARDBG_IMAGE_PALETTE_VAR)
    + ";\n"
)

CARDBACK_IMAGE_PALETTE_VAR = "CardBackPalette"
CARDBACK_IMAGE_VAR = "CardBack"
CARDBACKDI = DataImage(CARDBACKIM).pack_image_part()
CODE.append(
    f"const Palette {CARDBACK_IMAGE_PALETTE_VAR} = "
    + CARDBACKDI.palette.to_cpp_object()
    + f";\nconst Image {CARDBACK_IMAGE_VAR} = "
    + CARDBACKDI.to_cpp_object(palette_name="&" + CARDBACK_IMAGE_PALETTE_VAR)
    + ";\n"
)

CARDSHINE_IMAGE_PALETTE_VAR = "CardShinePalette"
CARDSHINE_IMAGE_VAR = "CardShine"
CARDSHINEDI = DataImage(CARDSHINEIM).pack_image_part()
CODE.append(
    f"const Palette {CARDSHINE_IMAGE_PALETTE_VAR} = "
    + CARDSHINEDI.palette.to_cpp_object()
    + f";\nconst Image {CARDSHINE_IMAGE_VAR} = "
    + CARDSHINEDI.to_cpp_object(palette_name="&" + CARDSHINE_IMAGE_PALETTE_VAR)
    + ";\n"
)


SUIT_SIZE = (8, 8)
SQUARE = Image.open(ROOT / "suits" / "square.png")
assert SQUARE.size == SUIT_SIZE
assert all(i.size == SUIT_SIZE for i in SUITIM)


def get_suit_image_obj(suit: int) -> str:
    pdi = DataImage(SUITIM[suit], PALETTE_TB).pack_image_part()
    if suit >> 1:  # black
        palette_name = "&" + PALETTE_TB_NAME
    else:  # red
        pdi = pdi.switch_palette(PALETTE_TR)
        palette_name = "&" + PALETTE_TR_NAME
    return pdi.to_cpp_object(palette_name=palette_name)


SUIT_IMAGES_VAR = "SuitImages"
CODE.append(
    f"const Image {SUIT_IMAGES_VAR}[{len(SUITS)}] = "
    + "{\n    "
    + ",\n    ".join(map(get_suit_image_obj, range(len(SUITS))))
    + ",\n};\n"
)


NUM_SIZE = (4, 7)
assert all(i.width == NUM_SIZE[0] for k, i in NUMBERIM.items() if k != 10)
assert all(i.height == NUM_SIZE[1] for i in NUMBERIM.values())
NUM_OFFSET_TL = (2, 2)
NUM_OFFSET_BR = (
    CARD_SIZE[0] - NUM_OFFSET_TL[0] - 1,
    CARD_SIZE[1] - NUM_OFFSET_TL[1] - 1,
)


NUM_IMAGES_DATA_VARS = {num: f"NumImagesData_{num}" for num in NUMBERS}


def get_num_image_data(num: int) -> str:
    pdi = DataImage(NUMBERIM[num], PALETTE_TB).pack_image_part()
    return pdi.to_c_list(with_type=True)


CODE.append(
    "".join(
        f"const unsigned char *const {NUM_IMAGES_DATA_VARS[num]} = {get_num_image_data(num)};\n"
        for num in NUMBERS
    )
)


def get_num_image_obj(num: int, palette: Palette, palette_name: str) -> str:
    pdi = DataImage(NUMBERIM[num], PALETTE_TB).pack_image_part().switch_palette(palette)
    return pdi.to_cpp_object(
        data_name=NUM_IMAGES_DATA_VARS[num], palette_name="&" + palette_name
    )


NUM_IMAGES_VAR = "NumbersImages"
CODE.append(
    f"const Image {NUM_IMAGES_VAR}[2][{len(NUMBERS)+1}] = "
    + "{\n"
    + "".join(
        "    {\n        {},\n        "
        + ",\n        ".join(
            get_num_image_obj(num, palette, name) for num in range(1, len(NUMBERS) + 1)
        )
        + ",\n    },\n"
        for palette, name in (
            (PALETTE_TR, PALETTE_TR_NAME),
            (PALETTE_TB, PALETTE_TB_NAME),
        )
    )
    + "};\n"
)

PICTUREDI = {num: DataImage(PICTUREIM[num]) for num in PICTUREIM}
PICTURES_IMAGE_PALETTE_SET = {p.palette for p in PICTUREDI.values()}
siter = iter(ascii_letters)
PICTURES_IMAGE_NAMED_PALETTES = {next(siter): pal for pal in PICTURES_IMAGE_PALETTE_SET}
PICTURES_IMAGE_PALETTE_NAMES = {
    pal: name for name, pal in PICTURES_IMAGE_NAMED_PALETTES.items()
}
PICTURES_IMAGE_PALETTE_VARS = {
    letter: f"NumImagePalette_{letter}" for letter in PICTURES_IMAGE_NAMED_PALETTES
}

PICTURES_IMAGE_VARS = {num: f"NumImage_{num}" for num in PICTURES}
CODE.append(
    "".join(
        f"const Palette {PICTURES_IMAGE_PALETTE_VARS[letter]} = "
        + palette.to_cpp_object()
        + ";\n"
        for letter, palette in PICTURES_IMAGE_NAMED_PALETTES.items()
    )
    + "\n"
    + "".join(
        f"const Image {PICTURES_IMAGE_VARS[num]} = "
        + PICTUREDI[num]
        .pack_image_part()
        .to_cpp_object(
            palette_name="&"
            + PICTURES_IMAGE_PALETTE_VARS[
                PICTURES_IMAGE_PALETTE_NAMES[PICTUREDI[num].palette]
            ]
        )
        + ";\n"
        for num in PICTUREDI
    )
)


LAYOUT_X = "x"
LAYOUT_Y = "y"
LAYOUT_SUIT_NAME = "suit"


LAYOUTP = {k: calculate_layout(SQUARE, layout) for k, layout in LAYOUTIM.items()}
assert all(len(v) == k for k, v in LAYOUTP.items())
# assert all(i.size == CARD_SIZE for i in PICTUREIM.values())


def flip_layout_pos(pos: tuple[int, int]) -> tuple[int, int, str]:
    x, y = pos
    upsidedown = y >= CARD_SIZE[1] // 2
    return (
        (x + SUIT_SIZE[0] - 1, y + SUIT_SIZE[1] - 1, "true")
        if upsidedown
        else (x, y, "false")
    )


def get_component_switchcase(num: int) -> str:
    return (
        f"    case {num}:\n"
        + "".join(
            f"        {LAYOUT_SUIT_NAME}->CopySprite("
            f"{LAYOUT_X} + {x}, {LAYOUT_Y} + {y}, {s});\n"
            for x, y, s in map(flip_layout_pos, sorted(LAYOUTP.get(num, frozenset())))
        )
        + (
            f"        {PICTURES_IMAGE_VARS[num]}->CopySprite({LAYOUT_X}, {LAYOUT_Y});\n"
            if num in PICTURES
            else ""
        )
        + "        return;\n"
    )


CODE.append(
    f"""\
void DrawCard(Card card, int {LAYOUT_X}, int {LAYOUT_Y})
{{
    {CARDBG_IMAGE_VAR}.CopySprite({LAYOUT_X}, {LAYOUT_Y});
    if (!card.NonNull())
        return;
    const Image *number = &{NUM_IMAGES_VAR}[card.GetColour()][card.GetRank()];
    number->CopySprite({LAYOUT_X} + {NUM_OFFSET_TL[0]}, {LAYOUT_Y} + {NUM_OFFSET_TL[1]}, false);
    number->CopySprite({LAYOUT_X} + {NUM_OFFSET_BR[0]}, {LAYOUT_Y} + {NUM_OFFSET_BR[1]}, true);
    const Image *const {LAYOUT_SUIT_NAME} = &{SUIT_IMAGES_VAR}[card.GetSuit()];
    switch (card.GetRank())
    {{
"""
    + "".join(map(get_component_switchcase, LAYOUTS))
    + """\
    }
}
"""
)


CODE.append(
    f"""\
void DrawCardBack(int {LAYOUT_X}, int {LAYOUT_Y})
{{
    {CARDBACK_IMAGE_VAR}.CopySprite({LAYOUT_X}, {LAYOUT_Y});
}}
"""
)


CODE.append(
    f"""\
void DrawCardShine(int {LAYOUT_X}, int {LAYOUT_Y})
{{
    {CARDSHINE_IMAGE_VAR}.CopySprite({LAYOUT_X} + {CARDSHINE_OFFSET[0]}, {LAYOUT_Y} + {CARDSHINE_OFFSET[1]});
}}
"""
)


CODE.append(f"const color_t BackgroundColour = 0x{rgb_to_565(BGCOLOUR):04x};\n")


CODE_STR = "\n".join(CODE)
# print(CODE_STR, end="")

SRC = ROOT / "scripts" / "imagedata.hpp"
DEST = ROOT.parent / "src" / "gen" / "imagedata.hpp"
START_MARKER = "\n#pragma region IMAGEDATA\n"
END_MARKER = "\n#pragma endregion IMAGEDATA\n"

with open(SRC, "r") as f:
    imagedata_hpp = f.read()

start = imagedata_hpp.find(START_MARKER)
end = imagedata_hpp.find(END_MARKER)
assert start >= 0 and end >= 0
start += len(START_MARKER)

imagedata_hpp = (
    imagedata_hpp[:start] + "\n" + CODE_STR.strip() + "\n" + imagedata_hpp[end:]
)

with open(DEST, "w") as f:
    f.write(imagedata_hpp)
