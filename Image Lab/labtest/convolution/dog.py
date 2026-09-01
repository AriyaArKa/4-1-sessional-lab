import cv2
import matplotlib.pyplot as plt
import numpy as np

img = cv2.imread('labtest/Inputs/retina_2.png', cv2.IMREAD_GRAYSCALE)

if img is None:
    print("Image not found")
    exit()


def dog_x_kernel(size,sigma_x,sigma_y):
    center = size//2

    kernel = np.zeros((size,size),dtype=np.float32)

    for i in range(size):
        for j in range(size):
            x = j-center
            y = i-center

            g = np.exp(-
                       ((x**2)/2*sigma_x**2)
                       +
                       ((y**2)/2*sigma_y**2)
                       )

            kernel[i,j] = -(x/sigma_x**2)*g

        

    return kernel


def convolution(img,kernel):
    height,width = img.shape
    kh,kw = kernel.shape

    ph = kh//2
    pw = kw//2

    padded_img = np.pad(img.astype(np.float32),((ph,ph),(pw,pw)),mode="constant")

    output = np.zeros(img.shape,dtype=np.float32)

    for i in range(height):
        for j in range(width):
            total = 0
            for m in range(kh):
                for n in range(kw):
                    total += padded_img[i+m,j+n]*kernel[kh-1-m,kh-1-n]
            output[i,j] = total

    return output



dog_x_kernel = dog_x_kernel(3,1,1)
output = convolution(img,dog_x_kernel)

output_show = np.abs(output)

output_show = cv2.normalize(
    output_show,
    None,
    0,
    255,
    cv2.NORM_MINMAX
).astype(np.uint8)

plt.figure()
plt.subplot(1,4,1)
plt.imshow(img,cmap='gray')
plt.title('Original image')
plt.axis('off')


plt.subplot(1,4,2)
plt.imshow(dog_x_kernel,cmap='gray')
plt.title('DOG X Kernel')
plt.axis('off')

plt.subplot(1,4,3)
plt.imshow(output,cmap='gray')
plt.title('Output beofre normalize')
plt.axis('off')

plt.subplot(1,4,4)
plt.imshow(output_show,cmap='gray')
plt.title('Output')
plt.axis('off')

plt.show()