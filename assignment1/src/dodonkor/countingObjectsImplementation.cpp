/* 

countingObjectsImplementation.cpp - implementation file for a program named example
============================================================================

This program is for course 18-799 Applied Cognitive Vision, Assignment No. 1 

Please refer to the interface file, countingObjects.h, for details of the functionality of this program


File organization
-----------------

countingObjects.h                  interface file:      contains the declarations required to use the functions that implement the solution to this problem
typically, these will include the definitions of the abstract data types used in the implementation

countingObjectsImplementation.cpp  implementation file: contains the definitions of the functions that implement the algorithms used in the implementation

countingObjectsApplication.cpp     application file:    contains the code that instantiates the abstract data types and calls the associated functions
in order to effect the required functionality for this application


Author
------

David Vernon
Derrick Odonkor
01/10/2019



*/

#include "countingObjects.h"

/**This function finds number of objects in an image using morphological operations on that image using a structuring element. 
	Find the contours in that image.

	This function returns an int i.e the number of contours
**/
int countingObjects(int, void*) {  

	//Global variables
	extern Mat inputImage; 
	extern int thresholdValue; 
	extern char* thresholded_window_name;
	extern char* components_window_name;
	extern int half_structuring_element_size; 

	//Image variables 
	Mat greyscaleImage;
	Mat thresholdedImage; 
	Mat binary_image;
	Mat filtered_image; 


	vector<vector<Point>> contours;	// Vector of vector array of points
	vector<Vec4i> hierarchy;


	if (thresholdValue < 1)  // the trackbar has a lower value of 0 which is invalid
		thresholdValue = 1;

	if (inputImage.type() == CV_8UC3) { // colour image
		cvtColor(inputImage, greyscaleImage, CV_BGR2GRAY);
	} 
	else {
		greyscaleImage = inputImage.clone();
	}

	/* perform binary thresholding */
	binary_image.create(greyscaleImage.size(), CV_8UC1);


	//Applying the threshold to the greyscale image
	threshold(greyscaleImage,binary_image,thresholdValue, 255,THRESH_BINARY_INV | THRESH_OTSU);


	int structuring_element_size;
	structuring_element_size = half_structuring_element_size * 2 + 1;
	Mat structuring_element(structuring_element_size,structuring_element_size,CV_8U,Scalar(1)); // create structuring element
	Mat structuring_element_sec(3,3,CV_8U,Scalar(1)); // create a 3*3 structuring element	


	erode(binary_image,filtered_image,structuring_element); //Erode the binary image with the first structuring element
	morphologyEx(filtered_image,filtered_image,MORPH_OPEN,structuring_element); //Apply a morphological opening to the results using the same structuring element
	dilate(filtered_image,filtered_image,structuring_element_sec); //Dilate the resulting image with a second structuring element 

	//Find the contours in the filtered image
	findContours(filtered_image,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);

	//imshow(components_window_name, contours_image);   		
	//printf("Number of objects in image %d\n", (int)contours.size());

	//Return the number of contours which represents the number of objects in the image.
	return (int)contours.size();


}





/**
Function to print the results in to a file
Takes in an int i.e number of contours, the filename of the images, and the output file
as parameters.
**/
void print_to_file(int &num, char filename[MAX_FILENAME_LENGTH], FILE &fp_out)
{	
	fprintf(&fp_out,"%25s: %3d objects\n", filename, num); 
}


void prompt_and_exit(int status) {
	printf("Press any key to continue and close terminal ... \n");
	getchar();
	exit(status);
}

void prompt_and_continue() {
	printf("Press any key to continue ... \n");
	getchar();
}