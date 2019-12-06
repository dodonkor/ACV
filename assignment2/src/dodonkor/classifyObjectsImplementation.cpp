/* 

classifyObjectsImplementation.cpp - implementation file for a program named classifyObjects
============================================================================

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

#include "classifyObjects.h"
#define PI 3.14159265
extern Mat inputImage; 

/**This function classifies number of objects in an image based on the color and shape using the hu_moments 
This function returns a string i.e the different classification and the number of object in that classification
**/
string classifyObjects(int, void*) {  

	//Global variables
	extern int thresholdValue; 
	extern char* thresholded_window_name;
	extern char* components_window_name;
	extern char* contours_window_name;

	//Image variables 
	Mat greyscaleImage;
	Mat thresholdedImage; 
	Mat binary_image;
	Mat filtered_image;
	Mat hueImage;   
	Mat saturationImage;
	Mat intensityImage;

	vector<RotatedRect> min_bounding_rectangle;		// bounding rectangles
	vector<dataValue> datavalues;					// Vector array of datavalue
	vector<vector<Point>> contours;					// Vector of vector array of points
	vector<vector<Point>> hu_contours;				// Vector of vector array of points
	vector<Vec4i> hierarchy;

	//Other declarations
	int row;
	int col;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	float hue;
	float saturation;
	float intensity;

	int redObjectCount[3] = {0,0,0};
	int blackObjectCount[3] = {0,0,0};
	double hu_moments[7];

	if (thresholdValue < 1)  // the trackbar has a lower value of 0 which is invalid
		thresholdValue = 1;

	if (inputImage.type() == CV_8UC3) { // colour image
		cvtColor(inputImage, greyscaleImage, CV_BGR2GRAY);
		hueImage.create(inputImage.size(), CV_8UC1);
		saturationImage.create(inputImage.size(), CV_8UC1);
		intensityImage.create(inputImage.size(), CV_8UC1);
	} 
	else {
		hueImage = inputImage.clone();
		greyscaleImage = inputImage.clone();
		intensityImage = inputImage.clone();
		saturationImage = inputImage.clone();
	}

	// Convert image to HSI to be able to extract the saturationed images
	for (row=0; row < inputImage.rows; row++) {
		for (col=0; col < inputImage.cols; col++) {
			blue  = inputImage.at<Vec3b>(row,col)[0];
			green = inputImage.at<Vec3b>(row,col)[1];
			red   = inputImage.at<Vec3b>(row,col)[2];

			rgb2hsi(red, green, blue, &hue, &saturation, &intensity);

			hueImage.at<uchar>(row,col)        = (char)  (255.0  * (hue/360.0));
			saturationImage.at<uchar>(row,col) = (char)  (saturation * 255);
			intensityImage.at<uchar>(row,col)  = (char)  (intensity * 255);

		}
	}


	/* perform binary thresholding */
	binary_image.create(greyscaleImage.size(), CV_8UC1);


	//Applying the threshold to the greyscale image
	threshold(greyscaleImage,binary_image,thresholdValue, 255,THRESH_BINARY_INV | THRESH_OTSU);

	// create a 3*3 structuring element	
	Mat structuring_element(3,3,CV_8U,Scalar(1)); 

	//Dilate the resulting image with a structuring element
	dilate(binary_image,filtered_image,structuring_element);  

	//imshow(components_window_name, filtered_image); 

	//Find the contours in the binary image
	findContours(filtered_image,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	vector<Moments>       contour_moments(contours.size());          // Hu moments

	//Fonts setup
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.6f, 0.6f, 0, 2);


	//Extract the saturation and shape(using hu_moments values of the contours
	Mat contours_image = Mat::zeros(inputImage.size(), CV_8UC3);
	for (int contour_number=0; (contour_number<(int)contours.size()); contour_number++)
	{

		Scalar colour( rand()&0xFF, rand()&0xFF, rand()&0xFF );

		if (contours[contour_number].size() > 100) { // only consider contours of appreciable length
			min_bounding_rectangle.push_back( minAreaRect(contours[contour_number]));

			//Calculate the bounding box of the contours and get the centroid
			Point2f bounding_rect_points[4];
			min_bounding_rectangle[min_bounding_rectangle.size()-1].points(bounding_rect_points);
			line(contours_image, bounding_rect_points[0], bounding_rect_points[1], Scalar(0, 0, 127));
			line(contours_image, bounding_rect_points[1], bounding_rect_points[2], Scalar(0, 0, 127));
			line(contours_image, bounding_rect_points[2], bounding_rect_points[3], Scalar(0, 0, 127));
			line(contours_image, bounding_rect_points[3], bounding_rect_points[0], Scalar(0, 0, 127));
			line(contours_image, min_bounding_rectangle[min_bounding_rectangle.size()-1].center, min_bounding_rectangle[min_bounding_rectangle.size()-1].center, Scalar(0, 255, 127));

			//Calculate moments of the contours
			contour_moments[contour_number] = moments( contours[contour_number] );  

			// get the hu_moments of the contours
			HuMoments(contour_moments[contour_number], hu_moments ); 

			// Extract the saturation values
			int saturation = saturationImage.at<uchar>(min_bounding_rectangle[min_bounding_rectangle.size()-1].center.y,min_bounding_rectangle[min_bounding_rectangle.size()-1].center.x);

			if(saturation >20 && saturation < 98){ //Red color

				if((hu_moments[0] > 0.19) && (hu_moments[0] < 0.22)) //Cross
				{
					redObjectCount[0]++;
				}else if((hu_moments[0] > 0.23) && (hu_moments[0] < 0.45))//linear
				{
					redObjectCount[1]++;
				}else if((hu_moments[0] > 0.14) && (hu_moments[0] < 0.16))//round
				{
					redObjectCount[2]++;
				}

			}else if(saturation >= 0 && saturation < 12){//Black Color

				if((hu_moments[0] > 0.19) && (hu_moments[0] < 0.22)) //Cross
				{
					blackObjectCount[0]++;
				}else if((hu_moments[0] > 0.23) && (hu_moments[0] < 0.45))//linear
				{
					blackObjectCount[1]++;
				}else if((hu_moments[0] > 0.14) && (hu_moments[0] < 0.16))//round
				{
					blackObjectCount[2]++;
				}
			}



			/*
			// declaring output string stream 
			ostringstream str1; 

			// Sending a number as a stream into output 			
			str1 << hu_moments[0] << " " << saturation ; 

			string geek = str1.str(); 

			putText(contours_image,geek, min_bounding_rectangle[min_bounding_rectangle.size()-1].center, FONT_HERSHEY_PLAIN, 0.8, Scalar(255, 255, 255), 1);

			// Storing all values in a struct obj of dataValue
			dataValue dataobj = {min_bounding_rectangle[min_bounding_rectangle.size()-1].center.x, min_bounding_rectangle[min_bounding_rectangle.size()-1].center.y,hue, saturation};
			datavalues.push_back(dataobj);*/

		}
	}



	//Display in window
	//imshow(contours_window_name, contours_image);   		

	// declaring output string stream 
	ostringstream text; 

	// Sending a number as a stream into output 
	text << "There are RO" << redObjectCount[2] <<" RX "<<redObjectCount[0]<<" R| "<<redObjectCount[1]<<" BO "<<blackObjectCount[2]<<" BX "<<blackObjectCount[0]<<" B| "<<blackObjectCount[1]<<"\n" ; 

	// the str() coverts number into string 
	string text_output = text.str(); 


	return text_output;


}

