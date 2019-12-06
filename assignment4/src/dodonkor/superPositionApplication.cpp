/* 

superPositionApplication.cpp - implementation file for a program named objDetectionLocal
============================================================================

This program is for course 18-799 Applied Cognitive Vision, Assignment No. 4 

The assignment is defined as follows.

The goal of this assignment is to develop an OpenCV application using SIFT that can detect
the presence of known objects and determine their positions.

Since this directory is a sibling directory of the bin directory where the dodonkor.exe file resides, 
the filename used when opening the file is "../data/input.txt".

Output for the test case is written to an output file "../data/output.txt"


Input
-----

- An input.txt file with the background image and camera


Output
------

- Out window with superimposed image


Sample Input
------------

a.jpg
camera



Solution Strategy
-----------------

First read in the both the background image, save it and then read from the camera.
perform a face detection on the input from the camera.
If there is a face resize the face x, y, width and height so that we can grab a bigger segment
Check to be sure if the resized face is not beyond the size of the input image.
If it is within bounds draw the rectangle on the detected face, crop the image to the size of the face, 
create a reduced face and perform grabcut with the cropped image and the reduced face size.
After performing grabcut resize the output to a 100 x 100 image and copy the pixels other than 255 to the background image.

Test Strategy
--------------
Used different iteration counts for grabcut

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
-----------
Derrick Odonkor
04/12/2019


Audit Trail
-----------


*/

#include "superPosition.h"

Mat inputImage;
Mat background;

int main() {

   int end_of_file;
   bool debug = true;
   char filename[MAX_FILENAME_LENGTH];

   FILE *fp_in;
   
   if ((fp_in = fopen("../data/input.txt","r")) == 0) {
	  printf("Error can't open input input.txt\n");
     prompt_and_exit(1);
   }

   printf("Example of how to use openCV to perform face detection using Haar features and boosed classification.\n\n");
   
   /* ---------------------------------------------------------------------------------------------
    * This code is provided as part of "A Practical Introduction to Computer Vision with OpenCV"
    * by Kenneth Dawson-Howe © Wiley & Sons Inc. 2014.  All rights reserved.
    */

   	
   // Load Haar Cascade(s)

   char* file_location = "../data/Media/";

	vector<CascadeClassifier> cascades;
	char* cascade_files[] = { 
		"haarcascades/haarcascade_frontalface_alt.xml" };
	int number_of_cascades = sizeof(cascade_files)/sizeof(cascade_files[0]);
	for (int cascade_file_no=0; (cascade_file_no < number_of_cascades); cascade_file_no++)
	{
		CascadeClassifier cascade;
		string filename(file_location);
		filename.append(cascade_files[cascade_file_no]);
		if( !cascade.load( filename ) )
		{
			cout << "Cannot load cascade file: " << filename << endl;
			return -1;
		}
		else cascades.push_back(cascade);
	}

   /* --------------------------------------------------------------------------------------------- */
	 end_of_file = fscanf(fp_in, "%s", filename);
      
      if (end_of_file != EOF) {          
         printf("\n Background extracted %s \n",filename);
      }

	  background = imread(filename, CV_LOAD_IMAGE_COLOR);

   do {

      end_of_file = fscanf(fp_in, "%s", filename);
      
      if (end_of_file != EOF) {          
         printf("\n Performing face detection using Haar features and boosed classification on %s \n",filename);
         faceDetection(filename, cascades[HAAR_FACE_CASCADE_INDEX]);
      }
   } while (end_of_file != EOF);

   fclose(fp_in);
   
   return 0;
}