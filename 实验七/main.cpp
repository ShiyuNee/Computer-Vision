#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
#define PI acos(-1)
Mat opencv_image;
Mat source_image;
Mat binary_image;
Mat dx;
Mat dy;
int thread = 50;
int thegma = 3;
double Gauss[1000][1000];
int k_int = 6;
int size = 5;
string image_path = "D:\\cLion\\project\\image\\18.jpg";

void getGauss(){
    if(size % 2 == 0)
        size += 1;
    if(thegma == 0)
        thegma += 1;

    int center = size / 2;
    double sum = 0;
    for(int i = 0 ; i < size ; i++){
        for(int j = 0 ; j < size ; j++){
            Gauss[i][j] =
                    exp(-( (i - center) * (i - center) + (j - center) * (j - center) ) / (2 * thegma * thegma)) / (2 * PI * thegma * thegma);
            sum += Gauss[i][j];
        }
    }

    for(int i = 0 ; i < size ; i++)
        for(int j = 0 ; j < size ; j++)
            Gauss[i][j] /= sum;
}

void calcuM(double k , Mat& target , int size){
    int dist = size / 2;

    for(int i = dist ; i < binary_image.rows - dist ; i++){
        for(int j = dist ; j < binary_image.cols - dist ; j++){
            double M[2][2] = {0};
            for(int kx = -dist ; kx <= dist ; kx++){
                for(int ky = -dist ; ky <= dist ; ky++){
                    int new_x = i + kx;
                    int new_y = j + ky;
                    M[0][0] += Gauss[kx + dist][ky + dist] * dx.at<float>(new_x , new_y) * dx.at<float>(new_x , new_y);
                    M[0][1] += Gauss[kx + dist][ky + dist] * dx.at<float>(new_x , new_y) * dy.at<float>(new_x , new_y);
                    M[1][0] += Gauss[kx + dist][ky + dist] * dx.at<float>(new_x , new_y) * dy.at<float>(new_x , new_y);
                    M[1][1] += Gauss[kx + dist][ky + dist] * dy.at<float>(new_x , new_y) * dy.at<float>(new_x , new_y);
                }
            }
            double det_M = M[0][0] * M[1][1] - M[1][0] * M[0][1];
            double trace_M = M[0][0] + M[1][1];
            target.at<float>(i,j) = float(det_M - k * trace_M * trace_M);
        }
    }
}

void changeThread(int pos , void* userdata){
    source_image = imread(image_path);
    Mat source_image2 = imread(image_path);
    getGauss();
    float thread2 = thread * 1.0 / 100.0;
    double k2 = k_int * 1.0 / 100.0;
    int size2;
    if(size % 2 == 0)
        size2 = size + 1;
    else
        size2 = size;

    Mat target = Mat::zeros(dx.rows , dx.cols , dx.type());

    //调用角点检测函数并计算时间
    auto t = (double)getTickCount();
    calcuM(k2 , target , size2);
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "own function time(s): " << t << endl;
    //归一化
    normalize(target , target, 0, 1, NORM_MINMAX);

    for(int i = 0 ; i < target.rows ; i++)
        for(int j = 0 ; j < target.cols ; j++)
            if(target.at<float>(i,j) > thread2)
                circle(source_image , Point(j,i) , 2 , Scalar(0 , 255 , 0));

    //opencv内置角点检测算法
    t = (double)getTickCount();
    cornerHarris(binary_image , opencv_image , size2 , 3 , k2 , BORDER_DEFAULT);
    t = ((double)getTickCount() - t)/getTickFrequency();
    cout << "opencv function time(s): " << t << endl;

    normalize(opencv_image , opencv_image , 0 , 1 , NORM_MINMAX);
    for(int i = 0 ; i < target.rows ; i++)
        for(int j = 0 ; j < target.cols ; j++)
            if(opencv_image.at<float>(i,j) > thread2)
                circle(source_image2 , Point(j,i) , 2 , Scalar(0 , 255 , 0));

    imshow("Opencv Window" , source_image2);
    imshow("Target Window" , source_image);
}

int main()
{
    source_image = imread(image_path);
    cvtColor(source_image,binary_image , COLOR_RGB2GRAY);
    Sobel( binary_image, dx, CV_32F, 1, 0 , 3);
    Sobel( binary_image , dy, CV_32F, 0, 1 , 3);

    namedWindow("Target Window" , WINDOW_AUTOSIZE);
    createTrackbar("thread", "Target Window", &thread, 500, changeThread , 0);
    createTrackbar("size", "Target Window", &size, 20, changeThread , 0);
    createTrackbar("thegma", "Target Window", &thegma, 20, changeThread , 0);
    createTrackbar("k", "Target Window", &k_int, 10, changeThread , 0);

    waitKey(0);
}


