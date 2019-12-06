/* 

	countingObjects.h - interface file for a program named example
	======================================================

	This program is for course 18-799 Applied Cognitive Vision, Assignment No. 1 

	The assignment is defined as follows.

	The goal of this assignment is to develop an openCV computer vision application that determines the number
	of objects in an image placed on a flat surface. 

	Since this directory is a sibling directory of the bin directory where the dodonkor.exe file resides, 
	the filename used when opening the file is "../data/input.txt".

	Output for the test case is written to an output file "../data/output.txt"


	Input
	-----

	- An input.txt file with the image locations


	Output
	------

	- output.txt file containing the andrew ID, and the list of the images paths with their respective number of objects.


	Sample Input
	------------

	../data/assignment1A.png
	../data/assignment1B.png
	../data/isolated1.png
	../data/isolated3.png
	../data/isolated5.png
	../data/isolated7.png
	../data/isolated9.png
	../data/isolated11.png
	../data/isolated13.png
	../data/isolated15.png
	../data/isolated17.png
	../data/touching1.png
	../data/touching3.png
	../data/touching5.png
	../data/touching7.png
	../data/rotating1.png
	../data/rotating2.png
	../data/rotating3.png
	

	Sample Output
	-------------

	dodonkor
 ../data/assignment1A.png:   7 objects
 ../data/assignment1B.png:  13 objects
    ../data/isolated1.png:   1 objects
    ../data/isolated3.png:   4 objects
    ../data/isolated5.png:   2 objects
    ../data/isolated7.png:   2 objects
    ../data/isolated9.png:   4 objects
   ../data/isolated11.png:   6 objects
   ../data/isolated13.png:   9 objects
   ../data/isolated15.png:  11 objects
   ../data/isolated17.png:  13 objects
    ../data/touching1.png:   4 objects
    ../data/touching3.png:   4 objects
    ../data/touching5.png:  13 objects
    ../data/touching7.png:  13 objects
    ../data/rotating1.png:   4 objects
    ../data/rotating2.png:   4 objects
    ../data/rotating3.png:  13 objects


	Solution Strategy
	-----------------

	The strategy used is as follows.
	First read in the images using the input file.
	Convert the image to a greysclae image
	Create a binary image out of the greyscale image using the Otsu thresholding technique
	Define two structuring element, use one for Eroding the binary image.
	Apply morphological openning to the resulting image with the same structuring element
	Dilate the resulting image with the second structuring element
	Find the contours in the resulting image and return the results as the number of objects in the image.
	Write the results to a file.

	Test Strategy
	--------------

	All testcases were provided. Some were rotated for testing.

	File organization
	-----------------

	countingObjects.h                  interface file:      contains the declarations required to use the functions that implement the solution to this problem
	typically, these will include the definitions of the abstract data types used in the implementation

	countingObjectsImplementation.cpp  implementation file: contains the definitions of the functions that implement the algorithms used in the implementation

	countingObjectsApplication.cpp     application file:    contains the code that instantiates the abstract data types and calls the associated functions
	in order to effect the required functionality for this application


	Author
	------

	Derrick Odonkor
	David Vernon
	01/10/2019


	Audit Trail
	-----------



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



/* function prototypes go here */
int countingObjects(int, void*); 
void print_to_file(int &num, char filename[MAX_FILENAME_LENGTH], FILE &fp_out);

void prompt_and_exit(int status);
