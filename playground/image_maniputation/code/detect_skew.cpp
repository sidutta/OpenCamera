#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

double compute_skew(const char* filename)
{
   // Load in grayscale.
    cv::Mat src = cv::imread(filename, 0);
    cv::Size size = src.size();
    cv::bitwise_not(src, src);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(src, lines, 1, CV_PI/180, 100, size.width / 2.f, 20);
    cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = lines.size();
    for (unsigned i = 0; i < nb_lines; ++i)
    {
        cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
                 cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));
        angle += atan2((double)lines[i][3] - lines[i][1],
                       (double)lines[i][2] - lines[i][0]);
    }
    angle /= nb_lines; // mean angle, in radians.

    std::cout << "File " << filename << ": " << angle * 180 / CV_PI << std::endl;

    cv::imshow(filename, disp_lines);
    cv::waitKey(0);
    //cv::destroyWindow(filename);
    return (angle * 180 / CV_PI);
}

void deskew(const char* filename)
{
    double angle;
    angle = compute_skew(filename);
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
    
    imwrite( "rotated.jpg", rotated );
    imwrite( "cropped.jpg", cropped );
//    cv::imshow("Original", img);
//    cv::imshow("Rotated", rotated);
//    cv::imshow("Cropped", cropped);
//    cv::waitKey(0);
}



int main()
{
    
    Mat image;
    
    image = imread("pic.jpg", CV_LOAD_IMAGE_COLOR);
    
    if(! image.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    // Create a new matrix to hold the gray image
    Mat gray;
    
    // convert RGB image to gray
    cvtColor(image, gray, CV_BGR2GRAY);
    
    //namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    //imshow( "Display window", image );
    
    //    namedWindow( "Result window", CV_WINDOW_AUTOSIZE );
    //    imshow( "Result window", gray );
    
    cv::Mat binaryMat(gray.size(), gray.type());
    
    //Apply thresholding
    cv::threshold(gray, binaryMat, 90, 255, cv::THRESH_BINARY);
    
    const char* filename = "gray.jpg";
    
    imwrite( filename, binaryMat );

    deskew(filename);
}

