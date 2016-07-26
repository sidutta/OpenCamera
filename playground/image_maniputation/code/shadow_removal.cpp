#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat frame;
    frame = imread("text2.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat back;
    cv::Mat fore;
    cv::VideoCapture cap(0);
    
    Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2(20, 16, true);
    bg->setNMixtures(3);
    bg.set("bShadowDetection",true);
    bg.set("nShadowDetection", 0); //resolved!
    bg->fTau = 0.5;           //resolved!
    
    
    std::vector<std::vector<cv::Point> > contours;
    
    cv::namedWindow("Frame");
    cv::namedWindow("Background");
    
    for(;;)
    {
        cap >> frame;
        bg->operator ()(frame,fore);
        bg->getBackgroundImage(back);
        
        cv::erode(fore,fore,cv::Mat());
        cv::dilate(fore,fore,cv::Mat());
        
        cv::findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        cv::drawContours(frame,contours,-1,cv::Scalar(0,0,255),2);
        
        cv::imshow("Frame",frame);
        cv::imshow("Background",back);
        if(cv::waitKey(30) >= 0) break;
    }
    return 0;
}