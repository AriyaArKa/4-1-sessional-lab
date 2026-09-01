import cv2
import numpy as np
import matplotlib.pyplot as plt


img = cv2.imread('labtest/boat.jpg',cv2.IMREAD_GRAYSCALE)

if img is None:
    print("Image not found")
    exit()

height,width = img.shape

print("Height:",height)
print("Width:",width)

plt.figure()
plt.subplot(2,2,1)
plt.imshow(img,cmap='gray')
plt.title('Image1')
plt.axis('off')

plt.subplot(2,2,2)
plt.imshow(img,cmap='gray')
plt.title('Image2')
plt.axis('off')

plt.subplot(2,2,3)
plt.imshow(img,cmap='gray')
plt.title('Image3')
plt.axis('off')

plt.subplot(2,2,4)
plt.imshow(img,cmap='gray')
plt.title('Image4')
plt.axis('off')


plt.tight_layout()

plt.savefig("all_images.png")


plt.show()


cv2.imwrite('image3.jpg', img)