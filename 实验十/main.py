import numpy as np
import cv2

img1 = cv2.imread(r'D:\cLion\project\exp10\end2.jpg')
img2 = cv2.imread(r'D:\cLion\project\exp10\end3.jpg')

SURF_detector = cv2.xfeatures2d.SURF_create(hessianThreshold = 3000)
keyPoints1, des1 = SURF_detector.detectAndCompute(img1, None)  # 查找关键点和描述符
keyPoints2, des2 = SURF_detector.detectAndCompute(img2, None)

FLANN_INDEX_KDTREE = 0  # 建立FLANN匹配器的参数
indexParams = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)  # 配置索引，密度树的数量为5
searchParams = dict(checks=50)  # 指定递归次数
# FlannBasedMatcher：是目前最快的特征匹配算法（最近邻搜索）
flann = cv2.FlannBasedMatcher(indexParams, searchParams)  # 建立匹配器
matches = flann.knnMatch(des1, des2, k=2)  # 得出匹配的关键点

good = []
# 提取优秀的特征点
for m, n in matches:
    if m.distance < 0.7 * n.distance:  # 如果第一个邻近距离比第二个邻近距离的0.7倍小，则保留
        good.append(m)

src_pts = np.array([keyPoints1[m.queryIdx].pt for m in good])  # 查询图像的特征描述子索引
dst_pts = np.array([keyPoints2[m.trainIdx].pt for m in good])  # 训练(模板)图像的特征描述子索引
H = cv2.findHomography(src_pts, dst_pts)  # 生成变换矩阵

h, w = img1.shape[:2]
h1, w1 = img2.shape[:2]
shft = np.array([[1.0, 0, w], [0, 1.0, 0], [0, 0, 1.0]])
M = np.dot(shft, H[0])  # 获取左边图像到右边图像的投影映射关系
dst_corners = cv2.warpPerspective(img1, M, (w * 2, h))  # 透视变换，新图像可容纳完整的两幅图
cv2.imshow('tiledImg1', dst_corners)  # 显示，第一幅图已在标准位置
dst_corners[0:h, w:w * 2] = img2  # 将第二幅图放在右侧
# cv2.imwrite('tiled.jpg',dst_corners)


cv2.imshow('tiledImg', dst_corners)
cv2.imshow('leftgray', img1)
cv2.imshow('rightgray', img2)
cv2.waitKey()
cv2.destroyAllWindows()
