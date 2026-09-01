import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.imread(
    'labtest/Inputs/retina_2.png',
    cv2.IMREAD_GRAYSCALE
)

if img is None:
    print("Image not found")
    exit()


def convolution(img, kernel):

    height, width = img.shape

    kh, kw = kernel.shape

    ph = kh // 2
    pw = kw // 2

    padded_img = np.pad(
        img.astype(np.float32),
        ((ph, ph), (pw, pw)),
        mode="constant"
    )

    output = np.zeros(
        img.shape,
        dtype=np.float32
    )

    for i in range(height):

        for j in range(width):

            total = 0

            for m in range(kh):

                for n in range(kw):

                    total += (
                        padded_img[i+m, j+n]
                        *
                        kernel[
                            kh-1-m,
                            kw-1-n
                        ]
                    )

            output[i,j] = total

    return output


# =========================
# Sobel X
# =========================

sobel_x = np.array([
    [-1, 0, 1],
    [-2, 0, 2],
    [-1, 0, 1]
], dtype=np.float32)


# =========================
# Sobel Y
# =========================

sobel_y = np.array([
    [-1, -2, -1],
    [ 0,  0,  0],
    [ 1,  2,  1]
], dtype=np.float32)


# =========================
# X response
# =========================

gx = convolution(
    img,
    sobel_x
)


# =========================
# Y response
# =========================

gy = convolution(
    img,
    sobel_y
)


# =========================
# X + Y
# =========================

magnitude = np.sqrt(
    gx**2 + gy**2
)


# =========================
# Prepare for display
# =========================

gx_show = cv2.normalize(
    np.abs(gx),
    None,
    0,
    255,
    cv2.NORM_MINMAX
).astype(np.uint8)


gy_show = cv2.normalize(
    np.abs(gy),
    None,
    0,
    255,
    cv2.NORM_MINMAX
).astype(np.uint8)


magnitude_show = cv2.normalize(
    magnitude,
    None,
    0,
    255,
    cv2.NORM_MINMAX
).astype(np.uint8)


# =========================
# Display
# =========================

plt.figure(figsize=(10, 8))


plt.subplot(2, 2, 1)
plt.imshow(img, cmap='gray')
plt.title("Original")
plt.axis('off')


plt.subplot(2, 2, 2)
plt.imshow(gx_show, cmap='gray')
plt.title("Sobel X")
plt.axis('off')


plt.subplot(2, 2, 3)
plt.imshow(gy_show, cmap='gray')
plt.title("Sobel Y")
plt.axis('off')


plt.subplot(2, 2, 4)
plt.imshow(magnitude_show, cmap='gray')
plt.title("Sobel X + Y")
plt.axis('off')


plt.tight_layout()
plt.show()