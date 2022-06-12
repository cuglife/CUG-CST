% 创建一个含噪示例图像 
I =checkerboard(8);                % 读入原始的棋盘图像
PSF = fspecial('gaussian',7,10);   % 设定尺寸7、标准偏差10的高斯噪声
V=.0001;                           % 设定方差值
BlurredNoisy = imnoise(imfilter(I,PSF),'gaussian',0,V);    % 生成高斯噪声污染的退化图像
 
% 创建一个权重数组以指定处理中包含哪些像素
WT = zeros(size(I));                             % 创建一个与原始图像大小相同的数值为0的图像          
WT(5:end-4,5:end-4) = 1;
INITPSF = ones(size(PSF));                       % 创建一个与PSF大小相同的数值为1的PSF初始估计

% 对污染图像进行盲去卷积复原
% 参数依次为 模糊图像, PSF初始估计值, 迭代次数, 阻尼的阈值, 每个像素的权重, 噪声
[J P] = deconvblind(BlurredNoisy,INITPSF,20,10*sqrt(V),WT); 
subplot(231);imshow(I),title('原始图像');
subplot(232);imshow(BlurredNoisy),title('退化图像');
subplot(233);imshow(PSF,[]),title('真实的PSF'); 
subplot(234);imshow(J),title('盲去卷积复原图像');
subplot(236);imshow(P,[]),title('复原的PSF');