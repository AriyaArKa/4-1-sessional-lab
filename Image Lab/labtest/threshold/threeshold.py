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

T1 = 150

for i in range(height):
    for j in range(width):
        if img[i,j] >= T1:
            output1[i,j] = 255
        else:
            output1[i,j] = 0

        if img[i,j] < T1:
            output2[i,j] = 255
        else:
            output2[i,j] = 0
        

        

plt.figure()
plt.subplot(1,3,1)
plt.imshow(img,cmap='gray')
plt.title('Original image') 
plt.axis('off')

plt.subplot(1,3,2)
plt.imshow(output1,cmap='gray')
plt.title('Threshold image (T=150) r>=150') 
plt.axis('off')

plt.subplot(1,3,3)
plt.imshow(output2,cmap='gray')
plt.title('Threshold image (T=150) r<150') 
plt.axis('off')
plt.tight_layout()

plt.savefig('labtest/threshold/threshold_transformed.png')
plt.show()