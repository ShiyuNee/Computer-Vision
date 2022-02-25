#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include"opencv2/video/tracking.hpp"
#include <iostream>
using namespace std;
using namespace cv;
Mat image;
Mat image2;
Mat imageCopy; //绘制矩形框时用来拷贝原图的图像
Mat targetImage;//截取的目标图像
int finish = 0;
Point startPoint; //矩形框起点
Point endPoint; //矩形框终点
vector<KeyPoint> keyPoints;
Mat target_descriptors;
Mat last_image;
bool leftButtonDown=false; //左键单击后视频暂停播放的标志位
vector<KeyPoint> keyPoints2; //用于存储特征点
Mat descriptors2; //描述符
vector<Point2f> points2; //注意是point2f类
Ptr<FlannBasedMatcher> matcher_SIFT = FlannBasedMatcher::create();
vector<DMatch> matches; //匹配，一直不能变

void process_init(){
    int numFeatures = 500;
    Ptr<SIFT> detector_SIFT;
    detector_SIFT = SIFT::create(numFeatures);
    detector_SIFT->detect(targetImage, keyPoints, Mat());
    detector_SIFT->compute(targetImage , keyPoints , target_descriptors);
}

void Sift_match(bool is_first_time){
    int numFeatures = 1000;
    Ptr<SIFT> detector_SIFT;
    detector_SIFT = SIFT::create(numFeatures);

    if(is_first_time){
        detector_SIFT->detect(image2, keyPoints2, Mat());
        detector_SIFT->compute(image2 , keyPoints2 , descriptors2);
        points2.clear();
        for(auto & t : keyPoints2)
            points2.emplace_back(t.pt);
        //匹配之后不要变
        matcher_SIFT->match(target_descriptors, descriptors2, matches);
        int numOfGood = matches.size() * 0.2;
        std::sort(matches.begin(), matches.end());
        matches.erase(matches.begin() + numOfGood, matches.end());
    }
    else{
        vector<uchar> status;
        vector<float> err;
        vector<Point2f> points3;
        //prevPts 和 nextPts都需要是 point2f类型
        calcOpticalFlowPyrLK(last_image, image2, points2, points3, status, err);
        for(int j = 0 ; j < points3.size() ; j++)
            points2[j] = points3[j];

        for(int i = 0 ; i < points2.size() ; i++)
            keyPoints2[i].pt = points2[i];
    }
    last_image = image2.clone(); // clone 一定要加，否则两个 Mat 指向同一块地址，一变都变

    Mat dstImage;
    drawMatches(targetImage, keyPoints, image2, keyPoints2, matches, dstImage, Scalar(0,0,255), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("Match Window", dstImage);
}

void onMouse(int event, int x, int y, int flags, void *ustc)
{
    if (event == EVENT_LBUTTONDOWN){
        leftButtonDown = true; //标志位
        startPoint = Point(x, y);  //设置左键按下点的矩形起点
        endPoint = startPoint;
    }
    if (event == EVENT_MOUSEMOVE && leftButtonDown){
        imageCopy = image.clone();
        endPoint = Point(x, y);

        if (startPoint != endPoint)
            //在复制的图像上绘制矩形
            rectangle(imageCopy, startPoint, endPoint, Scalar(255, 0, 0), 2);

        imshow("Display Window", imageCopy);
    }
    if (event == EVENT_LBUTTONUP){// 左键松开时，置finish = 1，删除原图显示
        leftButtonDown = false;
        targetImage = image(Rect(startPoint, endPoint)); //获取目标图像targetImage
        finish = 1;
    }
}

//启动函数加载视频，并交互框出目标图像
void getTarget() {
    VideoCapture video;
    video.open(0);
    if (!video.isOpened()){
        cout << "video not open.error!" << endl;
        return;
    }
    namedWindow("Display Window");
    setMouseCallback("Display Window", onMouse);
    while (true){
        if (!leftButtonDown)
            video >> image;
        if (!image.data || waitKey(10) == 27)
            break;
        imshow("Display Window", image);
        if (finish == 1) {
            destroyWindow("Display Window");
            break;
        }
    }
    video.release();
}

int main()
{
    bool is_first_time = true;
    getTarget();
    process_init();
    VideoCapture video;
    video.open(0);
    if (!video.isOpened())
        cout << "video not open.error!" << endl;

    int num = 0;
    while (true){
        video >> image2;
        if (!image2.data || waitKey(10) == 27)
            break;
        num++;
        Sift_match(is_first_time);
        if(num % 100 == 0)
            is_first_time = true;
        else
        is_first_time = false;
    }
}
