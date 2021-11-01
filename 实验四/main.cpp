#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
Mat image;
Mat imageCopy; //绘制矩形框时用来拷贝原图的图像
Mat targetImage;//截取的目标图像
int finish = 0;
double source_hist[3][300];
double target_hist[3][300];
Point startPoint; //矩形框起点
Point endPoint; //矩形框终点
bool leftButtonDown=false; //左键单击后视频暂停播放的标志位


void calcuHist(Mat& source_image , double hist[3][300]){
        double hist_temp[3][256];
        double hist_temp2[3][256];

        for(int i = 0 ; i < 3 ; i++)
            for(int j = 0 ; j < 256 ; j++){
                hist[i][j] = 0;
            }


        for(int i = 0 ; i < source_image.rows ; i++)
            for(int j = 0 ; j < source_image.cols ; j++)
                for(int c = 0 ; c < 3 ; c++){
                    int value = source_image.at<Vec3b>(i , j)[c];
                    hist[c][value] += 1;
                }

        for(int c = 0 ; c < 3 ; c++){
            for(int i = 0 ; i < 256 ; i++)
                hist_temp[c][i] = hist[c][i];
        }

        for(int c = 0 ; c < 3 ; c++){
            double sum = 0;
            for(int i = 0 ; i < 256 ; i++){
                sum += hist_temp[c][i];
                hist[c][i] = sum  / (source_image.rows * source_image.cols);
            }
        }
}

double compareHist(){

    double dist = 0;
    for(int c = 0 ; c < 3 ; c++){
        for(int i = 0 ; i < 256 ; i++){
            dist += pow((source_hist[c][i] - target_hist[c][i]) , 2)  / 255 /3;
        }
    }
    return dist;
}

void tracing(){
    int width = abs(endPoint.x - startPoint.x);
    int height = abs(endPoint.y - startPoint.y);
    calcuHist(targetImage , source_hist);//计算所画方框内图像的直方图，保存在source_hist中
    cout << width << ' ' << height << endl;
    //目标搜索区域设定为原区域的周围且面积为原来三倍
    int X1 = startPoint.x - width;
    int X2 = startPoint.x + width;
    int Y1 = startPoint.y - height;
    int Y2 = startPoint.y + height; // X2 和 Y2 不会越界
    //越界检查
    if (X1 < 0)
        X1 = 0;
    if (Y1 < 0)
        Y1 = 0;
    cout << X1 << ' ' << Y1 << ' ' << X2 << ' ' << Y2 << endl;

    Point targetStart;
    Point targetEnd;

    Point getStart(0, 0);
    Point getEnd(0, 0);

    VideoCapture video;
    video.open(0);
    if (!video.isOpened())
    {
        cout << "video not open.error!" << endl;
        return;
    }
    namedWindow("targetTracing");
    while (true)
    {
        //取视频的一帧
        video >> image;
        if (!image.data || waitKey(10) >= 0){
            cout << "finish";
            break;
        }
        double comnum = 999999;
        //初始化搜索区域
        for (int Cy = Y1; Cy <= Y2; Cy += 5) { //X1 , X2 , Y1 , Y2是矩形框
            for (targetStart.x = X1, targetStart.y = Cy; targetStart.x <= X2; targetStart.x += 5) {//按框进行搜索
                //防止End坐标越界0
                if ((targetStart.x + width) < image.cols)
                    targetEnd.x = targetStart.x + width;
                else
                    targetEnd.x = image.cols - 1;
                if ((targetStart.y + height) < image.rows)
                    targetEnd.y = targetStart.y + height;
                else
                    targetEnd.y = image.rows - 1;

                Mat compareImg;
                compareImg = image(Rect(targetStart, targetEnd));//取方框中的图像
                calcuHist(compareImg , target_hist);//计算直方图
                double c = compareHist();//计算两个直方图差异

                if (c < comnum) {//更新差异值，和对应的点
                    getStart = targetStart;
                    getEnd = targetEnd;
                    comnum = c;
                    //cout << c << endl;
                }
            }
        }
        if(comnum < 0.01)
            rectangle(image, getStart, getEnd, Scalar(0, 0, 255), 2);

        if(comnum < 0.1){
            X1 = getStart.x - width;
            X2 = getStart.x + width;
            Y1 = getStart.y - height;
            Y2 = getStart.y + height;

            if(X2 >= image.cols)
                X2 = image.cols - 1;
            if (X1 < 0)
                X1 = 0;
            if (Y1 < 0)
                Y1 = 0;
            if(Y2 >= image.rows)
                Y2 = image.rows - 1;
        }

//
//        //写入一帧
//        //write.write(image);
        imshow("targetTracing", image);

    }
    video.release();
    //write.release();
}

void onMouse(int event, int x, int y, int flags, void *ustc)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        leftButtonDown = true; //标志位
        startPoint = Point(x, y);  //设置左键按下点的矩形起点
        endPoint = startPoint;
    }
    if (event == EVENT_MOUSEMOVE && leftButtonDown)
    {
        imageCopy = image.clone();
        endPoint = Point(x, y);

        if (startPoint != endPoint)
        {
            //在复制的图像上绘制矩形
            rectangle(imageCopy, startPoint, endPoint, Scalar(255, 0, 0), 2);
        }
        imshow("Display Window", imageCopy);

    }
    if (event == EVENT_LBUTTONUP)
    {
        leftButtonDown = false;
        targetImage = image(Rect(startPoint, endPoint)); //获取目标图像targetImage
        finish = 1;
    }
}

//启动函数加载视频，并交互框出目标图像
void getTarget() {
    VideoCapture video;
    video.open(0);
    if (!video.isOpened())
    {
        cout << "video not open.error!" << endl;
        return;
    }
    namedWindow("Display Window");
    setMouseCallback("Display Window", onMouse);
    while (true){
        if (!leftButtonDown){
            video >> image;
            }
        if (!image.data || waitKey(1) == 27){
            break;
            }
        imshow("Display Window", image);
        if (finish == 1) {
            destroyWindow("Display Window");
            break;
        }
    }

    video.release();
}

//鼠标回调函数

int main()
{

    getTarget();
    imshow("Target object", targetImage);
    cout << startPoint << endPoint << endl;



    tracing();
    waitKey(0);
    return 0;
}

