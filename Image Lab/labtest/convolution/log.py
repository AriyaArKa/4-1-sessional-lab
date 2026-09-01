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


def log_kernel(size, sigma):

    center = size // 2

    kernel = np.zeros(
        (size, size),
        dtype=np.float32
    )

    for i in range(size):

        for j in range(size):

            x = j - center
            y = i - center

            r2 = x*x + y*y

            kernel[i, j] = (
                (r2 - 2*sigma*sigma)
                /
                (sigma**4)
            ) * np.exp(
                -r2 /
                (2*sigma*sigma)
            )

    return kernel


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


size = 3
sigma = 2.0


kernel = log_kernel(
    size,
    sigma
)

output = convolution(
    img,
    kernel
)


output_show = cv2.normalize(
    np.abs(output),
    None,
    0,
    255,
    cv2.NORM_MINMAX
).astype(np.uint8)


plt.figure(figsize=(10,4))


plt.subplot(1,3,1)
plt.imshow(img, cmap='gray')
plt.title("Original")
plt.axis('off')


plt.subplot(1,3,2)
plt.imshow(kernel, cmap='gray')
plt.title("LoG Kernel")
plt.axis('off')


plt.subplot(1,3,3)
plt.imshow(output_show, cmap='gray')
plt.title("LoG Output")
plt.axis('off')


plt.tight_layout()
plt.show()