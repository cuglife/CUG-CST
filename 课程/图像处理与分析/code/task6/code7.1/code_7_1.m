% 利用腐蚀、膨胀、开启、闭合及形态学重建对受噪声污染的图像进行平滑处理的示例

I = imread('shape.jpg');
I = im2bw(I, graythresh(I)); % 读入一幅灰度图像并二值化
se = strel('disk',2);             % 设定结构元素为一个半径为1的圆盘
I1 = imerode(I,se);               % 对原始二值图像进行腐蚀
I2 = imdilate(I,se);              % 对原始二值图像进行膨胀
I3 = imopen(I,se);                % 对原始二值图像进行开启
I4 = imclose(I,se);               % 对原始二值图像进行闭合
I5 = imclose(I3,se);              % 对开启后的结果图像再进行闭合
I6 = imopen(I4,se);               % 对闭合后的结果图像再进行开启
I7 = imerode(I,se);               % 对二值图像进行腐蚀
I8 = imreconstruct(I7,I);         % 形态学重建
se = strel('disk',8);             % 设定结构元素为半径为3的圆盘
I9 = imclose(imopen(I,se),se);    % 对原始图像先开启再闭合

subplot(331), imshow(I),  title('原始二值图像');
subplot(332), imshow(I1), title('腐蚀后图像');
subplot(333), imshow(I2), title('膨胀后图像');
subplot(334), imshow(I3), title('开启后图像');
subplot(335), imshow(I4), title('闭合后图像');
subplot(336), imshow(I5), title('先开启后闭合的图像');
subplot(337), imshow(I6), title('先闭合后开启的图像');
subplot(338), imshow(I8), title('先腐蚀后重建的图像');
subplot(339), imshow(I9), title('先开启后闭合的图像（加大圆盘半径）');
