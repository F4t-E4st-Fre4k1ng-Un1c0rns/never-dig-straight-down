#!/usr/bin/python
from PIL import Image
import sys
from pathlib import Path

source_file = Path(sys.argv[1])

img = Image.open(sys.argv[1])
img = img.convert("RGBA")

pixdata = img.load()

# Clean the background noise, if color != white, then set to black.

for y in range(img.size[1]):
    for x in range(img.size[0]):
        if pixdata[x, y] == (0, 0, 0, 255) or pixdata[x, y] == (1, 1, 1, 255):
            pixdata[x, y] = (0, 0, 0, 0)

output_file = source_file.rename('cleared_' + source_file.name)
img.save(output_file)
img.close()