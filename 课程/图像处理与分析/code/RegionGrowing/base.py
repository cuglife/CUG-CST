from _stack import Stack
import cv2
import numpy as np
import matplotlib.pyplot as plt


# 仅适用于二值图像
class Region_Growing_Base(object):
    def __init__(self, im=None, im_path=None):
        self.stack = Stack()

        if im is not None:
            self.im = im
        elif im_path is not None:
            self.im_read(im_path)
        else:
            raise ValueError("No Input image!")

        self.im_size = self.im.shape
        self.im_height, self.im_width = self.im_size
        self.im_label = np.full_like(self.im, 0).astype(np.int)

        self.max_label = 0
        self.label_area = {}

    def im_read(self, img_path):
        self.im_path = img_path
        self.im = cv2.imread(self.im_path)

    def output(self, threshold=20):
        im_out = np.zeros_like(self.im)
        im_out = np.stack((im_out, im_out, im_out), axis=2)
        for label_idx in range(1, self.max_label + 1):
            if self.label_area[label_idx] > threshold:  # areas smaller than threshold will be treated as background!!!
                rand_color = np.random.randint(0, 255), np.random.randint(0, 255), np.random.randint(0, 255)
                im_out[np.where(self.im_label == label_idx)] = rand_color
        cv2.waitKey(0)
        return im_out

    def run(self):
        for x0 in range(self.im_height):
            for y0 in range(self.im_width):
                if self.im_label[x0, y0] == 0 and self.im[x0, y0] != 0:  # ignoring the background
                    self.max_label += 1
                    self.im_label[x0, y0] = self.max_label
                    self.stack.push((x0, y0))
                    tmp_area = 1  # tmp_area is the area of growned region in this round
                    while not self.stack.is_empty():
                        x, y = self.stack.pop()
                        tmp_area += self.grow(x, y)

                    self.label_area[self.max_label] = tmp_area

        return self.output(threshold=20)

    def grow(self, x0, y0):
        tmp = 0
        current_label = self.im_label[x0, y0]
        for x, y in self._get_neighbour(x0, y0):
            if self.im_label[x, y] == 0 and self.im[x, y] == self.im[x0, y0]:  # threshold
                self.im_label[x, y] = current_label
                self.stack.push((x, y))
                tmp += 1
        return tmp

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
        return True if 0 <= x < self.im_height and 0 <= y < self.im_width else False


def main():
    ori_im = cv2.imread('img/jx3.jpg', 0)
    aver_gray, u1, u2, u3 = cv2.mean(ori_im)
    bin_im = ((ori_im > aver_gray) * 255).astype(np.uint8)
    grow_seed = Region_Growing_Base(im=bin_im)
    result = grow_seed.run()

    cv2.imwrite('base_jx3.png', result)

    print(result)
    plt.figure('region growing', figsize=(12, 8))
    plt.subplot(131), plt.imshow(ori_im, cmap='gray'), plt.title('original'), plt.axis('off')
    plt.subplot(132), plt.imshow(bin_im, cmap='gray'), plt.title('binary image'), plt.axis('off')
    plt.subplot(133), plt.imshow(result, cmap='gray'), plt.title('result'), plt.axis('off')
    plt.show()


if __name__ == '__main__':
    main()
