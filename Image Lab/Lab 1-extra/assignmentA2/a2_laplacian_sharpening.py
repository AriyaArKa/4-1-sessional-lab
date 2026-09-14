import cv2
import numpy as np
import matplotlib.pyplot as plt


image = cv2.imread(r"homework_a2_b2.png", cv2.IMREAD_GRAYSCALE)
image = image.astype(float)

rows, cols = image.shape

kernel = [
    [0, -1, 0],
    [-1, 4, -1],
    [0, -1, 0],
]

#print("kernel ready")

laplacian = np.zeros((rows, cols))

for i in range(1, rows - 1):          
    for j in range(1, cols - 1):
        total = 0
        for m in range(3):
            for n in range(3):
                total += image[i - 1 + m, j - 1 + n] * kernel[m][n]
        laplacian[i, j] = total


#sharping 
c = 2.5
sharp = image + c * laplacian

def normalize(img):
    low = img.min()
    high = img.max()
    return ((img - low) * 255 / (high - low)).astype(np.uint8)


laplacian_show = normalize(laplacian)
output = normalize(sharp)


plt.figure(figsize=(9, 3))

plt.subplot(1, 3, 1)
plt.imshow(image, cmap="gray", vmin=0, vmax=255)
plt.title("Original Image")
plt.axis("off")

plt.subplot(1, 3, 2)
plt.imshow(laplacian_show, cmap="gray", vmin=0, vmax=255)
plt.title("Laplacian")
plt.axis("off")

plt.subplot(1, 3, 3)
plt.imshow(output, cmap="gray", vmin=0, vmax=255)
plt.title("Laplacian Sharpening")
plt.axis("off")

plt.tight_layout()
plt.savefig(r"original_laplacian_sharpening.png", dpi=300, bbox_inches="tight")
plt.show()

cv2.imwrite(r"laplacian.png", laplacian_show)
cv2.imwrite(r"sharpened.png", output)
