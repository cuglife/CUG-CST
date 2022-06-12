% 用 Roberts算子、 Prewitt算子、 Sobel算子、 LoG算子 及 Canny算子进行边缘检测的示例

I = rgb2gray(imread('edgec.jpg')); %读入一幅图像

B1 = edge(I, 'roberts',0.04);   % 用 Roberts算子，梯度阈值为0.04
B2 = edge(I, 'prewitt',0.04);   % 用 Prewitt算子，梯度阈值为0.04
B3 = edge(I, 'sobel',  0.04);   % 用 Sobel 算子，梯度阈值为 0.04
B4 = edge(I, 'log',    0.003);  % 用 LoG 算子，高斯滤波器宽度参数 Sigma=2
B5 = edge(I, 'canny',  0.04);   % 用 Canny算子， Sigma=2

subplot(231), imshow(I);  title('原始图像');     % 把一个图形窗口划分为2×3矩形显示区域，在左上区域 显示图像I
subplot(232), imshow(B1); title('Roberts算子');  % 在中上区域显示 Roberts算子 检测结果
subplot(233), imshow(B2); title('Prewitt算子');  % 在右上区域显示 Prewitt算子 检测结果
subplot(234), imshow(B3); title('Sobel算子');    % 在左下区域显示 Sobel算子   检测结果
subplot(235), imshow(B4); title( 'LOG算子');     % 在中下区域显示 LoG算子     检测结果
subplot(236), imshow(B5); title( 'Canny算子');   % 在右下区域显示 Canny算子   检测结果