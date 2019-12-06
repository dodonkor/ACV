/* 

countingObjectsApplication.cpp - implementation file for a program named example
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

Derrick Odonkor
01/10/2019



*/

#include "countingObjects.h"


// Global variables to allow access by the display window callback functions

Mat inputImage;
int thresholdValue            =55; // default threshold 49
//char* thresholded_window_name = "Thresholded Image";
//char* components_window_name  = "Connected Components";
int half_structuring_element_size  = 5;   // default size of structuring element divided by two: structuring element size = value * 2 + 1


int main() {
	int end_of_file;
	bool debug = true;
	char filename[MAX_FILENAME_LENGTH];

	int const max_threshold     = 255; 
	int num_contours = 0;

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
	int counter = 0;
	do {

		end_of_file = fscanf(fp_in, "%s", filename); // Read from input file
		if (end_of_file != EOF) {
			counter++;
			
			
			inputImage = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
			if(inputImage.empty()) {
				cout << "can not open " << filename << endl;
				prompt_and_exit(-1);
			}

			// Calculate the number of objects in the image
			num_contours = countingObjects(0, 0);


			// Print to file
			print_to_file(num_contours,filename, *fp_out);

		}
	} while (end_of_file != EOF);

	

	// Close all opened file
	fclose(fp_in);
	fclose(fp_out);

	return 0;
}
