

# 姓名

- 倪诗宇

# 学号

- 201900180065

# 实验日期

- 2021.11.13

# 实验题目

- 测试OpenCV中的SIFT, SURF, ORB等特征检测与匹配的方法。将检测到的特征点和匹配关系进行可视化输出，比较不同方法的效率、效果等

# 实验过程中遇到的问题和解决办法

- 问题一：

  - 问题：我们完全可以在原图的基础上一直变换scale来检测，为什么要进行降采样呢？
- 解决：
    - 为了使特征检测具有==尺寸不变性==，我们应该用不同大小的检测框来对图像进行特征检测
  
  - 降采样是==为了减少计算量==，在scale不变的情况下进行降采样，scale相当于增大了，也就是相当于对原图使用更大的scale进行检测
    - ==sigma的值也决定了滤波核的大小 ， 在原图用2*sigma滤波和降采样一半后用sigma滤波效果相当==，因为scale和图像的相对大小没有改变
    - 高斯金字塔有多个组，每个组又有多层。一个组中的多个层之间的尺度是不一样的（也就是使用的高斯参数 $\sigma$ 不同），相邻两层之间的尺度相差一个比例因子 k。如果每个组有 S 层，则 $k=2^{\frac{1}{S}}$
    
  - 小图的第一个是由大图的第一个
  
  - $o$ 是层数  $\sigma(o, s)=\sigma_{0} \cdot 2^{o+\frac{s}{S}}$
  
  - 一个组内相邻层的图像尺度关系
  
    - $\sigma_{s+1} = k*\sigma_s = 2^{\frac{1}{S}}*\sigma_s$
  
  - 相邻组之间的尺度关系
  
    - $\sigma_{o+1}=2\sigma_o$
  
    ![image-20211112113936006](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211112113936006.png)

- 问题二：

  - 问题：为什么要用检测响应值最大的点呢

- 解决：
  - ==最大的响应值意味着可能是特征点==
  - 我们构建如下图所示的响应函数（**scale自动选择函数**），可以看到，图像放大的倍数和检测框放大的倍数保持一致（圈出一样的图形来）。也就是说，若将一个图放大到原来的两倍，则其特征响应值最大时对应的scale size也会增大到原来的两倍。这就是==检测框和图像大小的相对不变性==**以下两张图都能看出这个情况**
  - ![image-20211113014256641](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211113014256641.png)
  - ![image-20211113012520595](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211113012520595.png)

  ​	

- 问题三：

  - 问题：为什么 LOG 可以用来进行特征检测

- 解决：

  - ==LOG中sigma的大小刚好对应着检测区域的大小==
  - 我们不断变换sigma进行特征检测就是不断变换检测框的scale，这样就可以在不同尺度的图像上找到相同的特征点
  - ![image-20211113011836665](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211113011836665.png)
  - ![image-20211113015205427](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211113015205427.png)
  - **进行斑点检测最好的scale是Laplacian响应最大值对应的scale**
  - 上图中正好包含该黑点的检测框就是最好的检测框，用该框得到的响应值最大，这个点也最有可能是特征点
  - ==LOG的sigma决定了检测区域的大小，我们的目的是将不同大小的检测框应用于图像，因此我们通过改变sigma来改变检测区域的大小。降采样可以减少计算量，同时图像减小了，相对来说检测框就变大了。因此，降采样相当于sigma增大，也就是我们用更大的框来进行检测了，同时还可以减少计算量==
  - 采用DOG来近似LOG同样是为了减少计算量

- 例子
  - 下图中越往上sigma越大，图像越模糊，也就是scale越大

![image-20211113015422359](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211113015422359.png)

# 结论分析与体会

# SIFT

- 使用 C++

- SIFT(Scale-Invariant Feature Transform)特征，即尺度不变特征变换，是一种计算机视觉的特征提取算法，用来侦测与描述图像中的局部性特征。

  实质上，它是**在不同的尺度空间上查找关键点(特征点)，并计算出关键点的方向**。SIFT所查找到的关键点是一些十分突出、不会因光照、仿射变换和噪音等因素而变化的点，如角点、边缘点、暗区的亮点及亮区的暗点等。

![image-20211114223013135](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211114223013135.png)

```c++
Total keyPoints srcImage: 100//左边图像检测特征点的个数
Total keyPoints srcImage2: 99//右边图像检测特征点的个数
SIFTtime(s): 0.151523//SIFT特征检测运行时间
total matches:100//总的匹配特征的个数
good matches:50//绘制的匹配特征的个数
```

# SURF

- 使用 Python

- SURF(Speeded Up Robust Features, 加速稳健特征) 是一种稳健的图像识别和描述算法。它是SIFT的高效变种，也是提取尺度不变特征，算法步骤与SIFT算法大致相同，但采用的方法不一样，要比SIFT算法更高效。SURF使用海森(Hesseian)矩阵的行列式值作特征点检测并用积分图加速运算；SURF 的描述子基于 2D 离散小波变换响应并且有效地利用了积分图。

  ![image-20211114224243422](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211114224243422.png)

  ```python
  Total KeyPoints srcImage1: 531 #左边图像检测的特征点个数
  Total KeyPoints srcImage2: 111 #右边图像检测的特征点个数
  SURF time(s): 0.400488 #SURF特征检测运行时间
  total match: 531 #总的匹配特征点的个数
  good match: 46 #绘制的匹配特征点的个数
  ```

# ORB

- 使用 C++
- ORB（Oriented FAST and Rotated BRIEF）该特征检测算法是在著名的FAST特征检测和BRIEF特征描述子的基础上提出来的，**其运行时间远远优于SIFT和SURF**，可应用于实时性特征检测。ORB特征检测具有尺度和旋转不变性，对于噪声及其透视变换也具有不变性，良好的性能是的利用ORB在进行特征描述时的应用场景十分广泛。ORB特征检测主要分为以下两个步骤:(1)方向FAST特征点检测(2)BRIEF特征描述。

![image-20211114222617416](C:\Users\nishiyu\AppData\Roaming\Typora\typora-user-images\image-20211114222617416.png)

```c++
Total keyPoints srcImage: 100 //左边图像检测的特征点个数
Total keyPoints srcImage2: 71 //右边图像检测的特征点个数
ORBtime(s): 0.404704//ORB检测运行时间
total matches:100//总的匹配特征点个数
good matches:50//绘制的匹配特征点个数
```

- 易知，若两种尺寸的图片特征点完全匹配正确，则各匹配线应当基本平行。因此，匹配线的交叉越多，则匹配效果越差
- 可以看到，SIFT速度最快，但是效果最差，**产生了很多错误的匹配（很疑惑，不是具有尺度不变性么）**
- 理论上SURF和ORB都比SIFT快，但是实验中发现，SURF和ORB运行时间差不多，但是都比SIFT慢
- SURF的检测效果最好
- ORB的检测效果次之
