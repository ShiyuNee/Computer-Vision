#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
Mat srcImage;
Mat srcImage2;
Ptr<SIFT> detector_SIFT;
Ptr<ORB>  detector_ORB;
Ptr<FlannBasedMatcher> matcher_SIFT = FlannBasedMatcher::create();
FlannBasedMatcher matcher_ORB(new flann::LshIndexParams(20, 10, 2));

template<typename T>
void process(T detector , string choose_type){
    vector<KeyPoint> keyPoints;
    vector<KeyPoint> keyPoints2; //用于存储特征点

    auto t = (double)getTickCount();
    detector->detect(srcImage, keyPoints, Mat());
    detector->detect(srcImage2, keyPoints2, Mat());
    printf("Total keyPoints srcImage: %d\n", keyPoints.size());
    printf("Total keyPoints srcImage2: %d\n", keyPoints2.size());

    Mat descriptors;
    Mat descriptors2;
    detector->compute(srcImage , keyPoints , descriptors);
    detector->compute(srcImage2 , keyPoints2 , descriptors2);
    t = ((double)getTickCount() - t)/getTickFrequency();

    cout << choose_type <<"time(s): " << t << endl;

    vector<DMatch> matches;

    if(choose_type == "SIFT")
        matcher_SIFT->match(descriptors, descriptors2, matches);
    else
        matcher_ORB.match(descriptors, descriptors2, matches);

    // 发现高质量匹配
    std::cout << "total matches:" << matches.size() <<std::endl;
    int numOfGood = matches.size() * 0.5;//挑选匹配得比较好的点
    std::sort(matches.begin(), matches.end());
    matches.erase(matches.begin() + numOfGood, matches.end());
    std::cout << "good matches:" << matches.size() << std::endl;

    Mat dstImage;
    drawMatches(srcImage, keyPoints, srcImage2, keyPoints2, matches, dstImage);
    imshow("Display Window", dstImage);
    waitKey(0);
}

void detecte_match(string choose_type){
    srcImage = imread("D:\\cLion\\project\\image\\3.jpg");
    srcImage2 = imread("D:\\cLion\\project\\image\\3_3.jpg");

    int numFeatures = 100;
    if(choose_type == "SIFT"){
        detector_SIFT = SIFT::create(numFeatures);
        process(detector_SIFT , choose_type);
    }

    else{
        detector_ORB = ORB::create(numFeatures);
        process(detector_ORB , choose_type);
    }
}

int main()
{
    detecte_match("SIFT");
}


