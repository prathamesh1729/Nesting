/*
 * main.cpp
 *
 *  Created on: Oct 25, 2011
 *      Author: prathamesh
 */

#include "common.hpp"
#include "init.hpp"



int main(int argc, char* argv[]){

	initTemplate();
	rotate();
	leafColor = imread( "conv2.png", 1 );
	cvtColor( leafColor, leaf, CV_BGR2GRAY );


	vector<vector<Point> > lcontours;
	vector<Vec4i> lhierarchy;
	vector<vector<Point> > tcontours(3);
	vector<Vec4i> thierarchy;

	vector<vector<Point> > tempcontours;
	vector<Vec4i > temphierarchy;


	/*vector<vector<Point> > testcontours(3);
	testcontours[0].push_back(Point(0,0));
	testcontours[0].push_back(Point(1,2));
	cout<<testcontours.size()<<endl;
	cout<<testcontours[0].size()<<endl;*/


	Mat drawing = Mat::zeros( leaf.size(), CV_8UC3 );	//Final image to be displayed
	Mat finaldrawing = Mat::zeros( leaf.size(), CV_8UC3 );	//Final image to be displayed
	Mat contours = Mat::zeros( leaf.size(), CV_8UC3 );	//Final image to be displayed
	Mat leaf2 = Mat::zeros(leaf.size(), CV_8UC1); 		//Image for template 2
	Mat leaf3 = Mat::zeros(leaf.size(), CV_8UC1); 		//Image for template 3


	//findContours( templ, tcontours, thierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	findContours( leaf, lcontours, lhierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			tcontours[i].push_back(Point(0,0));
		}
	}



#ifdef DRAWCONTOUR
	for(int i = 0; i < (int)lcontours.size(); i++){
		   Scalar color = Scalar( 255, 255, 255);
		   drawContours( contours, lcontours, i, color, 2, 8, lhierarchy, 0, Point() );
		}
		namedWindow( "Original Contours", 0 );
		imshow( "Original Contours", contours);
#endif
	/*Size of image available as
	 * leaf.size().width
	 * leaf.size().height
	 */

	//Number of holes in leaf
	holes = lcontours.size();
#ifdef DEBUG
	cout<<"Number of holes in leaf: "<<holes - 1<<endl;
