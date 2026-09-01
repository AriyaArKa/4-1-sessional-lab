import cv2
import numpy as np
import matplotlib.pyplot as plt

# ==========================================
# READ INPUT IMAGE
# ==========================================

img = cv2.imread("labtest/Inputs/boat.jpg")

if img is None:
    print("Image not found")
    exit()


# Convert BGR -> LAB
img_lab = cv2.cvtColor(img, cv2.COLOR_BGR2LAB)


# ==========================================
# READ REFERENCE IMAGE
# ==========================================

ref_img = cv2.imread("labtest/Inputs/power_plant.jpg")

if ref_img is None:
    print("Reference image not found")
    exit()


ref_img_lab = cv2.cvtColor(ref_img, cv2.COLOR_BGR2LAB)


# ==========================================
# PDF FUNCTION
# ==========================================


def get_pdf(channel):

    height = channel.shape[0]
    width = channel.shape[1]

    pdf = np.zeros(256, dtype=np.float32)

    for i in range(height):

        for j in range(width):

            pdf[channel[i, j]] += 1

    pdf /= height * width

    return pdf


# ==========================================
# CDF FUNCTION
# ==========================================


def get_cdf(pdf):

    cdf = np.zeros_like(pdf)

    cdf[0] = pdf[0]

    for i in range(1, 256):

        cdf[i] = cdf[i - 1] + pdf[i]

    return cdf


# ==========================================
# GET L CHANNELS
# ==========================================

l, a, b = cv2.split(img_lab)

l_ref, a_ref, b_ref = cv2.split(ref_img_lab)


# ==========================================
# ORIGINAL AND REFERENCE PDF
# ==========================================

pdf_l = get_pdf(l)

pdf_l_ref = get_pdf(l_ref)


# ==========================================
# ORIGINAL AND REFERENCE CDF
# ==========================================

cdf_l = get_cdf(pdf_l)

cdf_l_ref = get_cdf(pdf_l_ref)


# ==========================================
# CREATE MAPPING
# ==========================================

mapping = np.zeros(256, dtype=np.uint8)

for r in range(256):

    # CDF value of input intensity r
    cdf_value = cdf_l[r]

    # Difference with every reference CDF value
    diff_array = np.abs(cdf_l_ref - cdf_value)

    # Find reference intensity with
    # closest CDF value
    s = np.argmin(diff_array)

    # r -> s
    mapping[r] = s


# ==========================================
# APPLY MAPPING TO L CHANNEL
# ==========================================

height, width = l.shape

new_l = np.zeros_like(l)

for i in range(height):

    for j in range(width):

        r = l[i, j]

        new_l[i, j] = mapping[r]


# ==========================================
# PUT NEW L INTO LAB IMAGE
# ==========================================

new_img_lab = img_lab.copy()

new_img_lab[:, :, 0] = new_l


# ==========================================
# NEW PDF AND CDF
# ==========================================

new_pdf_l = get_pdf(new_l)

new_cdf_l = get_cdf(new_pdf_l)


# ==========================================
# CONVERT FOR MATPLOTLIB
# ==========================================

original_rgb = cv2.cvtColor(img_lab, cv2.COLOR_LAB2RGB)

reference_rgb = cv2.cvtColor(ref_img_lab, cv2.COLOR_LAB2RGB)

matched_rgb = cv2.cvtColor(new_img_lab, cv2.COLOR_LAB2RGB)


fig, ax = plt.subplots(
    3,
    3,
    figsize=(16, 8)
)


# ==========================================
# ROW 1
# ==========================================

# Input Image
ax[0,0].imshow(original_rgb)
ax[0,0].set_title("Input Image")
ax[0,0].axis("off")


# Source PDF
ax[0,1].plot(pdf_l)
ax[0,1].set_title("Source PDF")
ax[0,1].set_xlim(0, 255)
ax[0,1].set_ylim(0, 0.04)


# Source CDF
ax[0,2].plot(cdf_l)
ax[0,2].set_title("Source CDF - S(r)")
ax[0,2].set_xlim(0, 255)
ax[0,2].set_ylim(0, 1.05)


# ==========================================
# ROW 2
# ==========================================

# Reference Image
ax[1,0].imshow(reference_rgb)
ax[1,0].set_title("Reference Image")
ax[1,0].axis("off")


# Reference PDF
ax[1,1].plot(pdf_l_ref)
ax[1,1].set_title("Reference PDF")
ax[1,1].set_xlim(0, 255)
ax[1,1].set_ylim(0, 0.04)


# Reference CDF
ax[1,2].plot(cdf_l_ref)
ax[1,2].set_title("Reference CDF - G(z)")
ax[1,2].set_xlim(0, 255)
ax[1,2].set_ylim(0, 1.05)


# ==========================================
# ROW 3
# ==========================================

# Output Image
ax[2,0].imshow(matched_rgb)
ax[2,0].set_title("Output Image")
ax[2,0].axis("off")


# Output PDF
ax[2,1].plot(new_pdf_l)
ax[2,1].set_title("Output PDF")
ax[2,1].set_xlim(0, 255)
ax[2,1].set_ylim(0, 0.04)


# Output CDF
ax[2,2].plot(new_cdf_l)
ax[2,2].set_title("Output CDF")
ax[2,2].set_xlim(0, 255)
ax[2,2].set_ylim(0, 1.05)


# ==========================================
# REMOVE EXTRA SPACE
# ==========================================

plt.subplots_adjust(
    left=0.03,
    right=0.98,
    top=0.95,
    bottom=0.06,
    wspace=0.15,
    hspace=0.25
)

plt.show()