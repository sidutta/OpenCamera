#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void deskew(const char* filename, double angle)
{
    cv::Mat img = cv::imread(filename, 0);
    
    cv::bitwise_not(img, img);
    
    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());
    
    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
    
    cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
    
    cv::Mat rotated;
    cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC);
    cv::Size box_size = box.size;
    if (box.angle < -45.)
        std::swap(box_size.width, box_size.height);
    cv::Mat cropped;
    cv::getRectSubPix(rotated, box_size, box.center, cropped);

    cv::imshow("Original", img);
    cv::imshow("Rotated", rotated);
    cv::imshow("Cropped", cropped);
    cv::waitKey(0);
}