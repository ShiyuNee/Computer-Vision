#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include<vector>
using namespace std;
using namespace cv;

int main()
{
    vector<Mat> images;
    Mat img1 = imread("D:\\cLion\\project\\exp10\\end1.jpg");//拼接图像1
    Mat img2 = imread("D:\\cLion\\project\\exp10\\end2.jpg");//拼接图象2
    //Mat img3 = imread("D:\\cLion\\project\\exp10\\computer2.jpg");
    images.push_back(img1);
    images.push_back(img2);
    //images.push_back(img3);

    Mat res;
    Stitcher::Mode mode = Stitcher::PANORAMA;
    Ptr<Stitcher> stitcher = Stitcher::create(mode);//建立拼接器
    Stitcher::Status  status = stitcher->stitch(images, res);//进行拼接
    imshow("Display Window" , res);
    waitKey(0);

    return 0;
}


