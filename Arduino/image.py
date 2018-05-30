from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

img = mpimg.imread('bomb_image_icons.png')

print(len(img))
print(len(img[0]))
print(img[0, 0])

numItems = 27
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
                if img[row * 64 + y, col * 64 + x][0] < 0.5:
                    # Or the white bit into the byte.
                    byteValue |= 1

                # Add the byte to the output array and reset the counter.
                if byteIndex >= 8:
                    output.append(byteValue)
                    byteIndex = 0
                    byteValue = 0


# Only use the numItems of the array.
output = output[0:numItems * 64 * 64 // 8]

counter = 0
for i in range(numItems * 64 * 64 // 8):
    print(str.format('0x{:02X}', output[i]) + ",", end="")
    counter = counter + 1
    if (counter % 16) == 0:
        print()

print(len(output))
plt.imshow(img)
plt.show()
