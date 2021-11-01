#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
#define N 10000000
using namespace std;
using namespace cv;
Mat source_image;
Mat binary_image;
Mat triangle_image;
Mat target_image;
int par[N];
int label[N];

int calcu(int x , int y){
    return x * binary_image.cols + y;
}

int find(int x){
    if(x == par[x])
        return x;
    else
        return par[x] = find(par[x]);
}

void unit(int x , int y){
    x = find(x);
    y = find(y);
    if(x == y)
        return;
    else
        par[x] = y;
}

void init(){ //三通道转为灰度图，以127分隔，归为 0 和 255
    cvtColor(source_image,binary_image , COLOR_RGB2GRAY);//转为灰度图
    //分隔
    for(int i = 0 ; i < binary_image.rows ; i++)
        for(int j = 0 ; j < binary_image.cols ; j++)
            if(binary_image.at<uchar>(i,j) > 127)
                binary_image.at<uchar>(i,j) = 255;
            else
                binary_image.at<uchar>(i,j) = 0;

    //初始化并查集
    for(int i = 0 ; i < binary_image.rows ; i++)
        for(int j = 0 ; j < binary_image.cols ; j++)
            par[calcu(i,j)] = calcu(i,j);
}

void process(){
    for(int i = 0 ; i < binary_image.rows ; i++){
        for(int j = 0 ; j < binary_image.cols ; j++){
          if(binary_image.at<uchar>(i,j) == 255){
              for(int k = -1 ; k <=1 ; k++){
                  if(i > 0 && j + k >= 0 && j + k < binary_image.cols && binary_image.at<uchar>(i-1 , j+k) == 255)
                      unit(calcu(i,j) ,calcu(i-1 , j+k));
              }
              if(j > 0 && binary_image.at<uchar>(i , j - 1) == 255)
                  unit(calcu(i,j) , calcu(i , j-1));
          }
        }
    }
}

int findMaxArea(){
    int max_num = 0;
    int max_label = 0;
    for(int i = 0 ; i < binary_image.rows ; i++){
        for(int j = 0 ; j < binary_image.cols ; j++){
            int value = find(calcu(i,j));

            if(binary_image.at<uchar>(i,j) != 0){//黑的不加，方便后面算连通域的个数
                label[value]++;
            }

            if(label[value] > max_num){
                max_num = label[value];
                max_label = value;
            }
        }
    }
    return max_label;
}

void draw(int label){
    for(int i = 0 ; i < binary_image.rows ; i++)
        for(int j = 0 ; j < binary_image.cols ; j++){
            if(find(calcu(i , j)) != label)
                binary_image.at<uchar>(i , j) = 0;
        }
    //imshow("Source Window" , source_image);
    imshow("Display Window" , binary_image);
    waitKey(0);
}

int main()
{
    source_image = imread("D:\\cLion\\project\\horse_mask.png");
    init();
    process();
    draw(findMaxArea());

    int num_label = 0;
    for(int i = 0 ; i < binary_image.rows * binary_image.cols ; i++)
        if(label[i] > 0) num_label++;
    cout << num_label;

    triangle_image = imread("D:\\cLion\\project\\triangle.png");
    cvtColor(triangle_image,triangle_image , COLOR_RGB2GRAY);
    distanceTransform(triangle_image, target_image , DIST_L2, 3); //欧式距离 ， 3 * 3的mask
    normalize(target_image, target_image, 0, 1, NORM_MINMAX);

    imshow("Target Window" , target_image);
    waitKey(0);

}

