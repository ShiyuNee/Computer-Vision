#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#define db double
#define pi acos(-1)
using namespace std;
using namespace cv;

int size;
db G_d1[10000000];

Mat source_image;
Mat padding_image;
db middle_image[1000][1000][3];
Mat transformed_image;
Mat transformed_image2;

db get_value(int x , int sigma){
    db G_value = exp(-(x * x * 1.0) / (2.0 * sigma * sigma));
    return G_value;
}

void transform(int sigma);

void Gauss_init_1d(int sigma , void* userdata){
    db sum = 0;
    size = 6 * sigma - 1;
    int center = size / 2;
    for(int i = 0 ; i < size ; i++){
        G_d1[i] = get_value(i - center, sigma);
        sum += G_d1[i];
    }

    for(int i = 0 ; i < size ; i++){
        G_d1[i] /= sum;
        cout << G_d1[i] << endl;
    }
    transform(sigma);
}

void transform(int sigma){
    if(sigma == 0)
        sigma = 1;
    int padding = size / 2;
    cv::copyMakeBorder(source_image , padding_image , padding , padding , padding , padding , cv::BORDER_DEFAULT);
    //middle_image = Mat::zeros(padding_image.rows , source_image.cols , source_image.type());
    transformed_image = Mat::zeros(source_image.size(), source_image.type());

    for(int r = 0 ; r < padding_image.rows ; r++)//遍历所有行
        for(int step = 0 ; step < source_image.cols ; step++){//填充每一行的所有值
            for(int c = 0 ; c < 3 ; c++){
                middle_image[r][step][c] = 0;
                for(int i = 0 ; i < size ; i++){//一次加权经过size次相加
                    middle_image[r][step][c] +=
                            padding_image.at<Vec3b>(r , step + i)[c] * G_d1[i];
                }
            }
        }

    for(int col = 0 ; col < source_image.cols ; col++)
        for(int step = 0 ; step < source_image.rows ; step++)
            for(int c = 0 ; c < 3 ; c++){
                db temp = 0;
                for(int i = 0 ; i < size ; i++)//一次加权经过size次相加
                    temp += middle_image[step+i][col][c] * G_d1[i];
                transformed_image.at<Vec3b>(step , col)[c] = int(temp);
        }

    cv::GaussianBlur(source_image , transformed_image2 , Size(size,size) ,sigma , sigma , cv::BORDER_DEFAULT);
    imshow("Transformed Window" , transformed_image);
    imshow("Transformed Window2" , transformed_image2);
}

int main()
{
    source_image = imread("D:\\cLion\\project\\mnls.png");

    int sigma = 2;

    namedWindow("Transformed Window", WINDOW_AUTOSIZE);
    createTrackbar("sigma", "Transformed Window", &sigma , 20, Gauss_init_1d , 0);

    imshow("Display Window" , source_image);


    waitKey(0);
}