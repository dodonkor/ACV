/* 

	superPosition.h - interface file for a program named superPosition
	======================================================
	This program is for course 18-799 Applied Cognitive Vision, Assignment No. 4 

	Please refer to the interface file, superPosition.h, for details of the functionality of this program


	File organization
	-----------------

	superPosition.h                  interface file:      contains the declarations required to use the functions that implement the solution to this problem
	typically, these will include the definitions of the abstract data types used in the implementation

	superPositionImplementation.cpp  implementation file: contains the definitions of the functions that implement the algorithms used in the implementation

	superPositionApplication.cpp     application file:    contains the code that instantiates the abstract data types and calls the associated functions
	in order to effect the required functionality for this application

	Author
	------
	David Vernon

	Modified by
	------------

	Derrick Odonkor
	04/12/2019

*/


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <conio.h>

//opencv
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>      // required for SIFT


using namespace std;
using namespace cv;

#define TRUE  1
#define FALSE 0
#define MAX_STRING_LENGTH 80
#define MAX_FILENAME_LENGTH 80
#define HAAR_FACE_CASCADE_INDEX 0

void faceDetection(char *filename, CascadeClassifier& cascade); 
void performGrabCut(Mat input_img, Rect rect);

void prompt_and_exit(int status);
void print_to_file(string &output_text, char filename[MAX_FILENAME_LENGTH], FILE &fp_out);