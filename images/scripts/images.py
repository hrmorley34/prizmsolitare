from dataclasses import dataclass
from math import ceil, log2
from typing import Any, Callable, Iterable, NewType

from PIL import Image

RGBA_PIXEL = tuple[int, int, int, int]
RGB_PIXEL = tuple[int, int, int]
C565 = NewType("C565", int)


def rgb_to_565(colour: RGB_PIXEL | RGBA_PIXEL) -> C565 | None:
    if len(colour) == 3:
        colour += (255,)
    assert len(colour) == 4
    if colour[3] == 0:
        return None
    assert colour[3] == 255
    x = C565((((colour[0] >> 3 << 6) + (colour[1] >> 2)) << 5) + (colour[2] >> 3))
    return x


def c565_to_rgb(colour: C565 | None) -> RGBA_PIXEL:
    if colour is None:
        return (0, 0, 0, 255)
    return (
        (colour >> (5 + 6) << 3),
        (((colour >> 5) & 0b11_1111) << 2),
        ((colour & 0b1_1111) << 3),
        255,
    )


@dataclass
class Palette:
    data: tuple[C565 | None, ...]
    width: int
    pixels_per_byte: int
    bitmask: int
    transparent: int

    def __init__(self, data: Iterable[RGB_PIXEL | RGBA_PIXEL]) -> None:
        self.data = tuple(
            sorted(set(map(rgb_to_565, data)), key=lambda c: -1 if c is None else c)
        )
        self.width = ceil(log2(len(self.data)))
        self.pixels_per_byte = 8 // self.width
        self.bitmask = (1 << self.width) - 1
        self.transparent = self.data.index(None) if None in self.data else -1

    def __hash__(self) -> int:
        return hash(
            (
                self.data,
                self.width,
                self.pixels_per_byte,
                self.bitmask,
                self.transparent,
            )
        )

    def rgb_to_palette(self, colour: RGB_PIXEL | RGBA_PIXEL) -> int:
        c565 = rgb_to_565(colour)
        if c565 not in self.data:
            raise ValueError(f"Unknown colour: {colour}")
        return self.data.index(c565)

    def c565_from_palette(self, colour: int) -> C565 | None:
        if colour < 0 or colour >= len(self.data):
            raise ValueError(f"Unknown colour: {colour}")
        return self.data[colour]

    def rgb_from_palette(self, colour: int) -> RGBA_PIXEL:
        return c565_to_rgb(self.c565_from_palette(colour))

    def to_c_list(self, with_type: bool = False) -> str:
        prefix = "(const color_t[])" if with_type else ""
        return (
            prefix
            + "{"
            + ", ".join("0" if c is None else f"0x{c:04x}" for c in self.data)
            + "}"
        )

    def to_new_c_list(self) -> str:
        assert False
        return f"new color_t[{len(self.data)}]{self.to_c_list()}"

    def to_cpp_object(self, with_type: bool = False) -> str:
        prefix = "(const Palette)" if with_type else ""
        return (
            prefix
            + "{"
            + f"{self.to_c_list(with_type=True)}, {self.width}, "
            + f"{self.pixels_per_byte}, 0b{self.bitmask:b}, {self.transparent}"
            + "}"
        )

    def to_new_cpp_object(self) -> str:
        assert False
        return "new Palette" + self.to_cpp_object()


class PalettisedDataImage:
    data: bytes
    palette: Palette
    width: int
    height: int

    def __init__(self, data: bytes, size: tuple[int, int], palette: Palette) -> None:
        self.data = data
        self.palette = palette
        self.width, self.height = size

    def switch_palette(self, palette: Palette):
        return PalettisedDataImage(self.data, (self.width, self.height), palette)

    def to_c_list(self, with_type: bool = False) -> str:
        prefix = "(const unsigned char[])" if with_type else ""
        return prefix + "{" + ", ".join(f"0x{d:02x}" for d in self.data) + "}"

    def to_new_c_list(self) -> str:
        assert False
        return f"new unsigned char[{len(self.data)}]{self.to_c_list()}"

    def to_cpp_object(self, palette_name: str, data_name: str | None = None) -> str:
        return (
            "{"
            + f"{data_name or self.to_c_list(with_type=True)}, "
            + f"{palette_name}, "
            + f"{self.width}, {self.height}"
            + "}"
        )

    def to_new_cpp_object(
        self, data_name: str | None = None, palette_name: str | None = None
    ) -> str:
        assert False
        return "new Image" + self.to_cpp_object(data_name, palette_name)


class DataImage:
    im: Image.Image
    palette: Palette

    def __init__(self, im: Image.Image, palette: Palette | None = None) -> None:
        self.im = im
        assert im.mode in ("RGB", "RGBA")
        if palette is None:
            self.palette = Palette(c for _, c in im.getcolors(256))
        else:
            self.palette = palette

    @classmethod
    def from_palette_mapping(
        cls, im: Image.Image, mapper: Callable[[Any], RGB_PIXEL | RGBA_PIXEL]
    ):
        palette = Palette(mapper(c) for _, c in im.getcolors(256**3))
        return cls(im, palette)

    def get_packed_length(self, size: tuple[int, int]) -> int:
        width = ceil(size[0] / self.palette.pixels_per_byte)
        return width * size[1]

    def pack_image_part(
        self, position: tuple[int, int] = (0, 0), size: tuple[int, int] | None = None
    ) -> PalettisedDataImage:
        if size is None:
            size = self.im.size

        arr = bytearray()
        for y in range(position[1], position[1] + size[1]):
            for x in range(
                position[0], position[0] + size[0], self.palette.pixels_per_byte
            ):
                value = 0
                for dx in range(self.palette.pixels_per_byte):
                    if x + dx >= position[0] + size[0]:
                        break
                    pix_t: RGB_PIXEL | RGBA_PIXEL = self.im.getpixel((x + dx, y))
                    pix_i = self.palette.rgb_to_palette(pix_t)
                    value |= pix_i << (8 - (dx + 1) * self.palette.width)
                arr.append(value)
        assert len(arr) == self.get_packed_length(size)
        return PalettisedDataImage(bytes(arr), size, self.palette)

    def unpack_image_part(
        self, data: bytes, position: tuple[int, int], size: tuple[int, int]
    ) -> None:
        arr = bytearray(data)
        assert len(arr) == self.get_packed_length(size)
        for y in range(position[1], position[1] + size[1]):
            for x in range(
                position[0], position[0] + size[0], self.palette.pixels_per_byte
            ):
                value = arr.pop(0)
                for dx in range(self.palette.pixels_per_byte):
                    if x + dx >= position[0] + size[0]:
                        break
                    pix_i = value >> (8 - (dx + 1) * self.palette.width)
                    pix_i &= self.palette.bitmask
                    pix_t = self.palette.rgb_from_palette(pix_i)
                    self.im.putpixel((x + dx, y), pix_t)
        assert not arr


# def group(strings: Iterable[str], length: int, perline: int):
#     it = iter(strings)
#     for gi in range(0, length, perline):
#         yield (next(it) for _ in range(gi, min(gi + perline, length)))
