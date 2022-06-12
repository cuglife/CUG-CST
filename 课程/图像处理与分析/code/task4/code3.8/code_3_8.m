% 利用中值滤波法，对受椒盐噪声干扰的图像进行平滑处理
I=imread('jx.png');                     % 读入图像
I=rgb2gray(I);                          % 转换为灰度图
I1=imnoise(I,'SALT & PEPPER',0.04);     % 叠加椒盐噪音，噪音密度为0.04
I2=double(I1);                          % 双精度化
[M,N]=size(I2);                         % 获取图像的高和宽
I3=ones(size(I2));                      % 创建一个与原始图像尺寸相同，灰度值皆为1的图像

% 求取向量的中值
for i=2:M-1
    for j=2:N-1
        I3(i,j)=median([I2(i-1,j-1) I2(i-1,j) I2(i-1,j+1)...
            I2(i,j-1) I2(i,j) I2(i,j+1)...
            I2(i+1,j-1) I2(i+1,j) I2(i+1,j+1)]);
    end
end

% 第一行和最后一行的边界处理
for i=2:M-1
    I3(i,1)=I3(i,2);
    I3(i,N)=I3(i,N-1);
end

% 第一列和最后一列的边界处理
I3(1,:)=I3(2,:);
I3(M,:)=I3(M-1,:);

subplot(2,2,1),imshow(I),title('原图');
subplot(2,2,2),imshow(I1),title('加椒盐噪音后');
subplot(2,2,3),imshow(uint8(I3)),title('中值滤波法处理后');
subplot(2,2,4),imshow(I1-uint8(I3)),title('噪声冗余图像');

