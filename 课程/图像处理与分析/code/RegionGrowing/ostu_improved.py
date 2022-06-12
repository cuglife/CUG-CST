from _stack import Stack
import copy
import cv2
import numpy as np
import matplotlib.pyplot as plt


# 仅适用于二值图像
class Region_Growing_Improved(object):
    def __init__(self, im=None, im_path=None):
        self.stack = Stack()

    def im_read(self, img_path):
        self.im_path = img_path
        self.im = cv2.imread(self.im_path)

    def output(self, threshold=20):
        im_out = np.zeros_like(self.im)
        im_out = np.stack((im_out, im_out, im_out), axis=2)
        # print(self.label_area)
        for label_idx in range(1, self.max_label + 1):
            if self.label_area[label_idx] > threshold:  # areas smaller than threshold will be treated as background!!!
                rand_color = np.random.randint(0, 255), np.random.randint(0, 255), np.random.randint(0, 255)
                im_out[np.where(self.im_label == label_idx)] = rand_color
        cv2.waitKey(0)
        return im_out

    def std_dev(self, img):
        arr = []
        h, w = img.shape
        for i in range(h):
            for j in range(w):
                if img[i][j] != 0:
                    arr.append(img[i][j])
        return np.std(arr)

    def run(self, im):
        self.im = im

        self.im_size = self.im.shape
        self.im_height, self.im_width = self.im_size
        self.im_label = np.full_like(self.im, 0).astype(np.int)

        self.max_label = 0
        self.label_area = {}

        seed_group = self._select_seed(im, 0.10)
        # print(len(seed_group))
        # print(seed_group)
        threshold = self.std_dev(im)
        print(threshold)
        for x0, y0 in seed_group:
            # select point which is not labeled and is not background
            if self.im_label[x0, y0] == 0 and self.im[x0, y0] != 0:
                self.max_label += 1
                self.im_label[x0, y0] = self.max_label
                self.stack.push((x0, y0))
                tmp_area = 1  # tmp_area is the area of grown region in this round
                while not self.stack.is_empty():
                    x, y = self.stack.pop()
                    tmp_area += self.grow(x, y, threshold)

                self.label_area[self.max_label] = tmp_area

        return self.output(threshold=20)

    def _get_neighbour(self, x0, y0):
        neighbour = []
        for i in (-1, 0, 1):
            for j in (-1, 0, 1):
                if (i, j) == (0, 0):
                    continue
                x = x0 + i
                y = y0 + j
                if self._in_region(x, y):
                    neighbour.append((x, y))
        return neighbour

    def _in_region(self, x, y):
        return 0 <= x < self.im_height and 0 <= y < self.im_width

    def grow(self, x0, y0, threshold):
        tmp = 0
        current_label = self.im_label[x0, y0]
        for x, y in self._get_neighbour(x0, y0):
            if self._is_meet_threshold(x0, y0, x, y, threshold):  # threshold
                self.im_label[x, y] = current_label
                self.stack.push((x, y))
                tmp += 1
        return tmp

    def _is_meet_threshold(self, x0, y0, x, y, threshold):
        return self.im_label[x, y] == 0 and self.im[x, y] != 0 and (
                abs(int(self.im[x, y]) - int(self.im[x0, y0])) < threshold)

    def _select_seed(self, img, brightness_range):
        seed_gray = self._get_top_gray(img, brightness_range)
        print("seed_gray")
        print(seed_gray)
        print(img[0, 0])
        seed_group = []
        for x0 in range(self.im_height):
            for y0 in range(self.im_width):
                if img[x0, y0] > seed_gray:
                    seed_group.append((x0, y0))
        return seed_group

    def _get_top_gray(self, img, hi_percentage):
        # we want we the hi_percentage brightest pixels
        # * histogram
        hist = cv2.calcHist([img], [0], None, [256], [1, 256]).flatten()

        # * find brightness threshold
        # here: highest thresh for including at least hi_percentage image pixels,
        #       maybe you want to modify it for lowest threshold with for including
        #       at most hi_percentage pixels
        total_count = img.shape[0] * img.shape[1]  # height * width
        target_count = hi_percentage * total_count  # bright pixels we look for
        summed = 0
        for i in range(255, 0, -1):
            summed += int(hist[i])
            if target_count <= summed:
                hi_thresh = i
                break
        else:
            hi_thresh = 0
        # * apply threshold & display result for demonstration purposes:
        return hi_thresh


def main():
    # origin image read
    ori_img = cv2.imread('img/jx3.jpg', 0)

    # 预处理
    # 1. 图像去噪：median filter processing
    img_median = cv2.medianBlur(ori_img, 3)
    img_median = cv2.medianBlur(img_median, 3)
    img_median = cv2.medianBlur(img_median, 3)
    img_median = cv2.medianBlur(img_median, 3)

    # 2. 图像增强
    # 创建CLAHE对象
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    # 限制对比度的自适应阈值均衡化
    dst = clahe.apply(img_median)

    # 3. 背景去除
    # otsu threshold to get a binary image
    ret, bin_ostu = cv2.threshold(dst, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    print(ret)  # threshold

    ostu_img = copy.deepcopy(dst)
    h, w = ostu_img.shape
    for i in range(h):
        for j in range(w):
            if ostu_img[i][j] < ret:
                ostu_img[i][j] = 0

    grow_seed = Region_Growing_Improved()
    result = grow_seed.run(ostu_img.astype(np.uint8))

    # 后处理
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (2, 2))
    result = cv2.morphologyEx(result, cv2.MORPH_CLOSE, kernel)

    # show image
    plt.figure('region growing', figsize=(15, 8))
    plt.subplot(231), plt.imshow(ori_img, cmap='gray'), plt.title('original'), plt.axis('off')
    plt.subplot(232), plt.imshow(img_median, cmap='gray'), plt.title('median'), plt.axis('off')
    plt.subplot(233), plt.imshow(dst, cmap='gray'), plt.title('CLAHE'), plt.axis('off')
    plt.subplot(234), plt.imshow(bin_ostu, cmap='gray'), plt.title('bin_ostu'), plt.axis('off')
    plt.subplot(235), plt.imshow(ostu_img, cmap='gray'), plt.title('ostu_img'), plt.axis('off')
    plt.subplot(236), plt.imshow(result, cmap='gray'), plt.title('result'), plt.axis('off')
    plt.show()


if __name__ == '__main__':
    main()
