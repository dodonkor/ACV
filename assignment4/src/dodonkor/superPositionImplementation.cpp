/* 

superPositionImplementation.cpp - implementation file for a program named superPosition
============================================================================

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

#include "superPosition.h"
extern Mat background;

void faceDetection(char *filename, CascadeClassifier& cascade) {

	VideoCapture camera;
	char inputWindowName[MAX_STRING_LENGTH]         = "Input Image";
	char outputWindowName[MAX_STRING_LENGTH]        = "Cascade of Haar Classifiers";
	char background_window_name[MAX_STRING_LENGTH]  = "Background Image: Running Average Model";
	char foreground_window_name[MAX_STRING_LENGTH]  = "Foreground Image: Running Average Model";
	extern Mat inputImage;
	Mat outputImage;
	char c;
	vector<Rect> faces;
	Rect face;
	Mat gray;


	namedWindow(outputWindowName,     CV_WINDOW_AUTOSIZE); 

	/* check to see if the image is the camera device     */
	/* if so, grab images live from the camera            */
	/* otherwise proceed to process the image in the file */

	if (strcmp(filename,"camera") == 0) {	
		/* --------------------------------------------------------------------------------------------
		* Adapted from code provided as part of "A Practical Introduction to Computer Vision with OpenCV"
		* by Kenneth Dawson-Howe © Wiley & Sons Inc. 2014.  All rights reserved.
		*/

		// Cascade of Haar classifiers (most often shown for face detection).

		//camera.open(1); // David Vernon ... this is the original code and uses an external USB camera
		camera.open(0);   // David Vernon ... use this for the internal web camera

		camera.set(CV_CAP_PROP_FRAME_WIDTH, 320);  // David Vernon ... has no effect on my webcam so resizing below
		camera.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

		if( camera.isOpened() ) { 
			Mat current_frame;

			do {
				camera >> current_frame;
				if( !current_frame.empty() )
					//break;
				{
					// vector<Rect> faces; // David Vernon ... moved to start of function
					// Mat gray;           // David Vernon ... moved to start of function

					//resize(current_frame,current_frame,Size(),0.5,0.5); // David Vernon 

					cvtColor( current_frame, gray, CV_BGR2GRAY );
					equalizeHist( gray, gray ); // David Vernon: irrespective of the equalization, well illumiated images are required
					cascade.detectMultiScale( gray, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );

					/* ----------------------------------Derrick----------------------------------------------- */
					if(!faces.empty())
					{
						face = faces[0];

						//Resize the rectangle by 90%
						Size reSize( face.width * 0.9f, face.height * 0.9f );
						Point relocate( reSize.width/2, reSize.height/2);
						face += reSize; //Resize the rectangle by the delta size
						face -= relocate; //Reposition the rectangle by an offset

						//Check for face boundaries
						if (0 <= face.x
							&& 0 <= face.width
							&& face.x + face.width <= current_frame.cols
							&& 0 <= face.y
							&& 0 <= face.height
							&& face.y + face.height <= current_frame.rows){

								rectangle(current_frame, face, cv::Scalar(255,0,0), 2);

								Mat Roi = current_frame(face); // Get the segment with the face 
								Rect Rec(10, 10, Roi.cols-20, Roi.rows-20); //Reduce the selection size
								performGrabCut(Roi, Rec); // Perform grabcut with new rectangle and reduced image

								imshow(outputWindowName, current_frame );

								/* ---------------------------------- End Derrick----------------------------------------------- */


								c = waitKey(10);   // This makes the image appear on screen ... DV changed from original
						}
					}


				}
				// } while (c == -1); // David Vernon
			} while (!_kbhit()); 
		}
	}
	/* --------------------------------------------------------------------------------------------- */

	do{
		waitKey(30);                                  
	} while (!_kbhit());       

	getchar(); // flush the buffer from the keyboard hit

	destroyWindow(outputWindowName);  
}



/*
* function grabCut
* Trackbar callback - number of iterations user input
*/

void performGrabCut(Mat input_img, Rect rect) {  
	Mat   inputImage = input_img; 
	int   numberOfIterations = 2; 
	char* grabcut_window_name = "GrabCut Image";
	Mat result;          // segmentation result 
	Mat bgModel,fgModel; // the models (hard constraints)



	/* GrabCut segmentation                                                                           */
	/* see: http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html#grabcut */
	grabCut(inputImage,         // input image
		result,             // segmentation result (4 values); can also be used as an input mask providing constraints
		rect,               // rectangle containing foreground 
		bgModel,fgModel,    // for internal use ... allows continuation of iterative solution on subsequent calls
		numberOfIterations, // number of iterations
		GC_INIT_WITH_RECT); // use rectangle

	/* Get the pixels marked as likely foreground */
	compare(result,GC_PR_FGD,result,CMP_EQ);

	/* Generate output image */
	Mat foreground(inputImage.size(),CV_8UC3,cv::Scalar(255,255,255));
	inputImage.copyTo(foreground,result); // use result to mask out the background pixels 


	/* ----------------------------------Derrick----------------------------------------------- */
	Mat b = background.clone();

	//Restrain the segmentation to a 100 by 100 image
	resize(foreground, foreground , Size(100,100) , 0 , 0,INTER_LINEAR );

	//imshow("Forground", foreground);

	//Copy the segmented pixels to the background
	for(int i=0; i < 100; i++)
	{
		for(int j=0; j < 100; j++)
		{
			if (((rect.y + j) <= foreground.cols) && ((rect.x + i) <= foreground.rows)){
				if(foreground.at<Vec3b>(Point(i, j)) != Vec3b(255, 255, 255) )
					b.at<Vec3b>(Point(350+i,100+j)) = foreground.at<Vec3b>(Point(i, j));
			}
			
		}
	}	

	
	imshow("Background", b);
	/* ---------------------------------- End Derrick----------------------------------------------- */
}


/**
Function to print the results in to a file
Takes in an int i.e number of contours, the filename of the images, and the output file
as parameters.
**/
void print_to_file(string &output_text, char filename[MAX_FILENAME_LENGTH], FILE &fp_out)
{	
	fprintf(&fp_out,"%s: %s", filename, output_text.c_str()); 
}


void prompt_and_exit(int status) {
	printf("Press any key to continue and close terminal ... \n");
	getchar();
	exit(status);
}


