% 使用 3x3/5x5 (加权)模板的均值滤波器对受高斯噪声干扰的图像进行平滑处理
I=imread('jx.png');

I=rgb2gray(I);
I1=imnoise(I,'gaussian',0,0.01);                % 叠加均值 0 和方差 0.01 的高斯白噪声
I2=imfilter(I1,fspecial('average',3));          % 3x3 邻域均值滤波
I3=imfilter(I1,fspecial('average',5));          % 5x5 邻域均值滤波
I4=imfilter(I1,[1 2 1;2 4 2;1 2 1]/16);         % 3x3 加权邻域均值
I5=imfilter(I1,[1 4 7 4 1;4 16 26 16 4;7 26 41 26 7;4 16 26 16 4;1 4 7 4 1;]/273);  % 5x5 加权邻域均值      

subplot(3,2,1),imshow(I),title('原图');
subplot(3,2,2),imshow(I1),title('加高斯噪音后');
subplot(3,2,3),imshow(I2),title('3x3 邻域均值滤波');
subplot(3,2,4),imshow(I3),title('5x5 邻域均值滤波');
subplot(3,2,5),imshow(I4),title('3x3 加权邻域均值');
subplot(3,2,6),imshow(I5),title('5x5 加权邻域均值');