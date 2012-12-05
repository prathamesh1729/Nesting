/*
 * common.hpp
 *
 *  Created on: Oct 25, 2011
 *      Author: prathamesh
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

//OpenCV Libraries
#include <cv.h>
#include <highgui.h>

//IO Libraries
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//Math Library
#include <math.h>

//Other MACROS
#define DRAW
#define DRAWALLT1
#define DRAWALLT2
#define DRAWALLT3
#define DEBUG
#define DEBUGL1
#define DEBUGL2
#define DEBUGL3
#define LEAFDISP
#define DRAWCONTOUR
//#define L2
//#define L3

//Constant MACROS
#define PI 3.14159265
#define DIST1 50
#define DIST2 50
#define DIST3 50
#define CLOSENESST1 2
#define CLOSENESST2 2
#define CLOSENESST3 1

using namespace cv;
using namespace std;



//Image Matrices
Mat leafColor, leaf;
Mat templColor, templ;
Mat leaf2, leaf3;

//Final Orientation and Position of Template 1
/*int t1x, t1y, r1;
int t2x, t2y, r2;
int t3x, t3y, r3;*/
int holes;

//Runtime current coordinates
int currentx, currenty;




#endif /* COMMON_HPP_ */
