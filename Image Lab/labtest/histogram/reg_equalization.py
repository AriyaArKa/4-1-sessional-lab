import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread("labtest/Inputs/boat.jpg")

if img is None:
    print("Image not found")
    exit()


img_rgb = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

img = img_rgb


height = img.shape[0]
width = img.shape[1]

r,g,b = cv2.split(img)


def get_hist(img):

    hist = np.zeros(256, dtype=np.int32)

    for i in range(height):
        for j in range(width):
            hist[img[i, j]] += 1

    return hist

# pdf
def get_pdf(channel):

    ch_h = channel.shape[0]
    ch_w = channel.shape[1]
    
    pdf = np.zeros(256, dtype=np.float32)

    for i in range(ch_h):
        for j in range(ch_w):
            pdf[channel[i, j]] += 1  # frequency

    pdf /= ch_h * ch_w  # normalize pk = (rk)/m*n

    return pdf


# cdf
def get_cdf(pdf):
    cdf = np.zeros_like(pdf)
    cdf[0] = pdf[0]
    for i in range(1, 256):
        cdf[i] = cdf[i - 1] + pdf[i]

    return cdf


pdf_r = get_pdf(r)
pdf_g = get_pdf(g)
pdf_b = get_pdf(b)

cdf_r = get_cdf(pdf_r)
cdf_g = get_cdf(pdf_g)
cdf_b = get_cdf(pdf_b)



# mapping
map_r = np.zeros_like(pdf_r)
map_g = np.zeros_like(pdf_g)
map_b = np.zeros_like(pdf_b)
for i in range(256):
    map_r[i] = np.round(cdf_r[i] * 255)
    map_g[i] = np.round(cdf_g[i] * 255)
    map_b[i] = np.round(cdf_b[i] * 255)


output_r = np.zeros_like(r)
output_g = np.zeros_like(g)
output_b = np.zeros_like(b)
for i in range(height):
    for j in range(width):
        output_r[i, j] = map_r[r[i, j]]
        output_g[i, j] = map_g[g[i, j]]
        output_b[i, j] = map_b[b[i, j]]


new_pdf_r = get_pdf(output_r)
new_pdf_g = get_pdf(output_g)
new_pdf_b = get_pdf(output_b)

new_cdf_r = get_cdf(new_pdf_r)
new_cdf_g = get_cdf(new_pdf_g)
new_cdf_b = get_cdf(new_pdf_b)

new_img = cv2.merge((output_r, output_g, output_b))

plt.figure(figsize=(14, 20))
plt.subplot(3, 2, 1)
plt.imshow(img, cmap="gray")
plt.title("Original Image")
plt.axis("off")


plt.subplot(3, 2, 2)
plt.imshow(new_img)
plt.title("Histogram Equalized Image")
plt.axis("off")


plt.subplot(3, 2, 3)
plt.plot(pdf_r, color='r')
plt.plot(pdf_g, color='g')
plt.plot(pdf_b, color='b')
plt.title("PDF")
plt.legend(["Original Red Channel", "Original Green Channel", "Original Blue Channel"])

plt.subplot(3, 2, 4)
plt.plot(cdf_r, color='r')
plt.plot(cdf_g, color='g')
plt.plot(cdf_b, color='b')
plt.title("CDF")
plt.legend(["Original Red Channel", "Original Green Channel", "Original Blue Channel"])



plt.subplot(3, 2, 5)
plt.plot(new_pdf_r, color='r')
plt.plot(new_pdf_g, color='g')
plt.plot(new_pdf_b, color='b')
plt.title("Equalized PDF")
plt.legend(["Equalized Red Channel", "Equalized Green Channel", "Equalized Blue Channel"])


#cdf

plt.subplot(3, 2, 6)
plt.plot(new_cdf_r, color='r')
plt.plot(new_cdf_g, color='g')
plt.plot(new_cdf_b, color='b')
plt.title("Equalized CDF")
plt.legend(["Equalized Red Channel", "Equalized Green Channel", "Equalized Blue Channel"])



plt.tight_layout()

plt.show()
