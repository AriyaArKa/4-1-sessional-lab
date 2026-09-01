import cv2
import numpy as np
import matplotlib.pyplot as plt

# =========================================
# 1. READ IMAGE AS GRAYSCALE
# =========================================

img = cv2.imread("labtest/Inputs/pnois2.jpg", cv2.IMREAD_GRAYSCALE)

if img is None:
    print("Image not found")
    exit()


# =========================================
# 2. FOURIER TRANSFORM
# =========================================

ft = np.fft.fft2(img)

ft_shift = np.fft.fftshift(ft)


# =========================================
# 3. MAGNITUDE AND PHASE
# =========================================

magnitude = np.abs(ft_shift)

phase = np.angle(ft_shift)


# For displaying original spectrum
magnitude_spectrum = 20 * np.log(magnitude + 1)


# =========================================
# 4. IMAGE SIZE + CENTER
# =========================================

h, w = img.shape

cy = h // 2
cx = w // 2


# =========================================
# 5. CREATE GAUSSIAN LOW PASS FILTER
# =========================================

D0 = 50

H = np.zeros((h, w), dtype=np.float32)


for u in range(h):

    for v in range(w):

        D = np.sqrt((u - cy) ** 2 + (v - cx) ** 2)

        H[u, v] = np.exp(-(D**2) / (2 * D0**2))


# =========================================
# 6. APPLY FILTER
# =========================================

filtered_ft = ft_shift * H


# =========================================
# 7. FILTERED MAGNITUDE SPECTRUM
# =========================================

filtered_spectrum = 20 * np.log(np.abs(filtered_ft) + 1)


# =========================================
# 8. INVERSE SHIFT
# =========================================

inverse_shift = np.fft.ifftshift(filtered_ft)


# =========================================
# 9. INVERSE FOURIER TRANSFORM
# =========================================

img_back = np.fft.ifft2(inverse_shift)

img_back = np.real(img_back)


# =========================================
# 10. NORMALIZE OUTPUT
# =========================================

output = cv2.normalize(img_back, None, 0, 255, cv2.NORM_MINMAX).astype(np.uint8)


# =========================================
# 11. PLOT EVERYTHING
# =========================================

plt.figure(figsize=(14, 8))


# Original image
plt.subplot(2, 3, 1)

plt.imshow(img, cmap="gray")

plt.title("Original Image")

plt.axis("off")


# Original Fourier spectrum
plt.subplot(2, 3, 2)

plt.imshow(magnitude_spectrum, cmap="gray")

plt.title("Magnitude Spectrum")

plt.axis("off")


# Gaussian LPF
plt.subplot(2, 3, 3)

plt.imshow(H, cmap="gray")

plt.title("Gaussian Low Pass Filter")

plt.axis("off")


# Filtered spectrum
plt.subplot(2, 3, 4)

plt.imshow(filtered_spectrum, cmap="gray")

plt.title("Filtered Spectrum")

plt.axis("off")


# Output image
plt.subplot(2, 3, 5)

plt.imshow(output, cmap="gray")

plt.title("GLPF Output")

plt.axis("off")


# Empty last plot
plt.subplot(2, 3, 6)

plt.axis("off")


plt.tight_layout()

plt.show()
