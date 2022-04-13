#include <time.h> 
#include <numeric>
#include <fstream>
#include <iostream>
#include <vector>
//#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <windows.h>

void printMat(cv::Mat A)
{
	using namespace std;
	for (int i = 0; i < A.rows; i++)
	{
		for (int j = 0; j < A.cols; j++)
		{
			cout << A.at<double>(i, j) << ' ';
		}
		cout << endl;
	}
	cout << endl;
}


int main()
{
	cv::Mat checkerboardOne, checkerboardTwo;
	checkerboardOne = cv::imread("checkerboardOne.jpg");
	if (checkerboardOne.channels() != 1)
		cv::cvtColor(checkerboardOne, checkerboardOne, CV_BGR2GRAY);
	checkerboardTwo = cv::imread("checkerboardTwo.jpg");
	if (checkerboardTwo.channels() != 1)
		cv::cvtColor(checkerboardTwo, checkerboardTwo, CV_BGR2GRAY);

	//cv::namedWindow("checkerboardOne", 0);
	//cv::imshow("checkerboardOne", checkerboardOne);
	//cv::namedWindow("checkerboardTwo", 0);
	//cv::imshow("checkerboardTwo", checkerboardTwo);

	int kp_number{ 50 };
	std::vector<cv::KeyPoint> kp1, kp2;
	cv::Ptr<cv::SiftFeatureDetector> siftdtc = cv::SiftFeatureDetector::create(kp_number);
	siftdtc->detect(checkerboardOne, kp1);
	cv::Mat outimg1;
	cv::drawKeypoints(checkerboardOne, kp1, outimg1);
	//cv::imshow("checkerboardOne keypoints", outimg1);
	//cv::imwrite("checkerboardOneKeypoints.jpg", outimg1);
	std::vector<cv::KeyPoint>::iterator itvc;
	for (itvc = kp1.begin(); itvc != kp1.end(); itvc++)
	{
		std::cout << "angle:" << itvc->angle << "\t" << itvc->class_id << "\t" << itvc->octave << "\t" << "pt ->" << itvc->pt << "\t" << itvc->response << "\t" << itvc->size << std::endl;
	}
	siftdtc->detect(checkerboardTwo, kp2);
	cv::Mat outimg2;
	cv::drawKeypoints(checkerboardTwo, kp2, outimg2);
	//cv::imshow("checkerboardTwo keypoints", outimg2);
	//cv::imwrite("checkerboardTwoKeypoints.jpg", outimg2);
	// SIFT - 特征向量提取
	cv::Ptr<cv::SiftDescriptorExtractor> extractor = cv::SiftDescriptorExtractor::create();
	cv::Mat descriptor1, descriptor2;
	extractor->compute(checkerboardOne, kp1, descriptor1);
	extractor->compute(checkerboardTwo, kp2, descriptor2);
	//cv::imshow("desc", descriptor1);
	std::cout << std::endl << "The size of feature matrix is: " << descriptor1.rows << "×" << descriptor1.cols << std::endl;

	// 两张图像的特征匹配
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
	std::vector<cv::DMatch> matches;
	cv::Mat img_matches;
	matcher->match(descriptor1, descriptor2, matches);
	cv::drawMatches(checkerboardOne, kp1, checkerboardTwo, kp2, matches, img_matches);
	//imshow("matches", img_matches);
	//cv::imwrite("matches.jpg", img_matches);
	///************************************************根据对应点求变换矩阵********************************************/
	//int pointNum = kp1.size();
	//pointNum = (pointNum < kp2.size()) ? pointNum : kp2.size();
	//cv::Point2f srcTri[4], dstTri[4];
	//int index1, index2;
	//cv::Mat checkerboardOneShow = checkerboardOne.clone();
	//cv::Mat checkerboardTwoShow = checkerboardTwo.clone();

	//int i = 2;
	//index1 = matches.at(i).queryIdx;
	//index2 = matches.at(i).trainIdx;
	//srcTri[0] = cv::Point2f(kp1[index1].pt.x, kp1[index1].pt.y);
	//cv::circle(checkerboardOneShow, srcTri[0], 2, 255, 1, 8, 0);
	//dstTri[0] = cv::Point2f(kp2[index2].pt.x, kp2[index2].pt.y);
	//cv::circle(checkerboardTwoShow, dstTri[0], 2, 255, 1, 8, 0);
	////cv::namedWindow("checkerboardOneShow", 0);
	////cv::namedWindow("checkerboardTwoShow", 0);
	////cv::imshow("checkerboardOneShow", checkerboardOneShow);
	////cv::imshow("checkerboardTwoShow", checkerboardTwoShow);
	////cv::waitKey(0);
	//i = 4;
	//index1 = matches.at(i).queryIdx;
	//index2 = matches.at(i).trainIdx;
	//srcTri[1] = cv::Point2f(kp1[index1].pt.x, kp1[index1].pt.y);
	//cv::circle(checkerboardOneShow, srcTri[1], 2, 255, 1, 8, 0);
	//dstTri[1] = cv::Point2f(kp2[index2].pt.x, kp2[index2].pt.y);
	//cv::circle(checkerboardTwoShow, dstTri[1], 2, 255, 2, 8, 0);
	////cv::namedWindow("checkerboardOneShow", 0);
	////cv::namedWindow("checkerboardTwoShow", 0);
	////cv::imshow("checkerboardOneShow", checkerboardOneShow);
	////cv::imshow("checkerboardTwoShow", checkerboardTwoShow);
	////cv::waitKey(0);
	//i = 12;
	//index1 = matches.at(i).queryIdx;
	//index2 = matches.at(i).trainIdx;
	//srcTri[2] = cv::Point2f(kp1[index1].pt.x, kp1[index1].pt.y);
	//cv::circle(checkerboardOneShow, srcTri[2], 2, 255, 1, 8, 0);
	//dstTri[2] = cv::Point2f(kp2[index2].pt.x, kp2[index2].pt.y);
	//cv::circle(checkerboardTwoShow, dstTri[2], 2, 255, 2, 8, 0);
	////cv::namedWindow("checkerboardOneShow", 0);
	////cv::namedWindow("checkerboardTwoShow", 0);
	////cv::imshow("checkerboardOneShow", checkerboardOneShow);
	////cv::imshow("checkerboardTwoShow", checkerboardTwoShow);
	////cv::waitKey(0);
	//i = 9;
	//index1 = matches.at(i).queryIdx;
	//index2 = matches.at(i).trainIdx;
	//srcTri[3] = cv::Point2f(kp1[index1].pt.x, kp1[index1].pt.y);
	//cv::circle(checkerboardOneShow, srcTri[3], 2, 255, 1, 8, 0);
	//dstTri[3] = cv::Point2f(kp2[index2].pt.x, kp2[index2].pt.y);
	//cv::circle(checkerboardTwoShow, dstTri[3], 2, 255, 2, 8, 0);

	////cv::namedWindow("checkerboardOneShow", 0);
	////cv::namedWindow("checkerboardTwoShow", 0);
	////cv::imshow("checkerboardOneShow", checkerboardOneShow);
	////cv::imshow("checkerboardTwoShow", checkerboardTwoShow);
	////cv::waitKey(0);
	////srcTri[1] = cv::Point2f(kp1[1].pt.x, kp1[1].pt.y);
	////srcTri[2] = cv::Point2f(kp1[2].pt.x, kp1[2].pt.y);
	////srcTri[3] = cv::Point2f(kp1[3].pt.x, kp1[3].pt.y);

	////dstTri[1] = cv::Point2f(kp2[1].pt.x, kp2[1].pt.y);
	////dstTri[2] = cv::Point2f(kp2[2].pt.x, kp2[2].pt.y);
	////dstTri[3] = cv::Point2f(kp2[3].pt.x, kp2[3].pt.y);
	//cv::Mat warpmatrix = cv::getPerspectiveTransform(srcTri, dstTri);
	//printMat(warpmatrix);
	//cv::Mat wrapDst = cv::Mat::zeros(checkerboardOne.rows, checkerboardOne.cols, checkerboardOne.type());
	//cv::warpPerspective(checkerboardOne, wrapDst, warpmatrix, wrapDst.size(), cv::INTER_LINEAR);//投射变换
	////cv::namedWindow("wrapDst", 0);
	////cv::imshow("wrapDst", wrapDst);

	////cv::waitKey(0);

	/************************************************求棋盘角点********************************************/
	cv::Mat checkerboardOneCornersTemp = checkerboardOne.clone();
	cv::Mat checkerboardTwoCornersTemp = checkerboardTwo.clone();

	system("matlab -nosplash -nodesktop -r getPoint");
	Sleep(15000);//等待
	std::fstream myfile;
	myfile.open("./1.txt", std::ifstream::in);
	std::string temp;
	std::vector<cv::Point> pointOne(88), pointTwo(88);
	int indx = 0;
	while (myfile >> temp)
	{
		int splitIndx = 0;
		for (int i = 0, len = temp.size(); i < len; ++i)
		{
			if (temp[i] == ',')
			{
				splitIndx = i;
				break;
			}

		}
		std::string x(temp, 0, splitIndx), y(temp, splitIndx + 1, temp.size() - splitIndx);
		pointOne[indx].x = std::atoi(x.c_str());
		pointOne[indx].y = std::atoi(y.c_str());
		++indx;
	}
	myfile.close();
	myfile.open("./2.txt", std::ifstream::in);
	indx = 0;
	while (myfile >> temp)
	{
		int splitIndx = 0;
		for (int i = 0, len = temp.size(); i < len; ++i)
		{
			if (temp[i] == ',')
			{
				splitIndx = i;
				break;
			}

		}
		std::string x(temp, 0, splitIndx), y(temp, splitIndx + 1, temp.size() - splitIndx);
		pointTwo[indx].x = std::atoi(x.c_str());
		pointTwo[indx].y = std::atoi(y.c_str());
		++indx;
	}
	myfile.close();
	for (int i = 0; i < 88; i++)
	{
		cv::circle(checkerboardOneCornersTemp, pointOne[i], 2, 255, 1, 8, 0);
		cv::circle(checkerboardTwoCornersTemp, pointTwo[i], 2, 255, 1, 8, 0);
	}
	cv::namedWindow("checkerboardOneCornersTemp", 0);
	cv::namedWindow("checkerboardTwoCornersTemp", 0);
	cv::imshow("checkerboardOneCornersTemp", checkerboardOneCornersTemp);
	cv::imshow("checkerboardTwoCornersTemp", checkerboardTwoCornersTemp);
	//cv::waitKey(0);

	//cv::Point2f srcTri[4], dstTri[4];
	//srcTri[0] = pointOne[0];
	//dstTri[0] = pointTwo[0];

	//srcTri[1] = pointOne[7];
	//dstTri[1] = pointTwo[7];

	//srcTri[2] = pointOne[80];
	//dstTri[2] = pointTwo[80];

	//srcTri[3] = pointOne[87];
	//dstTri[3] = pointTwo[87];

	//cv::Mat warpmatrix = cv::getPerspectiveTransform(srcTri, dstTri);

	myfile.open("./h.txt", std::ifstream::in);
	indx = 0;
	double h[9];
	while (myfile >> temp)
	{
		h[indx] = 1.0 * std::atoi(temp.c_str());
		++indx;
	}
	myfile.close();

	double Htemp[3][3] = { h[0],h[1],h[2],
						   h[3],h[4],h[5],
						   h[6],h[7],h[8] };
	cv::Mat warpmatrix(3, 3, CV_64F, Htemp);

	cv::Mat wrapDst = cv::Mat::zeros(checkerboardOne.rows, checkerboardOne.cols, checkerboardOne.type());
	cv::warpPerspective(checkerboardOne, wrapDst, warpmatrix, wrapDst.size(), cv::INTER_LINEAR);//投射变换
	cv::namedWindow("wrapDst", 0);
	cv::imshow("wrapDst", wrapDst);
	printMat(warpmatrix);



	cv::waitKey(0);
	return 0;
}
