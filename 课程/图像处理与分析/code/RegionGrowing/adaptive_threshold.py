import cv2
import matplotlib.pyplot as plt

literacy_path = 'img/jx3.jpg'

img_literacy = cv2.imread(literacy_path, 0)

# threshold
ret, thresh = cv2.threshold(img_literacy, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
print(ret)  # 124
# adaptive threshold
thresh1 = cv2.adaptiveThreshold(img_literacy, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 0)
thresh2 = cv2.adaptiveThreshold(img_literacy, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
thresh3 = cv2.adaptiveThreshold(img_literacy, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 0)
thresh4 = cv2.adaptiveThreshold(img_literacy, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

# show image
plt.figure('adaptive threshold', figsize=(12, 8))
plt.subplot(231), plt.imshow(img_literacy, cmap='gray'), plt.title('original')
plt.subplot(234), plt.imshow(thresh, cmap='gray'), plt.title('otsu')
plt.subplot(232), plt.imshow(thresh1, cmap='gray'), plt.title('adaptive_mean_0')
plt.subplot(235), plt.imshow(thresh2, cmap='gray'), plt.title('adaptive_mean_2')
plt.subplot(233), plt.imshow(thresh3, cmap='gray'), plt.title('adaptive_gaussian_0')
plt.subplot(236), plt.imshow(thresh4, cmap='gray'), plt.title('adaptive_gaussian_2')
plt.show()
