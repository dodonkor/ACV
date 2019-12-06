/* 

	classifyObjects.h - interface file for a program named classifyObjects
	======================================================
	This program is for course 18-799 Applied Cognitive Vision, Assignment No. 2 

	Please refer to the interface file, classifyObjects.h, for details of the functionality of this program


	File organization
	-----------------

	classifyObjects.h                  interface file:      contains the declarations required to use the functions that implement the solution to this problem
	typically, these will include the definitions of the abstract data types used in the implementation

	classifyObjectsImplementation.cpp  implementation file: contains the definitions of the functions that implement the algorithms used in the implementation

	classifyObjectsApplication.cpp     application file:    contains the code that instantiates the abstract data types and calls the associated functions
	in order to effect the required functionality for this application

	Author
	------
	David Vernon

	Modified by
	------------

	Derrick Odonkor
	24/10/2019

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

using namespace std;
using namespace cv;

#define TRUE 1
#define FALSE 0
#define MAX_STRING_LENGTH   80
#define MAX_FILENAME_LENGTH 80

//Struct to hold the x and y values of the centroid, the angle(pose), and the hue value
struct dataValue{
	int x;
	int y;
	int hue;
	int saturation;
};

/* function prototypes go here */
string classifyObjects(int, void*); 
void print_to_file(string &output_text, char filename[MAX_FILENAME_LENGTH], FILE &fp_out);
void rgb2hsi(unsigned char red, unsigned char green, unsigned char blue, float *hue, float *saturation, float *intensity);
void print_obj(int red[], int black[]);
string display_image_from_camera(int camera_number);
void prompt_and_exit(int status);
void prompt_and_continue();