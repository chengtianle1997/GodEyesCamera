#pragma once
#include "iostream"
#include "cv.h"
#include "math.h"
#include "omp.h"
#include "opencv2/opencv.hpp"
#include "highgui.h"
#include <vector>
#include <stdio.h> 
#include <fstream> 
//#include "mkl.h"
#include "tbb.h"

using namespace std;
using namespace cv;
using namespace tbb;

//Information for Every Row
typedef struct
{
	int x; //the estimated center 
	int y; //the row location
	int brightness; //the greatest brightness in real
	double cx; //Gaussfit center(Final Result)
	double cy; //the row location
	int bright; //The Greatest brightness in GaussFit
	int Rows; //the total num of rows(only saved in point[0])
	double s;
	double ay;
}MPoint;

//The information of GaussPoint
typedef struct
{
	int x; // The Cols Location
	int brightness; // The Pixel Bright
}GPoint;

//The param of GetGaussCenter
typedef struct
{
	Mat matImage; //input image
	MPoint *point; //output result
	double maxError = 0.13; //the error to top for Gausspoint selection
	double minError = 0.13; //the error to bottom for Gausspoint selection
	int xRange = 15; //horizontal range base on the estimated center for Gausspoint selection
	int threads = 2; //threads used in OpenMP for Calculation
	int StartScan = 500;
	int EndScan = 1400;

	//The Physical Param Input(From Calib Result)
	double kx = 0.9558;
	double ky = 0.9558;
	double b = 492.4;
	double phi = 1.168;
	double uo = 1226.7;
	double vo = 1008.4;
	double fx = 2371.9;
	double fy = 2358.9;

	double RangeMin = 1500;
	double RangeMax = 4000;

	int CalScale = 1;
	
}GaussCalParam;

//the param of GetGaussIdentify
typedef struct
{
	Mat matImage; //input image
	MPoint *point; //input result of Cal
	double doorin = 0.5; //the gate you can choose
}GaussIdentifyParam;

typedef struct
{
	Mat matImage; //input image
	MPoint *point; //input result of Cal
}GaussManuIdenParam;

class GaussCal {
public:
	//Sub-Pixel center-line detection algorithm base on GaussFit
	void GaussCenter(GaussCalParam &guasscalparam);

	//Automatical Identification for error base on the gate you can choose
	void GaussIdentify(GaussIdentifyParam gaussidentifyparam);

	//Manual indentification for error by mouse click
	void GaussManuIden(GaussManuIdenParam gaussmanuidenparam);

	

private:
	//calculate the n_th of x
	int fx(int x, int n);

	//generate x and z Cvmat 
	int getXZmatrix(CvMat* X, CvMat* Z, int n, GPoint *gpoint);

	int bound(short i, short a, short b);

	//On_Mouse_Callback
	static void on_mouse(int event, int x, int y, int flags, void* ustc);

	//The reverse of CvScarlar
	CvScalar getInverseColor(CvScalar c);

	Mat src;

	Mat dst;

	Mat ori;

	int n = 0;

	vector<Point> points;
};