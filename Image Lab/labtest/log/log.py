import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.imread('labtest/Inputs/chest.png',cv2.IMREAD_GRAYSCALE)

if img is None:
    print("Image not found")
    exit()

height = img.shape[0]
width = img.shape[1]

output = np.empty_like(img)
c=1

for i in range(height):
    for j in range(width):
        r = img[i,j].astype(np.float32)
        s = c * np.log(1 + r)
        output[i,j] = s.astype(np.uint8)

plt.figure()
plt.subplot(1,2,1)
plt.imshow(img,cmap='gray')
plt.title('Original image')
plt.axis('off')

plt.subplot(1,2,2)
plt.imshow(output,cmap='gray')
plt.title('log transformed image')
plt.axis('off')

plt.tight_layout()

plt.savefig("labtest/log/log_transformed.png")

plt.show()