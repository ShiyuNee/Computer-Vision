#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include<math.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(){
    Mat source_image, bg_image;

    source_image = imread("D:\\cLion\\project\\02.jpg");
    bg_image = imread("D:\\cLion\\project\\02_bg.jpg");
    if (!source_image.data || !bg_image.data){
        cout << "fail load！" << endl;
        return -1;
    }

    if (source_image.rows != bg_image.rows || source_image.cols != bg_image.cols)
        cout << "images not match！" << endl;

    Mat transformed_image = Mat::zeros(source_image.size(), source_image.type());

    namedWindow("Display Window", WINDOW_AUTOSIZE);

    double sum = 0.0;

    for (int y = 0; y < source_image.rows; y++)
        for (int x = 0; x < source_image.cols; x++){
            sum = 0.0;
            for (int c = 0; c < 3; c++) sum += pow((source_image.at<Vec3b>(y, x)[c] - bg_image.at<Vec3b>(y, x)[c]), 2);
            sum = sqrt(sum);
            if (sum >= 90){
                for (int c = 0; c < 3; c++)
                    transformed_image.at<Vec3b>(y, x)[c] = saturate_cast <uchar>(255);
            }
            else {
                for (int c = 0; c < 3; c++)
                    transformed_image.at<Vec3b>(y, x)[c] = saturate_cast <uchar>(0);
            }
        }
    imshow("Display Window", transformed_image);
    waitKey(0);
    return 0;
}
