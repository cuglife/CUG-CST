%用线性变换法增强灰度图像

I=imread('jx.png');                     %将图像读入变量I
I1=im2double(I);                        %图像灰度值双精度化
I2=imadjust(I1,stretchlim(I1),[0 1]);   %通过stretchlim自适应找到分割阈值并将灰度范围拉伸到[0 1]

%显示增强前后的图像和直方图
subplot(2,2,1),imshow(I),title('原图');
subplot(2,2,2),imhist(I1),title('原图直方图');
subplot(2,2,3),imshow(I2),title('增强后的图像');
subplot(2,2,4),imhist(I2),title('增强后直方图');