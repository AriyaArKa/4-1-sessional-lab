import cv2
import numpy as np
import matplotlib.pyplot as plt

img = cv2.imread('labtest/Inputs/chest.png',cv2.IMREAD_GRAYSCALE)

height = img.shape[0]
width = img.shape[1]

output1 = np.empty_like(img)
output2 = np.empty_like(img)

if img is None:
    print("Image not found")
    exit()

def kernel(size, sigma_x, sigma_y):
    center = size // 2

    kernel = np.zeros((size, size), dtype=np.float32)

    for i in range(size):
        for j in range(size):
            x = j - center
            y = i - center
            #kernel[i, j] = formula
    # normalize
    kernel /= np.sum(kernel)

    return kernel



def convolution(img, kernel):
    height, width = img.shape
    kh, kw = kernel.shape

    ph = kh // 2
    pw = kw // 2

    padded_img = np.pad(img.astype(np.float32), ((ph, ph), (pw, pw)), mode="constant")

    output = np.zeros(img.shape, dtype=np.float32)

    for i in range(height):
        for j in range(width):
            total = 0
            for m in range(kh):
                for n in range(kw):
                    total += padded_img[i + m, j + n] * kernel[kh - 1 - m, kh - 1 - n]
            output[i, j] = total

    return output
        

plt.figure()
plt.subplot(1,3,1)
plt.imshow(img,cmap='gray')
plt.title('Original image') 
plt.axis('off')

plt.subplot(1,3,2)
plt.imshow(output1,cmap='gray')
plt.title('Gama image (γ=1.0)') 
plt.axis('off')

plt.subplot(1,3,3)
plt.imshow(output2,cmap='gray')
plt.title('Gama image (γ=2.5)') 
plt.axis('off')
plt.tight_layout()

plt.savefig('labtest/gama/gama_transformed.png')
plt.show()