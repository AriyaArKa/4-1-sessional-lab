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

gama1 = 1
gama2 = 2.5
c = 1

for i in range(height):
    for j in range(width):
        r = img[i,j].astype(np.float32)/255.0
        s = c* r** gama1
        output1[i,j] = (255*s).astype(np.uint8)

        s = c* r** gama2
        output2[i,j] = (255*s).astype(np.uint8)

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