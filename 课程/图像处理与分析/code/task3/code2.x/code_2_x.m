% 图像的加减乘除
I=imread('jx.png');  %读入灰度图像

% 对图像的灰度值进行加减乘操作
Add_I = imadd(I,80);
Sub_I = imsubtract(I,80);
Mul_I = immultiply(I,3);
Mul_I2 = immultiply(I,0.6);

% 显示原图和运算后的图像
subplot(2,3,1),imshow(I),title('原图');
subplot(2,3,2),imshow(Add_I),title('+80 图像');
subplot(2,3,5),imshow(Sub_I),title('-80 图像');
subplot(2,3,3),imshow(Mul_I),title('x3 图像');
subplot(2,3,6),imshow(Mul_I2),title('x0.6 图像');