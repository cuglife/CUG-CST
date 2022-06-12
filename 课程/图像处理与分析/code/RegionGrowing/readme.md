## 运行环境

* Python 3.7 或更高版本
* Opencv 4.2.0 或更高版本
* numpy 1.19.2 或更高版本
* matplotlib 3.2.1 或更高版本

## 使用方法

* main 函数中修改需要进行图像分割的图片 (./img 下已有示例图片)，运行即可
* 可选参数
  * 生长阈值（默认为前景的标准差），可通过 `_is_meet_threshold(self, x0, y0, x, y, threshold)`中 threshold  参数进行设置。
  * 种子点选取 (默认选取亮度值的前10%) ，可通过`_select_seed(self, img, brightness_range)`中参数 brightness_range 进行设置。