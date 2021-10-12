#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include<vector>
#include <iostream>
using namespace std;
using namespace cv;

vector<vector<pair<double , double>>> transformed;
Mat source_image;
Mat transformed_image;

vector<double> getPosition(double x , double y){ //找 x,y 在原图中对应的坐标
    vector<double> temp;
    double r = sqrt(x*x + y*y);
    double theta = (1 - r) * (1 - r);

    if(r >= 1){
        temp.push_back(x * 1.0);
        temp.push_back(y * 1.0);
    }
    else{
        temp.push_back(cos(theta) * x - sin(theta) * y);
        temp.push_back(sin(theta) * x + cos(theta) * y);
    }
    return temp;
}

vector<vector<pair<double , double>>> preProcess(Mat& image){ //归一化
    int rows = image.rows;
    int cols = image.cols;
    int W = rows - 1;
    int H = cols - 1;
    vector<vector<pair<double , double>>> transformedP(rows , vector<pair<double,double>>(cols));
    for(int i = 0 ; i < rows ; i++){
        for(int j = 0 ; j < cols ; j++){
            double x = (i - 0.5 * W) / (0.5 * W);
            double y = (j - 0.5 * H) / (0.5 * H);
            transformedP[i][j] = pair<double,double>(x , y); // 通过i 和 j 检索归一化后的坐标
        }
    }
    return transformedP;
}

void transformBack(double& x , double& y , Mat& image){
    int source_W = image.rows - 1;
    int source_H = image.cols - 1;
    x = 0.5 * source_W * (1 + x);
    y = 0.5 * source_H * (1 + y);
}

void interpolation(){
    for(int i = 0 ; i < transformed_image.rows - 1 ; i++){
        for(int j = 0 ; j < transformed_image.cols - 1 ; j++){
            double x = transformed[i][j].first;
            double y = transformed[i][j].second; //变换图该位置归一化后的坐标
            vector<double> temp = getPosition(x , y); // 该坐标在原图中的位置
            x = temp[0] , y = temp[1];
            transformBack(x, y , source_image); //转回
            int x0 = int(x); int x1 = x0 + 1;
            int y0 = int(y); int y1 = y0 + 1; // 插值的四个点的坐标
            for(int c = 0 ; c < 3 ; c++){
                transformed_image.at<Vec3b>(i , j)[c] =
                (y1-y) * ((x1-x)*source_image.at<Vec3b>(x0,y0)[c] + (x-x0) * source_image.at<Vec3b>(x1,y0)[c]) +
                (y-y0) * ((x1-x)*source_image.at<Vec3b>(x0,y1)[c] + (x-x0)*source_image.at<Vec3b>(x1,y1)[c]);
            }
        }
    }
}

int main()
{
    source_image = imread("D:\\cLion\\project\\mnls.png");
    //source = preProcess(source_image);// 记录source归一化之后的坐标

    transformed_image = Mat::zeros(source_image.size(), source_image.type());
    transformed = preProcess(transformed_image);// 记录变换后的图形归一化之后的坐标

    interpolation();

    imshow("Transformed Window",transformed_image);
    imshow("Display Window" , source_image);
    waitKey(0);
}