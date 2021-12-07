import numpy as np
import cv2

img1 = cv2.imread(r'D:\cLion\project\image\mouse.jpg')
img2 = cv2.imread(r'D:\cLion\project\image\mouse3.jpg')


def sift_match():
    # 初始化SIFT检测器
    sift = cv2.xfeatures2d.SIFT_create()

    # 使用SIFT寻找关键点和描述符
    kp1, des1 = sift.detectAndCompute(img1, None)
    kp2, des2 = sift.detectAndCompute(img2, None)
    print(kp1[0])

    # 使用默认参数初始化BFMatcher
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(des1, des2, k=2)

    # 应用比率测试
    good = []
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            good.append([m])

    # cv2.drawMatchesKnn：绘制Knn匹配结果
    img3 = cv2.drawMatchesKnn(img1, kp1, img2, kp2, good, img1, flags=2)
    cv2.imshow('Display Window', img3)
    cv2.waitKey(0)


def r2d2_match():
    r2d2_1 = np.load(r'D:\2021-autumn\计算机视觉\r2d2-master\image\mouse.jpg.r2d2')
    r2d2_2 = np.load(r'D:\2021-autumn\计算机视觉\r2d2-master\image\mouse3.jpg.r2d2')
    print(r2d2_1['keypoints'][0])
    print(r2d2_2['keypoints'][0])

    r2d2_kp1 = []
    r2d2_kp2 = []
    for index in range(len(r2d2_1['keypoints'])):
        r2d2_kp1.append(cv2.KeyPoint(r2d2_1['keypoints'][index][0], r2d2_1['keypoints'][index][1], r2d2_1['keypoints'][index][2],  -1, r2d2_1['scores'][index]))

    for index in range(len(r2d2_2['keypoints'])):
        r2d2_kp2.append(cv2.KeyPoint(r2d2_2['keypoints'][index][0], r2d2_2['keypoints'][index][1], r2d2_2['keypoints'][index][2], -1, r2d2_2['scores'][index]))

    # 使用默认参数初始化BFMatcher
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(r2d2_1['descriptors'], r2d2_2['descriptors'], k=2)

    # 应用比率测试
    good = []
    for m, n in matches:
        if m.distance < 0.93 * n.distance:
            good.append([m])

    # cv2.drawMatchesKnn：绘制Knn匹配结果
    img3 = cv2.drawMatchesKnn(img1, r2d2_kp1, img2, r2d2_kp2, good, img1, flags=2)
    cv2.imshow('Display Window', img3)
    cv2.waitKey(0)

r2d2_match()









