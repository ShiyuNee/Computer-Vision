#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#include<vector>
#include<algorithm>
#define R 300
using namespace std;
using namespace cv;
#define PI acos(-1)
Mat source_image;
Mat target_image;
Mat binary_image;
int thread_1 = 200;
int thread_2 = 200;
int thread_3 = 15;
int H[R][1000][1000];
int radius[1000][1000];
vector<int> que_x;
vector<int> que_y;

void init(){
    source_image = imread("D:\\cLion\\project\\image\\5.jpg");
    cvtColor(source_image,binary_image , COLOR_RGB2GRAY);
    Canny(binary_image , target_image , thread_1 , thread_2);
}

void changeThread(int pos , void* userdata){
    init();
    for(int i = 0 ; i < que_x.size() ; i++){
        circle(source_image , Point(que_x[i] , que_y[i]) , radius[que_x[i]][que_y[i]] , Scalar(255 , 0 , 255));
    }

    imshow("Display Window" , source_image);
    imshow("Target Window" , target_image);
}

void vote(int x , int y , int num){
    if(target_image.at<uchar>(y,x) != 0)
        for(int r = 10 ; r < R ; r++){
            for(double theta = 0 ; theta <= 2 * PI ; theta += 0.2){
                int a = int (x - r * cos(theta));
                int b = int (y + r * sin(theta));
                if(a >= 0 && a < target_image.cols && b >= 0 && b < target_image.rows){
                        H[r][a][b] += 1;
                        if(H[r][a][b] >= num){
                            radius[a][b] = r;
                        if(find(que_x.begin() , que_x.end() , a) == que_x.end() && find(que_y.begin() , que_y.end() , b) == que_y.end()){
                            que_x.push_back(a);
                            que_y.push_back(b);
                        }

                    }
                }
            }
        }
}

void calcu(){
    que_y.clear();
    que_x.clear();
    for(int i = 0 ; i < target_image.rows ; i++)
        for(int j = 0 ; j < target_image.cols ; j++){
            vote(j , i , thread_3);
        }
}



int main()
{
    namedWindow("Target Window", WINDOW_AUTOSIZE);
    init();
    calcu();
    createTrackbar("thread_1", "Target Window", &thread_1, 200, changeThread , 0);
    createTrackbar("thread_2", "Target Window", &thread_2, 200, changeThread , 0);
    createTrackbar("thread_3", "Target Window", &thread_3, 25, changeThread , 0);

    waitKey(0);
}

