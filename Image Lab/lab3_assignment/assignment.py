import cv2
import numpy as np
import matplotlib.pyplot as plt


#periodic noise
def generate_noise(image, d=45, d0=2, A=1500000):

    rows, cols = image.shape

    F = np.fft.fftshift(np.fft.fft2(image))

    magnitude = np.abs(F)
    phase = np.angle(F)

    center_u = rows // 2
    center_v = cols // 2

    # Four noise locations
    for u in range(rows):
        for v in range(cols):

            points = [
                (center_u + d, center_v + d),
                (center_u - d, center_v - d),
                (center_u + d, center_v - d),
                (center_u - d, center_v + d),
            ]

            for pu, pv in points:
                if (u - pu) ** 2 + (v - pv) ** 2 <= d0**2:
                    magnitude[u, v] += A

    # Reconstruct noisy frequency spectrum
    F = magnitude * np.exp(1j * phase)

    # Inverse Fourier Transform
    noisy_image = np.fft.ifft2(np.fft.ifftshift(F))
    noisy_image = np.real(noisy_image)
    return np.clip(noisy_image, 0, 255).astype(np.uint8)

#distance form center
def distance_from_center(rows, cols):

    D = np.zeros((rows, cols))

    center_u = rows // 2
    center_v = cols // 2

    for u in range(rows):
        for v in range(cols):
            D[u, v] = np.sqrt((u - center_u) ** 2 + (v - center_v) ** 2)
    return D


#ideal band reject filter
def ideal_band_reject(D, D0, W):

    H = np.ones(D.shape)

    for u in range(D.shape[0]):
        for v in range(D.shape[1]):
            if abs(D[u, v] - D0) <= W / 2:
                H[u, v] = 0

    return H


#butterworth band reject filter
def butterworth_band_reject(D, D0, W, n):

    H = np.ones(D.shape)

    for u in range(D.shape[0]):
        for v in range(D.shape[1]):
            d = D[u, v]
            denominator = d**2 - D0**2
            if abs(denominator) < 1e-12:
                H[u, v] = 0
            else:
                value = (d * W) / denominator
                H[u, v] = 1 / (1 + abs(value) ** (2 * n))

    return H


#gaussian band reject
def gaussian_band_reject(D, D0, W):

    H = np.ones(D.shape)

    for u in range(D.shape[0]):
        for v in range(D.shape[1]):
            d = D[u, v]
            if d == 0:
                H[u, v] = 1
            else:
                value = (d**2 - D0**2) / (d * W)
                H[u, v] = 1 - np.exp(-0.5 * value**2)
    return H

#filter apply
def apply_filter(image, H):

    # Fourier Transform
    F = np.fft.fft2(image)

    # Move zero frequency to center
    F_shift = np.fft.fftshift(F)

    # Apply filter
    G_shift = F_shift * H

    # Move back
    G = np.fft.ifftshift(G_shift)

    # Inverse Fourier Transform
    result = np.fft.ifft2(G)
    result = np.real(result)
    return np.clip(result, 0, 255).astype(np.uint8)




# Read original image
image = cv2.imread("lab3_assignment/lena.jpeg", cv2.IMREAD_GRAYSCALE)

if image is None:
    print("Image not found!")
    exit()


# Generate noisy image
noisy_image = generate_noise(image)

# Image size
rows, cols = noisy_image.shape

# Distance matrix
D = distance_from_center(rows, cols)

# Noise frequency location
D0 = np.sqrt(45**2 + 45**2)

# Filter parameters
W = 8
n = 2

#3 filter creation
H_ideal = ideal_band_reject(D, D0, W)
H_butterworth = butterworth_band_reject(D, D0, W, n)
H_gaussian = gaussian_band_reject(D, D0, W)

#3 filter apply
ideal_image = apply_filter(noisy_image, H_ideal)
butterworth_image = apply_filter(noisy_image, H_butterworth)
gaussian_image = apply_filter(noisy_image, H_gaussian)


plt.figure(figsize=(12, 12))


#ideal
plt.subplot(3, 3, 1)
plt.imshow(noisy_image, cmap="gray", vmin=0, vmax=255)
plt.title("Input Corrupted Image with Noise")
plt.axis("off")

plt.subplot(3, 3, 2)
plt.imshow(H_ideal, cmap="gray", vmin=0, vmax=1)
plt.title("Ideal Band Reject Filter")
plt.axis("off")

plt.subplot(3, 3, 3)
plt.imshow(ideal_image, cmap="gray", vmin=0, vmax=255)
plt.title("Reconstructed Image")
plt.axis("off")


#butterworth
plt.subplot(3, 3, 4)
plt.imshow(noisy_image, cmap="gray", vmin=0, vmax=255)
plt.title("Input Corrupted Image with Noise")
plt.axis("off")

plt.subplot(3, 3, 5)
plt.imshow(H_butterworth, cmap="gray", vmin=0, vmax=1)
plt.title("Butterworth Band Reject Filter")
plt.axis("off")

plt.subplot(3, 3, 6)
plt.imshow(butterworth_image, cmap="gray", vmin=0, vmax=255)
plt.title("Reconstructed Image")
plt.axis("off")


#gaussian
plt.subplot(3, 3, 7)
plt.imshow(noisy_image, cmap="gray", vmin=0, vmax=255)
plt.title("Input Corrupted Image with Noise")
plt.axis("off")

plt.subplot(3, 3, 8)
plt.imshow(H_gaussian, cmap="gray", vmin=0, vmax=1)
plt.title("Gaussian Band Reject Filter")
plt.axis("off")

plt.subplot(3, 3, 9)
plt.imshow(gaussian_image, cmap="gray", vmin=0, vmax=255)
plt.title("Reconstructed Image")
plt.axis("off")


# Figure name
plt.figtext(
    0.5,
    0.01,
    "Figure 1: Comparison of Band Reject Filters",
    ha="center",
    fontsize=14,
    fontweight="bold",
)

plt.tight_layout(rect=[0, 0.03, 1, 1])
# Save output
plt.savefig("lab3_assignment/band_reject_results.png", dpi=300, bbox_inches="tight")
plt.show()
