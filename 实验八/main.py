import cv2
import time
srcImage = cv2.imread(r"D:\py\Pycharm-Professional\pythonProject\image\3.jpg")
srcImage2 = cv2.imread(r"D:\py\Pycharm-Professional\pythonProject\image\3_3_3.jpg")

a = time.time()
SURF_detector = cv2.xfeatures2d.SURF_create()
keyPoints1, disImage1 = SURF_detector.detectAndCompute(srcImage, None)
keyPoints2, disImage2 = SURF_detector.detectAndCompute(srcImage2, None)
b = time.time()
print("Total KeyPoints srcImage1: %d" % len(keyPoints1))
print("Total KeyPoints srcImage2: %d" % len(keyPoints2))
print("SURF time(s): %f" % (b-a))


bf = cv2.BFMatcher()
matches = bf.knnMatch(disImage1, disImage2, k=2)
print("total match: %d" % len(matches))
# Apply ratio test
good = [[m] for m, n in matches if m.distance < 0.3 * n.distance]
print("good match: %d" % len(good))

# cv2.drawMatchesKnn expects list of lists as matches.
img3 = cv2.drawMatchesKnn(srcImage, keyPoints1, srcImage2, keyPoints2, good, None, flags=2)
cv2.imshow("Display Window", img3)
cv2.waitKey(0)
