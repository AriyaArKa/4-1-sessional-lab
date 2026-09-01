import cv2
import numpy as np
import matplotlib.pyplot as plt
import os

folder = os.path.dirname(os.path.abspath(__file__))

source_path = os.path.join(folder, "boat.jpg")
reference_path = os.path.join(folder, "power_plant.jpg")

output_path = os.path.join(folder, "matched_output.jpg")
figure_path = os.path.join(folder, "histogram_matching_result.png")

source = cv2.imread(source_path)
reference = cv2.imread(reference_path)

if source is None:
    print("Source image not found.")
    exit()

if reference is None:
    print("Reference image not found.")
    exit()


source_lab = cv2.cvtColor(source, cv2.COLOR_BGR2LAB)
reference_lab = cv2.cvtColor(reference, cv2.COLOR_BGR2LAB)


source_L, source_a, source_b = cv2.split(source_lab)
reference_L, reference_a, reference_b = cv2.split(reference_lab)


def calculate_pdf_cdf(channel):

    histogram = np.zeros(256, dtype=np.int32)

    rows, cols = channel.shape

    for i in range(rows):
        for j in range(cols):
            intensity = channel[i, j]
            histogram[intensity] = histogram[intensity] + 1

    total_pixels = rows * cols

    pdf = np.zeros(256, dtype=float)
    cdf = np.zeros(256, dtype=float)

    for i in range(256):
        pdf[i] = histogram[i] / total_pixels

    cdf[0] = pdf[0]

    for i in range(1, 256):
        cdf[i] = cdf[i - 1] + pdf[i]

    return histogram, pdf, cdf


source_hist, source_pdf, source_cdf = calculate_pdf_cdf(source_L)
reference_hist, reference_pdf, reference_cdf = calculate_pdf_cdf(reference_L)


mapping = np.zeros(256, dtype=np.uint8)

for r in range(256):

    source_value = source_cdf[r]
    minimum_difference = 999999

    for z in range(256):
        difference = reference_cdf[z] - source_value
        if difference < 0:
            difference = -difference
        if difference < minimum_difference:
            minimum_difference = difference
            best_value = z

    # Store mapping
    mapping[r] = best_value


matched_L = np.zeros_like(source_L)

rows, cols = source_L.shape

for i in range(rows):
    for j in range(cols):
        old_value = source_L[i, j]
        matched_L[i, j] = mapping[old_value]


output_lab = cv2.merge([matched_L, source_a, source_b])


# LAB back to BGR
output = cv2.cvtColor(output_lab, cv2.COLOR_LAB2BGR)

output_hist, output_pdf, output_cdf = calculate_pdf_cdf(matched_L)

cv2.imwrite(output_path, output)

source_rgb = cv2.cvtColor(source, cv2.COLOR_BGR2RGB)

reference_rgb = cv2.cvtColor(reference, cv2.COLOR_BGR2RGB)

output_rgb = cv2.cvtColor(output, cv2.COLOR_BGR2RGB)


plt.figure(figsize=(14, 9))
plt.suptitle("Roll: 2107055", fontsize=16)


plt.subplot(3, 3, 1)
plt.imshow(source_rgb)
plt.title("Input Image")
plt.axis("off")


plt.subplot(3, 3, 2)
plt.plot(source_pdf, color="red")
plt.title("Source PDF")
plt.xlim([0, 255])
plt.ylim([0, 0.04])
plt.yticks([0.00, 0.01, 0.02, 0.03, 0.04])


plt.subplot(3, 3, 3)
plt.plot(source_cdf, color="black")
plt.title("Source CDF - S(r)")
plt.xlim([0, 255])
plt.ylim([0, 1.05])
plt.yticks([0.0, 0.2, 0.4, 0.6, 0.8, 1.0])


plt.subplot(3, 3, 4)
plt.imshow(reference_rgb)
plt.title("Reference Image")
plt.axis("off")


plt.subplot(3, 3, 5)
plt.plot(reference_pdf, color="green")
plt.title("Reference PDF")
plt.xlim([0, 255])
plt.ylim([0, 0.04])
plt.yticks([0.00, 0.01, 0.02, 0.03, 0.04])


plt.subplot(3, 3, 6)
plt.plot(reference_cdf, color="green")
plt.title("Reference CDF - G(z)")
plt.xlim([0, 255])
plt.ylim([0, 1.05])
plt.yticks([0.0, 0.2, 0.4, 0.6, 0.8, 1.0])


plt.subplot(3, 3, 7)
plt.imshow(output_rgb)
plt.title("Output Image")
plt.axis("off")


plt.subplot(3, 3, 8)
plt.plot(output_pdf, color="blue")
plt.title("Output PDF")
plt.xlim([0, 255])
plt.ylim([0, 0.04])
plt.yticks([0.00, 0.01, 0.02, 0.03, 0.04])


plt.subplot(3, 3, 9)
plt.plot(output_cdf, color="blue")
plt.title("Output CDF")
plt.xlim([0, 255])
plt.ylim([0, 1.05])
plt.yticks([0.0, 0.2, 0.4, 0.6, 0.8, 1.0])

plt.figtext(0.5, 0.01, "Figure: Histogram Matching", ha="center", fontsize=14)
plt.tight_layout(rect=[0, 0.05, 1, 0.95])

plt.savefig(figure_path, dpi=300)

plt.show()
print("Histogram matching completed successfully.")
print("Output image saved as:", output_path)
print("Result figure saved as:", figure_path)