#endif

	//Common Variables - INIT before T1 = 0

	int minAvgDist = DIST1 + 1;
	int totalPossibleCases = 0;
	float distanceLeaf[4];
	int distanceHole;
	bool insideLeaf = true;
	int closeLeaf = 0;
	int avgLeafDistance = 0;

	/* -----------------------------
	 * Check for Template 1
	 * -----------------------------
	 */

	int TI = 0;
	for(int RI = 0; RI < 36; RI++){
		for(int x = 0; x < (int)(leaf.size().width/50); x++){
			for(int y = 0; y < (int)(leaf.size().height/50); y++){
				currentx = 50*x;
				currenty = 50*y;

				//Variables used in switch below
				insideLeaf = true;
				closeLeaf = 0;
				avgLeafDistance = 0;

				/*INSIDE - OUTSIDE TEST
				 *
				 */
				/*
				 * Check if - 1. Template is inside the leaf 2. Not overlapping with any hole
				 * Eliminate all cases that violate the above two
				 * If there are remainder cases, then
				 * Check if any of the remainder cases fit -
				 * 	3 points close to boundary, or 2 points close to boundary
				 * 	Choose an optimum - using some algorithm
				 */
				for(int check = 0; check < 4; check++){
					distanceLeaf[check] = pointPolygonTest(lcontours[0], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), true);
					if(distanceLeaf[check] < 0){insideLeaf = false; break;}
					else if(distanceLeaf[check] < DIST1){closeLeaf++;}
				}

				if(closeLeaf == CLOSENESST1 && insideLeaf){

					for(int i = 1; i < (int)lcontours.size(); i++){

						//Check if any point is inside hole
						for(int check = 0; check < 4; check++){
							distanceHole = pointPolygonTest(lcontours[i], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
							if(distanceHole >= 0){insideLeaf = false; break;}
						}

						//Check if any point on hole is inside the template
						for(int j = 0; j < (int)tcontours[TI].size(); j++){
							tcontours[TI][j].x = temp[TI].rotationx[RI][j] + currentx;
							tcontours[TI][j].y = temp[TI].rotationy[RI][j] + currenty;
						}
#ifdef DEBUGL1
						circle(drawing, Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), 10, Scalar(255, 255, 255));
#endif
						if(pointPolygonTest(tcontours[0], Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
						for(int test = 0; test < (int)lcontours[i].size()/10; test++){
							if(pointPolygonTest(tcontours[0], Point(lcontours[i][test*10].x, lcontours[i][test*10].y), false) >= 0){insideLeaf = false;}
#ifdef DEBUGL1
							circle(drawing, Point(lcontours[i][test*10].x, lcontours[i][test*10].y), 10, Scalar(255, 255, 255));
#endif
						}
						if(!insideLeaf)break;
					}

					if(insideLeaf){
						//Final and filtered set of useful combinations
#ifdef DRAWALLT1
						cout<<currentx<<" "<<currenty<<" "<<RI<<endl;
						for(int i = 0; i < (int)tcontours[TI].size(); i++){
							tcontours[TI][i].x = temp[TI].rotationx[RI][i] + currentx;
							tcontours[TI][i].y = temp[TI].rotationy[RI][i] + currenty;
						}
						drawContours( drawing, tcontours, 0, Scalar( 0, 255, 0), 2, 8, thierarchy, 0, Point() );
#endif
						totalPossibleCases++;
						//Grab the optimum - average method
						for(int i = 0; i < 4; i++){
							if(distanceLeaf[i] < DIST1)avgLeafDistance += distanceLeaf[i];
						}
						avgLeafDistance /= 3;
						if(avgLeafDistance < minAvgDist){
							minAvgDist = avgLeafDistance;
							temp[TI].x = currentx;
							temp[TI].y = currenty;
							temp[TI].r = RI;
						}
					}
				}
			}
		}
	}

#ifdef DEBUG
	cout<<"Total possible cases for template 1: "<<totalPossibleCases<<endl;
#endif

	/* Get image ready for template 2
	 * Draw contours of original image and finalised position of template 1 on Mat leaf2
	 *
	 */


	for(int i = 0; i < (int)tcontours[TI].size(); i++){
			tcontours[TI][i].x = temp[TI].rotationx[temp[TI].r][i] + temp[TI].x;
			tcontours[TI][i].y = temp[TI].rotationy[temp[TI].r][i] + temp[TI].y;
		}
	drawContours( leaf2, tcontours, 0, Scalar( 255, 255, 255), 2, 8, thierarchy, 0, Point() );

#ifdef LEAFDISP
	namedWindow( "Leaf 2", 0 );
	imshow( "Leaf 2", leaf2);
#endif

	findContours( leaf2, tempcontours, temphierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//lcontours[1] = tempcontours[0];
	lcontours.push_back(tempcontours[0]);

	/* ------------------------------------
	 * Check for template 2
	 * ------------------------------------
	 *
	 */
	minAvgDist = DIST2 + 1;
	totalPossibleCases = 0;

	TI = 1;
	for(int RI = 0; RI < 36; RI++){
			for(int x = 0; x < (int)(leaf.size().width/50); x++){
				for(int y = 0; y < (int)(leaf.size().height/50); y++){
					currentx = 50*x;
					currenty = 50*y;

					insideLeaf = true;
					closeLeaf = 0;
					avgLeafDistance = 0;

					/*INSIDE - OUTSIDE TEST
					 *
					 */
					/*
					 * Check if - 1. Template is inside the leaf 2. Not overlapping with any hole
					 * Eliminate all cases that violate the above two
					 * If there are remainder cases, then
					 * Check if any of the remainder cases fit -
					 * 	3 points close to boundary, or 2 points close to boundary
					 * 	Choose an optimum - using some algorithm
					 */
					for(int check = 0; check < 4; check++){
						distanceLeaf[check] = pointPolygonTest(lcontours[0], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), true);
						if(distanceLeaf[check] < 0){insideLeaf = false; break;}
						else if(distanceLeaf[check] < DIST2){closeLeaf++;}
					}

					if(closeLeaf == CLOSENESST2 && insideLeaf){

						/* HOLES and INNER CONTOUR test
						 *
						 * ----------------------------
						 */
						for(int i = 1; i < (int)lcontours.size(); i++){

							//Check if any point is inside hole
							for(int check = 0; check < 4; check++){
								distanceHole = pointPolygonTest(lcontours[i], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
								if(distanceHole >= 0){insideLeaf = false; break;}
							}

							//Check if any point on hole is inside the template
							for(int j = 0; j < (int)tcontours[0].size(); j++){
								tcontours[0][j].x = temp[TI].rotationx[RI][j] + currentx;
								tcontours[0][j].y = temp[TI].rotationy[RI][j] + currenty;
							}
#ifdef DEBUGL2
							circle(drawing, Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), 10, Scalar(255, 255, 255));
#endif
							if(pointPolygonTest(tcontours[0], Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
							for(int test = 0; test < (int)lcontours[i].size()/10; test++){
								if(pointPolygonTest(tcontours[0], Point(lcontours[i][test*10].x, lcontours[i][test*10].y), false) >= 0){insideLeaf = false;}
#ifdef DEBUGL2
								circle(drawing, Point(lcontours[i][test*10].x, lcontours[i][test*10].y), 10, Scalar(255, 255, 255));
#endif
							}


							if(!insideLeaf)break;
						}

						/*
						 * Check with template 1 Test
						 * --------------------------
						 *
						 */
#ifdef L2
							if(insideLeaf){
								//Check if any point is inside hole
								for(int check = 0; check < 4; check++){
									distanceHole = pointPolygonTest(lcontours[1], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
									if(distanceHole >= 0){insideLeaf = false; break;}
								}

								//Check if any point on hole is inside the template
								for(int j = 0; j < (int)tcontours[0].size(); j++){
									tcontours[0][j].x = temp[TI].rotationx[RI][j] + currentx;
									tcontours[0][j].y = temp[TI].rotationy[RI][j] + currenty;
								}
								if(pointPolygonTest(tcontours[0], Point((lcontours[1][0].x + lcontours[1][lcontours[1].size()/2].x)/2,(lcontours[1][0].y + lcontours[1][lcontours[1].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
								for(int test = 0; test < (int)lcontours[1].size()/10; test++){
									if(pointPolygonTest(tcontours[0], Point(lcontours[1][test*10].x, lcontours[1][test*10].y), false) >= 0){insideLeaf = false;}
#ifdef DEBUGL2
									circle(drawing, Point(l2contours[0][test*10].x, l2contours[0][test*10].y), 10, Scalar(255, 255, 255));
#endif
								}
							}
#endif

						if(insideLeaf){
							//Final and filtered set of useful combinations
#ifdef DRAWALLT2
							cout<<currentx<<" "<<currenty<<" "<<RI<<endl;
							for(int i = 0; i < (int)tcontours[0].size(); i++){
								tcontours[0][i].x = temp[TI].rotationx[RI][i] + currentx;
								tcontours[0][i].y = temp[TI].rotationy[RI][i] + currenty;
							}
							drawContours( drawing, tcontours, 0, Scalar( 0, 0, 255), 2, 8, thierarchy, 0, Point() );
#endif
							totalPossibleCases++;
							//Grab the optimum - average method
							for(int i = 0; i < 4; i++){
								if(distanceLeaf[i] < DIST2)avgLeafDistance += distanceLeaf[i];
							}
							avgLeafDistance /= 3;
							if(avgLeafDistance < minAvgDist){
								minAvgDist = avgLeafDistance;
								temp[TI].x = currentx;
								temp[TI].y = currenty;
								temp[TI].r = RI;
							}
						}
					}
				}
			}
	}

#ifdef DEBUG
	cout<<"Total possible cases for template 2: "<<totalPossibleCases<<endl;
#endif

	/*
	 * Get ready for template 3.
	 * Draw contours of placement of template 2 on Mat leaf2
	 *
	 */
	for(int i = 0; i < (int)tcontours[0].size(); i++){
				tcontours[0][i].x = temp[TI].rotationx[temp[TI].r][i] + temp[TI].x;
				tcontours[0][i].y = temp[TI].rotationy[temp[TI].r][i] + temp[TI].y;
			}
		drawContours( leaf3, tcontours, 0, Scalar( 255, 255, 255), 2, 8, thierarchy, 0, Point() );

#ifdef LEAFDISP
	namedWindow( "Leaf 3", 0 );
	imshow( "Leaf 3", leaf3);
#endif

	findContours( leaf3, tempcontours, temphierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//lcontours[2] = tempcontours[0];
	lcontours.push_back(tempcontours[0]);


	/*
	 * --------------------
	 * Check for template 3
	 * --------------------
	 */

	minAvgDist = DIST3 + 1;
	totalPossibleCases = 0;

	TI = 2;
	for(int RI = 0; RI < 36; RI++){
			for(int x = 0; x < (int)(leaf.size().width/50); x++){
				for(int y = 0; y < (int)(leaf.size().height/50); y++){
					currentx = 50*x;
					currenty = 50*y;

					insideLeaf = true;
					closeLeaf = 0;
					avgLeafDistance = 0;

					/*INSIDE - OUTSIDE TEST
					 *
					 */
					/*
					 * Check if - 1. Template is inside the leaf 2. Not overlapping with any hole
					 * Eliminate all cases that violate the above two
					 * If there are remainder cases, then
					 * Check if any of the remainder cases fit -
					 * 	3 points close to boundary, or 2 points close to boundary
					 * 	Choose an optimum - using some algorithm
					 */
					for(int check = 0; check < 4; check++){
						distanceLeaf[check] = pointPolygonTest(lcontours[0], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), true);
						if(distanceLeaf[check] < 0){insideLeaf = false; break;}
						else if(distanceLeaf[check] < DIST3){closeLeaf++;}
					}

					if(closeLeaf == CLOSENESST3 && insideLeaf){

						/* HOLES and INNER CONTOUR test
						 *
						 * ----------------------------
						 */
						for(int i = 1; i < (int)lcontours.size(); i++){

							//Check if any point is inside hole
							for(int check = 0; check < 4; check++){
								distanceHole = pointPolygonTest(lcontours[i], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
								if(distanceHole >= 0){insideLeaf = false; break;}
							}

							//Check if any point on hole is inside the template
							for(int j = 0; j < (int)tcontours[0].size(); j++){
								tcontours[0][j].x = temp[TI].rotationx[RI][j] + currentx;
								tcontours[0][j].y = temp[TI].rotationy[RI][j] + currenty;
							}
#ifdef DEBUGL3
							circle(drawing, Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), 10, Scalar(255, 255, 255));
#endif
							if(pointPolygonTest(tcontours[0], Point((lcontours[i][0].x + lcontours[i][lcontours[i].size()/2].x)/2,(lcontours[i][0].y + lcontours[i][lcontours[i].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
							for(int test = 0; test < (int)lcontours[i].size()/10; test++){
								if(pointPolygonTest(tcontours[0], Point(lcontours[i][test*10].x, lcontours[i][test*10].y), false) >= 0){insideLeaf = false;}
#ifdef DEBUGL3
								circle(drawing, Point(lcontours[i][test*10].x, lcontours[i][test*10].y), 10, Scalar(255, 255, 255));
#endif
							}


							if(!insideLeaf)break;
						}

						/*
						 * Check with template 1 Test
						 * --------------------------
						 *
						 */
#ifdef L3
							if(insideLeaf){
								//Check if any point is inside hole
								for(int check = 0; check < 4; check++){
									distanceHole = pointPolygonTest(lcontours[1], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
									if(distanceHole >= 0){insideLeaf = false; break;}
								}

								//Check if any point on hole is inside the template
								for(int j = 0; j < (int)tcontours[0].size(); j++){
									tcontours[0][j].x = temp[TI].rotationx[RI][j] + currentx;
									tcontours[0][j].y = temp[TI].rotationy[RI][j] + currenty;
								}
								if(pointPolygonTest(tcontours[0], Point((lcontours[1][0].x + lcontours[1][lcontours[1].size()/2].x)/2,(lcontours[1][0].y + lcontours[1][lcontours[1].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
								for(int test = 0; test < (int)lcontours[1].size()/10; test++){
									if(pointPolygonTest(tcontours[0], Point(lcontours[1][test*10].x, lcontours[1][test*10].y), false) >= 0){insideLeaf = false;}
#ifdef DEBUGL3
									circle(drawing, Point(lcontours[1][test*10].x, lcontours[1][test*10].y), 10, Scalar(255, 255, 255));
#endif
								}
							}
#endif


							/*
							 * Check with template 2 Test
							 * --------------------------
							 *
							 */
#ifdef L3
								if(insideLeaf){
									//Check if any point is inside hole
									for(int check = 0; check < 4; check++){
										distanceHole = pointPolygonTest(lcontours[2], Point(currentx + temp[TI].rotationx[RI][check], currenty + temp[TI].rotationy[RI][check]), false);
										if(distanceHole >= 0){insideLeaf = false; break;}
									}

									//Check if any point on hole is inside the template
									for(int j = 0; j < (int)tcontours[0].size(); j++){
										tcontours[0][j].x = temp[TI].rotationx[RI][j] + currentx;
										tcontours[0][j].y = temp[TI].rotationy[RI][j] + currenty;
									}
									if(pointPolygonTest(tcontours[0], Point((lcontours[1][0].x + lcontours[1][lcontours[1].size()/2].x)/2,(lcontours[1][0].y + lcontours[1][lcontours[1].size()/2].y)/2 ), false) >= 0)insideLeaf = false;
									for(int test = 0; test < (int)lcontours[2].size()/10; test++){
										if(pointPolygonTest(tcontours[0], Point(lcontours[2][test*10].x, lcontours[2][test*10].y), false) >= 0){insideLeaf = false;}
	#ifdef DEBUGL3
										circle(drawing, Point(lcontours[2][test*10].x, lcontours[2][test*10].y), 10, Scalar(255, 255, 255));
	#endif
									}
								}
#endif

						if(insideLeaf){
							//Final and filtered set of useful combinations
#ifdef DRAWALLT3
							cout<<currentx<<" "<<currenty<<" "<<RI<<endl;
							for(int i = 0; i < (int)tcontours[0].size(); i++){
								tcontours[0][i].x = temp[TI].rotationx[RI][i] + currentx;
								tcontours[0][i].y = temp[TI].rotationy[RI][i] + currenty;
							}
							drawContours( drawing, tcontours, 0, Scalar( 255, 0, 0), 2, 8, thierarchy, 0, Point() );
#endif
							totalPossibleCases++;
							//Grab the optimum - average method
							for(int i = 0; i < 4; i++){
								if(distanceLeaf[i] < DIST3)avgLeafDistance += distanceLeaf[i];
							}
							avgLeafDistance /= 3;
							if(avgLeafDistance < minAvgDist){
								minAvgDist = avgLeafDistance;
								temp[TI].x = currentx;
								temp[TI].y = currenty;
								temp[TI].r = RI;
							}
						}
					}
				}
			}
	}

#ifdef DEBUG
	cout<<"Total possible cases for template 3: "<<totalPossibleCases<<endl;
#endif



	/*
	 * Final Drawing!
	 * --------------
	 */

#ifdef DRAW
	TI = 0;
	for(int i = 0; i < (int)tcontours[0].size(); i++){
		tcontours[0][i].x = temp[0].rotationx[temp[TI].r][i] + temp[TI].x;
		tcontours[0][i].y = temp[0].rotationy[temp[TI].r][i] + temp[TI].y;
	}
	TI = 1;
	drawContours( finaldrawing, tcontours, 0, Scalar( 0, 255, 0), 2, 8, thierarchy, 0, Point() );
	for(int i = 0; i < (int)tcontours[0].size(); i++){
		tcontours[0][i].x = temp[1].rotationx[temp[TI].r][i] + temp[TI].x;
		tcontours[0][i].y = temp[1].rotationy[temp[TI].r][i] + temp[TI].y;
	}
	TI = 2;
	drawContours( finaldrawing, tcontours, 0, Scalar( 0, 0, 255), 2, 8, thierarchy, 0, Point() );
	for(int i = 0; i < (int)tcontours[0].size(); i++){
		tcontours[0][i].x = temp[2].rotationx[temp[TI].r][i] + temp[TI].x;
		tcontours[0][i].y = temp[2].rotationy[temp[TI].r][i] + temp[TI].y;
	}
	for(int i = 0; i < (int)lcontours.size() - 2; i++){
		   Scalar color = Scalar( 255, 255, 255);
		   drawContours( finaldrawing, lcontours, i, color, 2, 8, lhierarchy, 0, Point() );
	}
	drawContours( finaldrawing, tcontours, 0, Scalar( 255, 0, 0), 2, 8, thierarchy, 0, Point() );
	namedWindow( "Final Drawing", 0 );
	imshow( "Final Drawing", finaldrawing);
#endif


	/*Final display - of leaf and holes
	 * on to a window
	 */
	for(int i = 0; i < (int)lcontours.size() - 2; i++){
	   Scalar color = Scalar( 255, 255, 255);
	   drawContours( drawing, lcontours, i, color, 2, 8, lhierarchy, 0, Point() );
	}
	namedWindow( "Contours", 0 );
	imshow( "Contours", drawing);
	waitKey(0);

	return 0;
}