//Print to console
void print_obj(int red[], int black[])
{	

	printf("There are RO %d RX %d R| %d BO %d BX %d B| %d\n", red[2], red[0], red[1], black[2], black[0], black[1]); 

}



// -----------------------------------------------------------------------------------------------
// rgb2hsi
//
// convert an RGB triple to a HSI triple
//
// The transform is based on "The Taming of the Hue, Saturation and Brightness Colour Space", Allan Hanbury, Proc. CVWW, [Hanbury02]
// 
// -----------------------------------------------------------------------------------------------

void rgb2hsi(unsigned char red, unsigned char green, unsigned char blue, float *hue, float *saturation, float *intensity){

	double y, h, h_star, c, c1, c2,  s, r, g, b; 
	int min =256;

	//  0 <= hue <= 2 pi
	//  0 <= saturation <= 1

	r = (float) red   / 256;
	g = (float) green / 256;
	b = (float) blue  / 256;

	y  = 0.2125 * r + 0.7154 * g + 0.0721 * b;
	c1 =          r - 0.5    * g - 0.5    * b;
	c2 =            - 0.8660 * g + 0.8660 * b;


	// chroma c: [0,1]

	c = sqrt(c1*c1 + c2*c2);


	// hue h: [0,360]

	if (c == 0) { // h and s are undefined
		*hue        = (float) 0;
		*saturation = (float) 0;
	}
	else {
		if(c2 <= 0) {
			h = acos (c1/c);
		}
		else {
			h = 2*3.14159  - acos (c1/c);
		}

		h = 360 * (h / (2 * 3.14159)); // convert to degrees


		// saturation: [0,1]

		h_star =  (int) h - (int) (60 * (  ((int) h) / 60));  // convert to interval 0,60


		s = (2 * c * sin( 2 * 3.14159 * ((120 - h_star) / 360.0))) / 1.73205;


		//*hue        = (float)  ((h / 360) * 2 * 3.14159); // convert to radians ... for the moment anyway
		*hue        = (float)  h;  
		*saturation = (float)  s;
	}

	*intensity  = (float)  (r+g+b)/3;

	// printf("rgb2hsi: (%d, %d, %d) -> (%3.1f, %3.1f, %3.1f)\n", red, green, blue, *hue, *saturation, *intensity);

}

