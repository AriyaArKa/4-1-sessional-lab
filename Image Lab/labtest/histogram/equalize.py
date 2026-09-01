import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread("labtest/Inputs/retina_2.png", cv2.IMREAD_GRAYSCALE)

if img is None:
    print("Image not found")
    exit()

height = img.shape[0]
width = img.shape[1]


def get_hist(img):

    hist = np.zeros(256, dtype=np.int32)

    for i in range(height):
        for j in range(width):
            hist[img[i, j]] += 1

    return hist


# pdf
def get_pdf(img):
    pdf = np.zeros(256, dtype=np.float32)

    for i in range(height):
        for j in range(width):
            pdf[img[i, j]] += 1  # frequency

    pdf /= height * width  # normalize pk = (rk)/m*n

    return pdf


# cdf
def get_cdf(pdf):
    cdf = np.zeros_like(pdf)
    cdf[0] = pdf[0]
    for i in range(1, 256):
        cdf[i] = cdf[i - 1] + pdf[i]

    return cdf


hist = get_hist(img)
pdf = get_pdf(img)
cdf = get_cdf(pdf)

# mapping
m = np.zeros_like(pdf)
for i in range(256):
    m[i] = np.round(cdf[i] * 255)  # (l-1) * cdf(rk) = (255) * cdf(rk)


output = np.zeros_like(img)
for i in range(height):
    for j in range(width):
        output[i, j] = m[img[i, j]]

new_hist = get_hist(output)
new_pdf = get_pdf(output)
new_cdf = get_cdf(new_pdf)

plt.figure(figsize=(14, 20))
plt.subplot(4, 2, 1)
plt.imshow(img, cmap="gray")
plt.title("Original Image")
plt.axis("off")


plt.subplot(4, 2, 2)
plt.imshow(output, cmap="gray")
plt.title("Histogram Equalized Image")
plt.axis("off")


plt.subplot(4, 2, 3)
plt.plot(hist)
plt.title("Histogram of Input Image")
plt.xlabel("Pixel Intensity")
plt.ylabel("Number of Pixels")
plt.xlim([0, 255])


plt.subplot(4, 2, 4)
plt.plot(new_hist)
plt.title("Histogram of Equalized Image")
plt.xlabel("Pixel Intensity")
plt.ylabel("Number of Pixels")
plt.xlim([0, 255])


plt.subplot(4, 2, 5)
plt.plot(pdf)
plt.title("Original PDF")
plt.xlabel("Pixel Intensity")
plt.ylabel("Probability")


plt.subplot(4, 2, 6)
plt.plot(new_pdf)
plt.title("Equalized PDF")
plt.xlabel("Pixel Intensity")
plt.ylabel("Probability")


plt.subplot(4, 2, 7)
plt.plot(cdf)
plt.title("Original CDF")
plt.xlabel("Pixel Intensity")
plt.ylabel("Cumulative Probability")


plt.subplot(4, 2, 8)
plt.plot(new_cdf)
plt.title("Equalized CDF")
plt.xlabel("Pixel Intensity")
plt.ylabel("Cumulative Probability")


plt.tight_layout()
plt.subplots_adjust(
    hspace=0.5,
    wspace=0.35
)
plt.show()
