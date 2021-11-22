#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
using namespace std;
using namespace cv;
Mat source_image;
Mat template_image;
Mat target_image;

int main()
{
    source_image = imread("D:\\cLion\\project\\02.jpg");
    template_image = imread("D:\\cLion\\project\\02_temp.jpg");
//调整大小时使用
//    for(int i = 0 ; i < template_image.rows ; i++)
//        for(int j = 0 ; j < template_image.cols ; j++)
//            for(int c = 0 ; c < 3 ; c++)
//                template_image.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(template_image.at<Vec3b>(i,j)[c] + 80);

    matchTemplate(source_image, template_image, target_image, TM_SQDIFF);


    double minVal, maxVal; //存储匹配的最大值和最小值
    Point minLoc, maxLoc; //存储匹配最大值的位置和最小值的位置0
    //寻找最佳匹配位置
    minMaxLoc(target_image, &minVal, &maxVal, &minLoc, &maxLoc);
    //Point中使用max或min根据具体方法确定
    circle(source_image, Point(minLoc.x + template_image.cols / 2, minLoc.y + template_image.rows / 2), 10, Scalar(0, 0, 255), 2, 8, 0);

    normalize(target_image, target_image, 0, 1, NORM_MINMAX);
    imshow("Source Window" , source_image);
    imshow("Template Window" , template_image);
    imshow("Target Window" , target_image);
    waitKey(0);
}


