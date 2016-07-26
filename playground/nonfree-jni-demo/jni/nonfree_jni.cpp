#include <stdio.h>
#include <iostream>
#include <jni.h>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <android/log.h>

using namespace cv;

#define  LOG_TAG    "nonfree_jni_demo"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

typedef unsigned char uchar;


extern "C" {
JNIEXPORT void JNICALL Java_com_example_nonfreejnidemo_NonfreeJNILib_runDemo2(JNIEnv * env, jobject obj, jlong result2);
};

void readme();

/** @function main */
JNIEXPORT void JNICALL Java_com_example_nonfreejnidemo_NonfreeJNILib_runDemo2(JNIEnv * env, jobject obj, jlong result2)
{
	LOGI("the new one");

	// if( argc != 3 )
	// { readme(); return -1; }

	// Load the images
	Mat image1= imread( "/sdcard/nonfree/s1.jpg" );
	Mat image2= imread(  "/sdcard/nonfree/s2.jpg"  );

	LOGI("the new one2");


	double angle = 90;  // or 270


	Mat gray_image1;
	Mat gray_image2;
	// Convert to Grayscale
	cvtColor( image1, gray_image1, CV_RGB2GRAY );
	cvtColor( image2, gray_image2, CV_RGB2GRAY );

	LOGI("the new one3");


	//imshow("first image",image2);
	// imshow("second image",image1);

	if( !gray_image1.data || !gray_image2.data )
	{ std::cout<< " --(!) Error reading images " << std::endl; }

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector( minHessian );

	std::vector< KeyPoint > keypoints_object, keypoints_scene;

	detector.detect( gray_image1, keypoints_object );
	detector.detect( gray_image2, keypoints_scene );
	LOGI("the new one4");

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute( gray_image1, keypoints_object, descriptors_object );
	extractor.compute( gray_image2, keypoints_scene, descriptors_scene );
	LOGI("the new one5");

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	LOGI("the new one5b");
	matcher.match( descriptors_object, descriptors_scene, matches );
	LOGI("the new one6");

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_object.rows; i++ )
	{
		DMatch match;
		match = matches[i];
		double dist = match.distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );
	LOGI("the new one7");

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for( int i = 0; i < descriptors_object.rows; i++ )
	{ if( matches[i].distance < 3*min_dist )
	{ const DMatch temp = matches[i]; good_matches.push_back(temp); }
	}
	std::vector< Point2f > obj2;
	std::vector< Point2f > scene;

	for( int i = 0; i < good_matches.size(); i++ )
	{
		const Point temp = keypoints_object[ good_matches[i].queryIdx ].pt;
		const Point temp2 = keypoints_scene[ good_matches[i].trainIdx ].pt;
		//-- Get the keypoints from the good matches
		obj2.push_back( temp );
		scene.push_back( temp2 );
	}
	LOGI("the new one8");

	// Find the Homography Matrix
	Mat H = findHomography( obj2, scene, CV_RANSAC );
	// Use the Homography Matrix to warp the images
	// cv::Mat result;
	Mat* result = (Mat*) result2;
	warpPerspective(image1,*result,H,cv::Size(image1.cols+image2.cols,image1.rows));
	cv::Mat half(*result,cv::Rect(0,0,image2.cols,image2.rows));
	image2.copyTo(half);
	// imshow( "Result", result );
	LOGI("the new one9");

	// imwrite("/sdcard/nonfree/s3.jpg" , result);
	// waitKey(0);
}

/** @function readme */
void readme()
{ std::cout << " Usage: Panorama < img1 > < img2 >" << std::endl; }


//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <opencv2/opencv.hpp>
//#include <jni.h>
//#include <android/log.h>
//
//#include <opencv2/stitching/stitcher.hpp>
//#define  LOG_TAG    "Siddhartha_nf"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//
//
//using namespace cv;
//using namespace std;
//
//extern "C" {
//    JNIEXPORT void JNICALL Java_com_example_nonfreejnidemo_NonfreeJNILib_runDemo2(JNIEnv * env, jobject obj, jlong result2);
//};
//
//void readme();
//
///** @function main */
//JNIEXPORT void JNICALL Java_com_example_nonfreejnidemo_NonfreeJNILib_runDemo2(JNIEnv * env, jobject obj, jlong result2)
//{
//	 vector< Mat > vImg;
//	 //Mat rImg;
//	  Mat* rImg = (Mat*) result2;
//
//	  Mat image1= imread( "/sdcard/nonfree/s1.jpg" );
//	  Mat image2= imread(  "/sdcard/nonfree/s2.jpg"  );
//
//
//	 vImg.push_back( image1 );
//	 vImg.push_back( image2 );
//
//	 LOGI("a");
//	 Stitcher stitcher = Stitcher::createDefault();
//	 LOGI("ab");
//
//	 unsigned long AAtime=0, BBtime=0; //check processing time
//	 AAtime = getTickCount(); //check processing time
//	 LOGI("ac");
//
//	 Stitcher::Status status = stitcher.stitch(vImg, *rImg);
//	 LOGI("acd");
//	 BBtime = getTickCount(); //check processing time
//	 LOGI("%.2lf sec \n",  (BBtime - AAtime)/getTickFrequency() ); //check processing time
//	 int st = status;
//	 LOGI("%d", (st));
//	 LOGI("%d", (Stitcher::OK == status));
//	 LOGI("%d", (Stitcher::ERR_NEED_MORE_IMGS == status));
//	// if (Stitcher::OK == status)
//		//  imwrite("/sdcard/nonfree/s3.jpg" , rImg);
//
//
//
//	 //waitKey(0);
//
//
//}
//
