%对图像进行量化处理
I=imread('bg.png');

I1=rgb2gray(I);
subplot(2,3,1),imshow(I1),title('8比特（256级）');

I2=double(I1); %提示赋值给I2的值可能无用，俺也是这么觉得的，书上的代码应该有点问题
I2=round(I1/4)*4;
subplot(2,3,2),imshow(uint8(I2)),title('6比特(64级)');

I3=round(I1/16)*16;
subplot(2,3,3),imshow(uint8(I3)),title('6比特(64级)');

I4=round(I1/32)*32;
subplot(2,3,4),imshow(uint8(I4)),title('4比特(16级)');

I5=round(I1/64)*64;
subplot(2,3,5),imshow(uint8(I5)),title('2比特(4级)');

I6=round(I1/128)*128;
subplot(2,3,6),imshow(uint8(I6)),title('1比特(2级)');