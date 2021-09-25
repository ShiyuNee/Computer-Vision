#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include<math.h>
#include <iostream>
using namespace std;
using namespace cv;

int contrast_value;
int bright_value;
Mat source_image, transformed_image;

static void ContrastAndBright(int pos , void* userdata)
{
    for (int y = 0; y < source_image.rows; y++)
    {
        for (int x = 0; x < source_image.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                //0<=contrast_value<=200
                double t = ((source_image.at<Vec3b>(y, x)[c] - 127) / 255.00) * contrast_value * 0.1;
                transformed_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(source_image.at<Vec3b>(y, x)[c] * ((1.00 / (1.00 + exp(-t))) + 0.3) + bright_value - 100);
                //int t = source_image.at<Vec3b>(y, x)[c];
                //transformed_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(t * ((contrast_value*0.1 / (1.00 + exp(-t))) + 1));
            }
        }
    }
    imshow("Display Window", source_image);
    imshow("Transformed Window", transformed_image);
}
int main()
{
    source_image = imread("D:\\cLion\\project\\01.jpg");

    if (!source_image.data)
    {
        cout << "图片读取失败！" << endl;
    }
    transformed_image = Mat::zeros(source_image.size(), source_image.type());

    contrast_value = 20;
    bright_value = 100;

    namedWindow("Transformed Window", WINDOW_AUTOSIZE);

    createTrackbar("contrast", "Transformed Window", &contrast_value, 200, ContrastAndBright , 0);
    createTrackbar("bright", "Transformed Window", &bright_value, 200, ContrastAndBright , 0);

    waitKey(0);
    return 0;

}

