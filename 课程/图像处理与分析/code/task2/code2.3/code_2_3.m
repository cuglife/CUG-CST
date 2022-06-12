%用对数变换法缩小灰度图像动态范围

I=imread('jx.png');                 %将图像读入变量I
I1=double(I);                       %双精度化
I2=im2double(I);                    %图像灰度值双精度化
I3=mat2gray(log(I1+1));             %对数变换并将图像的灰度范围变换为[0,1]

%对数变换缩小前后的图像和直方图
subplot(2,2,1),imshow(I),title('原图');
subplot(2,2,2),imhist(I2),title('原图直方图');
subplot(2,2,3),imshow(I3),title('对数变换缩小后的图像');
subplot(2,2,4),imhist(I3),title('对数变换缩小后直方图');