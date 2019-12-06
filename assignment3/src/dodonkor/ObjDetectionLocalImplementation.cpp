/* 

objDetectionLocalImplementation.cpp - implementation file for a program named objDetectionLocal
============================================================================

This program is for course 18-799 Applied Cognitive Vision, Assignment No. 3 

Please refer to the interface file, objDetectionLocal.h, for details of the functionality of this program


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
------------

Derrick Odonkor
15/11/2019



*/

#include "objDetectionLocal.h"



dataValue siftMatching(char *filename1, Mat filename2) {  
	
	char*                   SIFT_window_name   = "SIFT Descriptor Matches"; 
	Mat                     input_image1; 
	Mat                     input_image2; 
	Mat                     greyscale_image1;
	Mat                     greyscale_image2;
	Mat                     greyscale_image3;
	Mat                     descriptors1;
	Mat                     descriptors2;
	Mat                     sift_matches;
	Mat						binary_image;
	Mat                     temp;
	Mat                     results;
	vector<KeyPoint>        keypoints1;            // keypoints image 1
	vector<KeyPoint>        keypoints2;            // keypoints image 2
	SiftFeatureDetector     sift_detector;         // SIFT features
	SiftDescriptorExtractor sift_extractor;        // SIFT descriptors
	BFMatcher               sift_matcher(NORM_L1); // Brute-force matcher
	vector<DMatch>          matches;               // descriptor matches
	vector<DMatch>          bestMatches;               // descriptor  best matches
	vector<char>            matchMask;
	double                  matchThreshold = 1000;
	unsigned int            i;
	bool                    debug = false;
	double                  min_dist = 10000.0;
	double                  dist;
	dataValue				data_obj;

	vector<RotatedRect> min_bounding_rectangle;		// bounding rectangles
	vector<vector<Point>> contours;					// Vector of vector array of points
	vector<Vec4i> hierarchy;



	input_image1 = imread(filename1, CV_LOAD_IMAGE_COLOR); // Was CV_LOAD_IMAGE_UNCHANGED but this caused problems with JoinImagesVertically
	// when the input image was greyscale. David Vernon 14/11/2018
	if (input_image1.empty()) {
		cout << "can not open " << filename1 << endl;
		prompt_and_exit(-1);
	}

	//input_image2 = imread(filename2, CV_LOAD_IMAGE_COLOR); // Was CV_LOAD_IMAGE_UNCHANGED but this caused problems with JoinImagesVertically
	// when the input image was greyscale. David Vernon 14/11/2018
	//if (input_image2.empty()) {
	//	cout << "can not open " << filename2 << endl;
	//	prompt_and_exit(-1);
	//}
	input_image2 = filename2; // Copy the input scene image

	//printf("Press any key to continue ...\n");

	// Create a window for results image
	//namedWindow(SIFT_window_name, CV_WINDOW_AUTOSIZE );


	if (input_image1.type() == CV_8UC3) { // colour image
		cvtColor(input_image1, greyscale_image1, CV_BGR2GRAY); // SIFT operates on grey-scale
	} 
	else {
		greyscale_image1 = input_image1.clone();
	}


	if (input_image2.type() == CV_8UC3) { // colour image
		cvtColor(input_image2, greyscale_image2, CV_BGR2GRAY); // SIFT operates on grey-scale
	} 
	else {
		greyscale_image2 = input_image2.clone();
	}

	
	initModule_nonfree(); // required for SIFT

	/* Find SIFT features */
	sift_detector.detect(greyscale_image1, keypoints1);
	sift_detector.detect(greyscale_image2, keypoints2);

	/* Extract feature descriptors */
	sift_extractor.compute(greyscale_image1, keypoints1, descriptors1);
	sift_extractor.compute(greyscale_image2, keypoints2, descriptors2);

	/* Match descriptors */
	sift_matcher.match(descriptors1, descriptors2, matches);

	/* keypoints1[matches[i].queryIdx] has a corresponding point in keypoints2[matches[i].trainIdx] */
	if (debug) {
		for (i = 0; i < matches.size(); i++){
			printf("%3.1f %3.1f -> %3.1f %3.1f; distance %4.1f \n", keypoints1[matches[i].queryIdx].pt.x, keypoints1[matches[i].queryIdx].pt.y, 
				keypoints2[matches[i].trainIdx].pt.x, keypoints2[matches[i].trainIdx].pt.y,
				matches[i].distance);
		}
	}

	/* generate a mask for the matches so that we only draw the ones with a match distance less than the threshold */

	/* calculation of max and min distances for keypoint matches */
	for (i = 0; i < matches.size(); i++ ) { 
		dist = matches[i].distance;
		if ( dist < min_dist ) min_dist = dist; 
	}

	if (debug) printf("Min dist : %f \n", min_dist );

	matchThreshold = 3.0 * min_dist;

	matchMask.clear();

	//Find the best matches
	for (i = 0; i < matches.size(); i++){
		if (matches[i].distance < matchThreshold) {
			bestMatches.push_back(matches[i]); // true ... draw
		}

	}

	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( int i = 0; i < bestMatches.size(); i++ )
	{
		//-- Get the keypoints from the best matches
		obj.push_back( keypoints1[ bestMatches[i].queryIdx ].pt );
		scene.push_back( keypoints2[ bestMatches[i].trainIdx ].pt );
	}
	Mat mask;
	Mat H = findHomography( obj, scene, CV_RANSAC, 3.0, mask );
	Mat im_out;
	warpPerspective(greyscale_image1, im_out, H, greyscale_image2.size() );

	/* Display SIFT matches */
	//drawMatches(greyscale_image1, keypoints1, greyscale_image2, keypoints2 , matches, sift_matches); // original code
	drawMatches(greyscale_image1, keypoints1, greyscale_image2, keypoints2 , bestMatches, sift_matches, 
		Scalar::all(-1), Scalar::all(-1), mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS); 

	//printf("size of sift_matches %d %d \n", sift_matches.rows, sift_matches.cols);

	binary_image.create(im_out.size(), CV_8UC1);//Create the binary image


	//Applying the threshold to the greyscale image
	threshold(im_out,binary_image, 0, 255,THRESH_BINARY);

	//imshow( "Binary", binary_image );

	//Find the contours in the binary image
	findContours(binary_image,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	

	Mat contour_image = Mat::zeros(im_out.size(), CV_8UC3);
	greyscale_image3 = input_image2.clone();
	
	int X=0;
	int Y=0;

	//Find the best contours and centriods of the shape detected
	for (int contour_number=0; (contour_number<(int)contours.size()); contour_number++) {
		 
		Scalar colour(255,0,0);//Use the blue color
		if(contours[contour_number].size() > 200)
		{		
			
			drawContours( greyscale_image3, contours, contour_number, colour, 2, 8, hierarchy );

			min_bounding_rectangle.push_back( minAreaRect(contours[contour_number]));

			Point2f bounding_rect_points[4];
			min_bounding_rectangle[min_bounding_rectangle.size()-1].points(bounding_rect_points);
			line(contour_image, bounding_rect_points[0], bounding_rect_points[1], Scalar(0, 0, 127));
			line(contour_image, bounding_rect_points[1], bounding_rect_points[2], Scalar(0, 0, 127));
			line(contour_image, bounding_rect_points[2], bounding_rect_points[3], Scalar(0, 0, 127));
			line(contour_image, bounding_rect_points[3], bounding_rect_points[0], Scalar(0, 0, 127));
			line(contour_image, min_bounding_rectangle[min_bounding_rectangle.size()-1].center, min_bounding_rectangle[min_bounding_rectangle.size()-1].center, Scalar(0, 255, 127));

			X = min_bounding_rectangle[min_bounding_rectangle.size()-1].center.x;
			Y = min_bounding_rectangle[min_bounding_rectangle.size()-1].center.y;

		}
	}

	//Display the match on the scene image
	imshow( "Contour", greyscale_image3 );
	
	do {
		waitKey(30);              
	} while (!_kbhit());                           

	getchar(); // flush the buffer from the keyboard hit

	// declaring output string stream 
	ostringstream text; 

	// Sending a number as a stream into output 
	text << X <<" "<<Y <<"\n" ; 

	// the str() coverts number into string 
	string text_output = text.str(); 

	data_obj.text = text_output;
	data_obj.sceneImage = greyscale_image3;

	return data_obj;
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


