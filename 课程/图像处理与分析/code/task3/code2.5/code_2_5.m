% 直方图均衡化
I=imread('jx.png');  %读入灰度图像
[I1,T] = histeq(I);  %直方图均衡化

%显示增强前后的图像和直方图
subplot(2,3,1),imshow(I),title('原图');
subplot(2,3,2),imhist(I),title('原图直方图');
subplot(2,3,[3 6]),plot((0:255)/255,T),title('变换曲线');
subplot(2,3,4),imshow(I1),title('均衡化后的图像');
subplot(2,3,5),imhist(I1),title('均衡化后的直方图');