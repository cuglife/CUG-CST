%基本和书上代码一致，注释略
%对图像进行采样处理
I=imread('bg.png');

I1=rgb2gray(I);
imshow(I1),title('800X800');

I2=I1(1:2:end,1:2:end);
figure,imshow(I2),title('400X400');

I3=I1(1:4:end,1:4:end);
figure,imshow(I3),title('200X200');

I4=I1(1:8:end,1:8:end);
figure,imshow(I4),title('100X100');

I5=I1(1:16:end,1:16:end);
figure,imshow(I5),title('50X50');

figure,subplot(2,3,1),imshow(I),title('color image');
       subplot(2,3,2),imshow(I1),title('800X800');
       subplot(2,3,3),imshow(I2),title('400X400');
       subplot(2,3,4),imshow(I3),title('200X200');
       subplot(2,3,5),imshow(I4),title('100X100');
       subplot(2,3,6),imshow(I5),title('50X50');
