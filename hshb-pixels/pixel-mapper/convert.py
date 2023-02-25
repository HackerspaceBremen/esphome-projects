#!/usr/bin/env python3

import csv


def write_pixel_layout():
    with open("hshb_pixels_layout.csv", newline="") as csvfile:
        reader = csv.reader(csvfile, delimiter=",", quotechar='"')
        rows = [row[1:] for row in list(reader)[1:]]
        found_numbers = set()
        height = len(rows)
        width = len(rows[0])
        code = ""
        for y, row in enumerate(rows):
            assert len(row) == width, "Row with invalid width found"
            for x, value in enumerate(row):
                if value:
                    value = int(value)
                    assert value not in found_numbers, "Number duplicated"
                    found_numbers.add(value)
                    code += "  if (x == {} && y == {}) return {};\n".format(x, y, value)

    with open("../software/include/pixel-layout.h", "w") as f:
        f.write(
            """#ifndef HSHB_PIXELS_LAYOUT_H
#define HSHB_PIXELS_LAYOUT_H

int pixel_address(int x, int y) {
"""
        )
        f.write(code)
        f.write("  return {};".format(1 + max(found_numbers)))
        f.write(
            """
}

#endif
"""
        )


def write_pixel_outline():
    with open("hshb_pixels_outline.csv", newline="") as csvfile:
        reader = csv.reader(csvfile, delimiter=",", quotechar='"')
        rows = [row[1:] for row in list(reader)[1:]]
        found_values = set()
        found_numbers = set()
        height = len(rows)
        width = len(rows[0])
        coordinates = [""] * 200
        addresses = [""] * 200
        for y, row in enumerate(rows):
            assert len(row) == width, "Row with invalid width found"
            for x, value in enumerate(row):
                if value:
                    value, number = value.split("/")
                    assert value not in found_values, f"Value {value} duplicated"
                    found_values.add(str(value))
                    assert number not in found_numbers, f"Number {number} duplicated"
                    found_numbers.add(str(number))
                    coordinates[int(number)] = "{}, {}".format(x, y)
                    addresses[int(number)] = value

    coordinates = [c for c in coordinates if c]
    addresses = [a for a in addresses if a]
    with open("../software/include/pixel-outline.h", "w") as f:
        f.write(
            """#ifndef HSHB_PIXELS_OUTLINE_H
#define HSHB_PIXELS_OUTLINE_H

constexpr const int OUTLINE_PIXEL_COUNT = {};
constexpr const int OUTLINE_PIXEL_COORDS[] = {{{}}};
constexpr const int OUTLINE_PIXEL_ADDRS[] = {{{}}};

#endif
""".format(
                len(addresses),
                ", ".join(coordinates),
                ", ".join(addresses),
            )
        )


if __name__ == "__main__":
    write_pixel_layout()
    write_pixel_outline()
