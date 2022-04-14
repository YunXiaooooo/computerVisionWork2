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
double distanceSquare(cv::Point2f x1, cv::Point2f x2);
cv::Point2f correspondingPointCoordinates(cv::Point2f x1, double* H);
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


	/************************************************ÇóÆåÅÌ½Çµã********************************************/
	cv::Mat checkerboardOneCornersTemp = checkerboardOne.clone();
	cv::Mat checkerboardTwoCornersTemp = checkerboardTwo.clone();

	system("matlab -nosplash -nodesktop -r getPoint");
	Sleep(15000);//µÈ´ý
	std::fstream myfile;
	myfile.open("./1.txt", std::ifstream::in);
	std::string temp;
	std::vector<cv::Point2f> pointOne(88), pointTwo(88);
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
		pointOne[indx].x = 0.000001 * std::atoi(x.c_str());
		pointOne[indx].y = 0.000001 * std::atoi(y.c_str());
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
		pointTwo[indx].x = 0.000001 * std::atoi(x.c_str());
		pointTwo[indx].y = 0.000001 * std::atoi(y.c_str());
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

	myfile.open("./h.txt", std::ifstream::in);
	indx = 0;
	double h[9];
	while (myfile >> temp)
	{
		h[indx] = 0.000001 * std::atoi(temp.c_str());
		++indx;
	}
	myfile.close();

	double Htemp[3][3] = { h[0],h[1],h[2],
						   h[3],h[4],h[5],
						   h[6],h[7],h[8] };
	cv::Mat warpmatrix(3, 3, CV_64F, Htemp);

	cv::Mat wrapDst = cv::Mat::zeros(checkerboardOne.rows, checkerboardOne.cols, checkerboardOne.type());
	cv::warpPerspective(checkerboardOne, wrapDst, warpmatrix, wrapDst.size(), cv::INTER_LINEAR);//Í¶Éä±ä»»
	cv::namedWindow("wrapDst", 0);
	cv::imshow("wrapDst", wrapDst);
	printMat(warpmatrix);



	/***************************************×ªÒÆÎó²î************************************************/
	myfile.open("./hInv.txt", std::ifstream::in);
	indx = 0;
	double hInv[9];
	while (myfile >> temp)
	{
		hInv[indx] = 0.000001 * std::atoi(temp.c_str());
		++indx;
	}
	myfile.close();
	double distance = 0;
	for (int i = 0; i < 88; ++i)
	{
		cv::Point2f pointOneRefactor = correspondingPointCoordinates(pointTwo[i], hInv);
		cv::Point2f pointTwoRefactor = correspondingPointCoordinates(pointOne[i], h);
		distance = distance + distanceSquare(pointOne[i], pointOneRefactor)+ distanceSquare(pointTwo[i], pointTwoRefactor);
	}
	printf("¶Ô³Æ×ªÒÆÎó²î£º%f \n", distance);

	/***************************************ÖØÍ¶Ó°Îó²î************************************************/
	distance = 0;
	for (int i = 0; i < 88; ++i)
	{
		cv::Point2f pointOneInTwo = correspondingPointCoordinates(pointOne[i], h);
		cv::Point2f pointOneRefactor = correspondingPointCoordinates(pointOneInTwo, hInv);

		cv::Point2f pointTwoInOne = correspondingPointCoordinates(pointTwo[i], hInv);
		cv::Point2f pointTwoRefactor = correspondingPointCoordinates(pointTwoInOne, h);

		distance = distance + distanceSquare(pointOne[i], pointOneRefactor) + distanceSquare(pointTwo[i], pointTwoRefactor);
	}
	printf("ÖØÍ¶Ó°Îó²î£º%f \n", distance);

	/***************************************µ¥Í¼ÏñÎó²î************************************************/
	distance = 0;
	for (int i = 0; i < 88; ++i)
	{
		cv::Point2f pointOneInTwo = correspondingPointCoordinates(pointOne[i], h);


		distance = distance + distanceSquare(pointTwo[i], pointOneInTwo);
	}
	printf("µ¥Í¼ÏñÎó²î£º%f \n", distance);


	cv::waitKey(0);
	return 0;
}

double distanceSquare(cv::Point2f x1, cv::Point2f x2)
{
	double d = (x1.x - x2.x) * (x1.x - x2.x) + (x1.y - x2.y) * (x1.y - x2.y);
	return d;
}

cv::Point2f correspondingPointCoordinates(cv::Point2f x1, double* H)
{
	cv::Point2f x2;
	x2.x = H[0] * x1.x + H[1] * x1.y + H[2] * 1;
	x2.y = H[3] * x1.x + H[4] * x1.y + H[5] * 1;
	return x2;
}