/*===================================================*/
/* display images from a camera in an openCV window  */
/* pass the index of the camera as a parameter       */
/*===================================================*/

string display_image_from_camera(int cameraNum) {

	VideoCapture camera;            //  the camera device
	Mat frame;					        //  save an image read from a camera
	vector<int> compressionParams;  // parameters for image write
	string temp_text = "";
	char windowName[MAX_STRING_LENGTH]; 
	char cameraNumber[MAX_STRING_LENGTH]; 

	strcpy(windowName,"Camera");
	sprintf(cameraNumber, " %d", cameraNum);

	namedWindow(windowName,     CV_WINDOW_AUTOSIZE); // create the window  

	if (camera.open(cameraNum) == true) {          // open the camera input 

		printf("Press any key to stop image display\n");

		camera >> frame;                             // read a frame from the camera to get the image size ... this is actually C++


		do {
			camera >> frame;                          // read a frame from the camera                    
			imshow(windowName, frame);
			cvWaitKey(30); // this is essential as it allows openCV to handle the display event ... 
			// the argument is the number of milliseconds to wait 
		} while (!_kbhit());

		getchar(); // flush the buffer from the keyboard hit

		compressionParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compressionParams.push_back(9);                                  // 9 implies maximum compression

		imwrite("../data/camera_image.png", frame, compressionParams);   // write the image to a file just for fun

		string temp_file = "../data/camera_image.png"; //Get the image taken 
		inputImage = imread(temp_file, CV_LOAD_IMAGE_UNCHANGED); //Loadin the image

		temp_text = classifyObjects(0, 0); // Classify image from camera
		destroyWindow(windowName);   
	}
	else {
		printf("Failed to open camera %d\n",cameraNum);
		prompt_and_continue();
	}

	return temp_text;
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

void prompt_and_continue() {
	printf("Press any key to continue ... \n");
	getchar();
}