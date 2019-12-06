/* 

objDetectionLocalApplication.cpp - implementation file for a program named objDetectionLocal
============================================================================

This program is for course 18-799 Applied Cognitive Vision, Assignment No. 3 

The assignment is defined as follows.

The goal of this assignment is to develop an OpenCV application using SIFT that can detect
the presence of known objects and determine their positions.

Since this directory is a sibling directory of the bin directory where the dodonkor.exe file resides, 
the filename used when opening the file is "../data/input.txt".

Output for the test case is written to an output file "../data/output.txt"


Input
-----

- An input.txt file with the image locations


Output
------

- output.txt file containing the andrew ID, and the list of the images paths with their respective centriod points.


Sample Input
------------

Test1
../data/scene.pgm
../data/basmati.pgm
../data/book.pgm
../data/box.pgm
Test2
../data/handrr.bmp (Scene)
../data/mug.jpg


Sample Output
-------------

Results1
dodonkor
../data/basmati.pgm: 322 249
../data/book.pgm: 264 133
../data/box.pgm: 187 224
Results2
dodonkor
../data/mug.jpg: 192 189



Solution Strategy
-----------------

First read in the both the scene and the image on the object to be detected.
Convert both images to a grayscale images.
Import the library required for using SIFT.
Find the SIFT features, extract the feature descriptors, and match the descriptors.
Generate a mask for the matches so that we only draw the ones with a match distance less than the threshold.
Using the minimum distance we compute the required threshold.
Extract the keypoints from the best matches for both the scene and the object image
Using the keypoints of both images, we find a matrix to warp the object onto the scene image. 
To draw the warped image, we find contours of the warp image and draw that image on the scene image.
We can then find the centroid from the contours.
Finally we display the scene image with the drawn contour.
Write the results to a file.

Test Strategy
--------------
Use individual images from another image with all images

File organization
-----------------

objDetectionLocal.h                  interface file:      contains the declarations required to use the functions that implement the solution to this problem
typically, these will include the definitions of the abstract data types used in the implementation

objDetectionLocalImplementation.cpp  implementation file: contains the definitions of the functions that implement the algorithms used in the implementation

objDetectionLocalApplication.cpp     application file:    contains the code that instantiates the abstract data types and calls the associated functions
in order to effect the required functionality for this application


Author
------
David Vernon


Modified by
-----------
Derrick Odonkor
15/11/2019


Audit Trail
-----------


*/

#include "objDetectionLocal.h"



int main() {

	int end_of_file;
	bool debug = true;
	char scene_filename[MAX_FILENAME_LENGTH];
	char obj_filename[MAX_FILENAME_LENGTH];
	string text_output = "";
	dataValue				data_obj;

	FILE *fp_in, *fp_out; 

	printf("Example use of openCV to match SIFT interest point features.\n\n");

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

	end_of_file = fscanf(fp_in, "%s", scene_filename);

	data_obj.sceneImage = imread(scene_filename, CV_LOAD_IMAGE_COLOR); //Get the scene image
	if(data_obj.sceneImage.empty()) {
		cout << "can not open " << scene_filename << endl;
		return -1;
	}

	while (end_of_file != EOF) {

		end_of_file = fscanf(fp_in, "%s", obj_filename);

		if (end_of_file != EOF) { 

			printf("\nPerforming SIFT feature matching on %s and %s \n",scene_filename,obj_filename);
			data_obj = siftMatching(obj_filename, data_obj.sceneImage); 


			print_to_file(data_obj.text,obj_filename, *fp_out);
		}

	} 

	/* do {
	waitKey(30);              
	} while (!_kbhit());                           

	getchar(); // flush the buffer from the keyboard hit

	*/
	fclose(fp_in); 
	fclose(fp_out); 

	return 0;
}
