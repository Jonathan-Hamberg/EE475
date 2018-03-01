# This Python script converts an atlas image into a bytestream that can be
# stored on flash memory on an arduino.
# The program expects the atlas to be in a 5 row by 6 column grid where each
# sub-picture is 64 x 64 pixels wide.
# The output stream contains the first sub-picture then the second sub-picture
# and so on. The output format is as follows, the first 64 bits of the 8 bytes
# contain the first row of the forst image.  The next 64 bits contain the second
# row of the first image.  This continues until the next image is encounted and
# the same process is repeated for the number of images left.
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

# Read the image to be processed.
img = mpimg.imread('bomb_image_icons.png')

# Number of sub-images in the atlas.
numItems = 26

# Contain the output byte array.
output = []

byteValue = 0
byteIndex = 0
# Iterate through each character.
for row in range(5):
    for col in range(6):
        # Iterate through each pixel.
        for y in range(64):
            for x in range(64):
                # Shift the data for the next color bit.
                byteValue <<= 1
                # Increment the byte index.
                byteIndex += 1
                if img[row * 64 + y, col * 64 + x][0] > 0.5:
                    # Or the white bit into the byte.
                    byteValue |= 1

                # Add the byte to the output array and reset the counter.
                if byteIndex >= 8:
                    output.append(byteValue)
                    byteIndex = 0
                    byteValue = 0


# Only use the numItems of the array.
output = output[0:numItems * 64 * 64 // 8]

# Convert the output buffer into a c array style output that can be included
# in a Arduino program.
counter = 0
for i in range(numItems * 64 * 64 // 8):
    print(str.format('0x{:02X}', output[i]) + ",", end="")
    counter = counter + 1
    if (counter % 16) == 0:
        print()
