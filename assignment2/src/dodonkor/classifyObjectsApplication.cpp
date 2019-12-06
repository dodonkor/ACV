/* 

	classifyObjectsApplication.cpp - implementation file for a program named classifyObjects
	============================================================================

	This program is for course 18-799 Applied Cognitive Vision, Assignment No. 2 

	The assignment is defined as follows.

	The goal of this assignment is to develop an openCV computer vision application that classifies objects
	based on the color and shape. 

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

	../data/assignment2A.png
	../data/assignment2B.png
	../data/isolated1.png
	../data/isolated3.png
	../data/isolated5.png
	../data/isolated7.png
	../data/isolated9.png
	../data/isolated11.png
	../data/isolated13.png
	../data/isolated15.png
	../data/isolated17.png
	../data/test1.png
	../data/test2.png
	../data/test3.png
	../data/test4.png
	../data/test5.png
	../data/test6.png
	../data/test7.png
	../data/test8.png
	../data/test9.png
	../data/test10.png
	camera
	

	Sample Output
	-------------

	dodonkor
	../data/assignment2A.png: There are RO0 RX 1 R| 0 BO 1 BX 0 B| 2
	../data/assignment2B.png: There are RO0 RX 2 R| 1 BO 1 BX 0 B| 3
	../data/isolated1.png: There are RO0 RX 0 R| 0 BO 1 BX 0 B| 0
	../data/isolated3.png: There are RO0 RX 0 R| 0 BO 2 BX 0 B| 2
	../data/isolated5.png: There are RO0 RX 2 R| 0 BO 0 BX 0 B| 0
	../data/isolated7.png: There are RO0 RX 1 R| 0 BO 0 BX 0 B| 1
	../data/isolated9.png: There are RO0 RX 1 R| 0 BO 1 BX 0 B| 2
	../data/isolated11.png: There are RO0 RX 1 R| 1 BO 1 BX 0 B| 3
	../data/isolated13.png: There are RO0 RX 3 R| 2 BO 1 BX 0 B| 3
	../data/isolated15.png: There are RO0 RX 3 R| 2 BO 1 BX 0 B| 5
	../data/isolated17.png: There are RO0 RX 4 R| 2 BO 2 BX 0 B| 5
	../data/test1.png: There are RO0 RX 4 R| 2 BO 0 BX 0 B| 0
	../data/test2.png: There are RO0 RX 4 R| 0 BO 0 BX 0 B| 3
	../data/test3.png: There are RO0 RX 4 R| 0 BO 1 BX 0 B| 5
	../data/test4.png: There are RO0 RX 1 R| 1 BO 1 BX 0 B| 2
	../data/test5.png: There are RO0 RX 0 R| 0 BO 1 BX 0 B| 2
	../data/test6.png: There are RO0 RX 1 R| 0 BO 1 BX 0 B| 1
	../data/test7.png: There are RO0 RX 1 R| 1 BO 0 BX 0 B| 2
	../data/test8.png: There are RO0 RX 1 R| 1 BO 0 BX 0 B| 2
	../data/test9.png: There are RO0 RX 0 R| 2 BO 0 BX 0 B| 1
	../data/test10.png: There are RO0 RX 4 R| 2 BO 2 BX 0 B| 6
	camera: There are RO0 RX 1 R| 0 BO 0 BX 0 B| 0


	Solution Strategy
	-----------------

	The strategy used is as follows.
	First read in the images using the input file.
	Convert the image to a greysclae image
	Create a binary image out of the greyscale image using the Otsu thresholding technique
	Using the dilation approach apply a morphological operation with a structuring element
	Find the contours in the resulting image and compute the centroid of each contour using the bounding box
	Use the centroid to find the saturation of the contour i.e the object
	Calculate the hu_moments of the contours which will be used to classify the shapes.
	Using the saturation and the hu_moments, classify each objects in the image.
	Write the results to a file.

	Test Strategy
	--------------
	Images were taken using the camera to test the application.

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
	-----------
	Derrick Odonkor
	24/10/2019


	Audit Trail
	-----------


*/

#include "classifyObjects.h"


// Global variables to allow access by the display window callback functions

Mat inputImage;
int thresholdValue            =55; // default threshold 49
char* thresholded_window_name = "Original Hue image";
char* components_window_name  = "Dilated Hue Image";
char* contours_window_name  = "Contours";


int main() {
	int end_of_file;
	bool debug = true;
	char filename[MAX_FILENAME_LENGTH];

	int const max_threshold     = 255; 
	string text_output = "";

	FILE *fp_in, *fp_out;


	// Test the input file to see if exists
	if ((fp_in = fopen("../data/input.txt","r")) == 0) {
		printf("Error can't open input file input.txt\n");
		prompt_and_exit(1);
	}

	// Test the output file to see if exists
	if ((fp_out = fopen("../data/output.txt","w")) == 0) {
		printf("Error can't open output output.txt\n");
		prompt_and_exit(1);
	}

	// Print my andrew ID to the top of the file
	fprintf(fp_out,"dodonkor\n"); 

	end_of_file = fscanf(fp_in, "%s", filename); // Read from input file
	while (end_of_file != EOF) {


		if (end_of_file != EOF) {

			if(strcmp(filename, "camera") != 0){

				inputImage = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
				if(inputImage.empty()) {
					cout << "can not open " << filename << endl;
					prompt_and_exit(-1);
				}

				// Classify the number of objects in the image
				text_output = classifyObjects(0, 0);
			}else if (strcmp(filename, "camera") == 0){

				printf("\nDisplaying image from USB camera ... \n");
				text_output = display_image_from_camera(1);

			}


		}

		// Print to file
		print_to_file(text_output,filename, *fp_out);
		
		end_of_file = fscanf(fp_in, "%s", filename); // Read from input file

	} 


	// Close all opened file
	fclose(fp_in);
	fclose(fp_out);

	return 0;
}
