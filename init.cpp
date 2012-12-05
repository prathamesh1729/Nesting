/*
 * init.cpp
 *
 *  Created on: Oct 25, 2011
 *      Author: prathamesh
 */

#include "init.hpp"

struct templ temp[3];

void rotate(void){
	/*Clockwise rotation by 10 deg
	 *Use AntiClockwise rotation formula - since axes are reverse
	 *Call once to generate and populate the rotation coordinates
	 */
	for(int TI = 0; TI < 3; TI++){
		for(int i = 0; i < 36; i++){
			for(int j = 0; j < 4; j++){
				temp[TI].rotationx[i][j] = cos(i*10*PI/180)*(float)temp[TI].xcor[j] - sin(i*10*PI/180)*(float)temp[TI].ycor[j];
				temp[TI].rotationy[i][j] = sin(i*10*PI/180)*(float)temp[TI].xcor[j] + cos(i*10*PI/180)*(float)temp[TI].ycor[j];
				}
			}
		}
}

void initTemplate(void){

	temp[0].xcor[0] = 0;
	temp[0].xcor[1] = 0;
	temp[0].xcor[2] = 300;
	temp[0].xcor[3] = 450;
	temp[0].ycor[0] = 0;
	temp[0].ycor[1] = 300;
	temp[0].ycor[2] = 350;
	temp[0].ycor[3] = 0;
	temp[1].xcor[0] = 0;
	temp[1].xcor[1] = 0;
	temp[1].xcor[2] = 300;
	temp[1].xcor[3] = 450;
	temp[1].ycor[0] = 0;
	temp[1].ycor[1] = 300;
	temp[1].ycor[2] = 350;
	temp[1].ycor[3] = 0;
	temp[2].xcor[0] = 0;
	temp[2].xcor[1] = 0;
	temp[2].xcor[2] = 50;
	temp[2].xcor[3] = 400;
	temp[2].ycor[0] = 0;
	temp[2].ycor[1] = 250;
	temp[2].ycor[2] = 250;
	temp[2].ycor[3] = 0;
}
