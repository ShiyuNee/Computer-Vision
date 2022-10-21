#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#define ll long long
#define db double
using namespace std;
using namespace cv;
Mat source_image;
Mat transformed_image;
Mat transformed_image2;
ll sum2d[1000][1000][3];
ll sum_row[3]; // 记录一行三通道的和

void init(){
    for(int i = 0 ; i < 3 ; i++)
        sum_row[i] = 0;
}

db calculate(int x , int y , int padding , int c){
    int Z = (2*padding + 1) * (2*padding + 1); // 窗口内像素个数
    ll temp = sum2d[x + 2 * padding + 1][y + 2 * padding + 1][c] + sum2d[x][y][c] // sum是在padding后图像上计算的,因此原图的索引在sum上需要加上一个padding
            - sum2d[x + 2 * padding + 1][y][c] - sum2d[x][y + 2 * padding + 1][c]; // 窗口内的和
    return (temp * 1.0) / (Z * 1.0); // 计算均值
}

void MeanFilter(int windowSize , void* userdata){
    if(windowSize % 2 == 0) windowSize += 1;
    int t1_s = getTickCount(); // 开始时间1
    Mat padding_image;
    int padding = windowSize / 2;
    cv::copyMakeBorder(source_image , padding_image , padding , padding , padding , padding , cv::BORDER_DEFAULT);
    for(int i = 1 ; i <= padding_image.rows ; i++){ // 计算并保存积分图在 sum2d 中,因为使用前缀和,因此保留一个守卫节点,这里从1开始存储
        init();
        for(int j = 1 ; j <= padding_image.cols ; j++){
            for(int c = 0 ; c < 3 ; c++){
                sum2d[i][j][c] = 0;
                sum_row[c] += padding_image.at<Vec3b>(i - 1 , j - 1)[c];//计算当前行的sum
                sum2d[i][j][c] += sum2d[i - 1][j][c] + sum_row[c];
            }
        }
    }

    for(int i = 0 ; i < source_image.rows ; i++)
        for(int j = 0 ; j < source_image.cols ; j++)
            for(int c = 0 ; c < 3 ; c++)
                transformed_image.at<Vec3b>(i,j)[c] = calculate(i , j , padding , c);

    int t1_e = getTickCount(); // 结束时间1
    cout << "Manual transform :" << (t1_e - t1_s) / getTickFrequency() << ' ';

    int t2_s = getTickCount(); // 开始时间2
    boxFilter(source_image , transformed_image2 , -1 , Size(windowSize , windowSize));
    int t2_e = getTickCount(); // 结束时间2
    cout << "Opencv blur :" << (t2_e - t2_s) / getTickFrequency() << endl;
    imshow("Transformed Window" , transformed_image);
    imshow("Opencv-standard Window" , transformed_image2);
    imshow("Original Window", source_image);
}

int main()
{
    source_image = imread("D:\\cLion\\project\\mnls.png" );
    transformed_image = Mat::zeros(source_image.size() , source_image.type());
    int windowSize = 9;

    namedWindow("Transformed Window", WINDOW_AUTOSIZE);
    createTrackbar("windowSize", "Transformed Window", &windowSize , 20, MeanFilter , 0);
    waitKey(0);
}

