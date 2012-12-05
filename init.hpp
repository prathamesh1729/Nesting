/*
 * init.hpp
 *
 *  Created on: Oct 25, 2011
 *      Author: prathamesh
 */

#ifndef INIT_HPP_
#define INIT_HPP_

#include <math.h>
#define PI 3.14159265

struct templ{
	int xcor[4];
	int ycor[4];
	int rotationx[36][4];
	int rotationy[36][4];
	int x;
	int y;
	int r;
	int distance;
	int closeness;

};


extern struct templ temp[3];

void rotate(void);

void initTemplate(void);


#endif /* INIT_HPP_ */
