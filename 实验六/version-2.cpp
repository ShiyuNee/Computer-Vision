#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
using namespace cv;
Mat source_image;
Mat target_image;
Mat binary_image;
Mat dx;
Mat dy;
int thread_1 = 86;
int thread_2 = 146;
int thread_3 = 33;
int R_max = 50;
int R_min = 30;
int H[300][1000][1000];

void init(){
    source_image = imread("D:\\cLion\\project\\image\\26.bmp");
    cvtColor(source_image,binary_image , COLOR_RGB2GRAY);
    Canny(binary_image , target_image , thread_1 , thread_2);
    memset(H , 0 , sizeof(H));
    Sobel( binary_image, dx, CV_32F, 1, 0 , 3 );
    Sobel( binary_image , dy, CV_32F, 0, 1 , 3 );
}


void vote(int x , int y){//输入的是坐标 (x,y)
    if(target_image.at<uchar>(y,x) != 0)
        for(int r = R_min ; r < R_max ; r++){
            for(double theta = -0.1 ; theta <= 0.1 ; theta = theta + 0.05){
                int a = int (x + r * cos(atan(dy.at<float>(y,x) / dx.at<float>(y,x)) + theta));
                int b = int (y + r * sin(atan(dy.at<float>(y,x) / dx.at<float>(y,x)) + theta));
                if(a >= 0 && a < target_image.cols && b >= 0 && b < target_image.rows){
                    H[r][a][b] += 1;
                }
            }
        }
}

void changeThread(int pos , void* userdata){
    init();
    for(int i = 0 ; i < target_image.rows ; i++)
        for(int j = 0 ; j < target_image.cols ; j++){
            vote(j , i);
        }

    vector<Vec3f> circles;
    circles.clear();
    for(int r = R_min ; r < R_max ; r++)
        for(int i = 0 ; i < target_image.rows ; i++)
            for(int j = 0 ; j < target_image.cols ; j++)
                if(H[r][j][i] >= thread_3){
                    //cout << r << endl;
                   circles.push_back(Vec3f(r,j,i));
                }

    for(int i = 0 ; i < circles.size() ; i++){
        circle(source_image , Point(circles[i][1] , circles[i][2]) , circles[i][0] , Scalar(0 , 255 , 0));
    }

    imshow("Display Window" , source_image);
    imshow("Target Window" , target_image);
}

int main()
{
    namedWindow("Target Window", WINDOW_AUTOSIZE);
    init();
    createTrackbar("thread_1", "Target Window", &thread_1, 200, changeThread , 0);
    createTrackbar("thread_2", "Target Window", &thread_2, 200, changeThread , 0);
    createTrackbar("thread_3", "Target Window", &thread_3, 100, changeThread , 0);
    createTrackbar("R_min", "Target Window", &R_min, 100, changeThread , 0);
    createTrackbar("R_max", "Target Window", &R_max, 400, changeThread , 0);
    waitKey(0);
}

