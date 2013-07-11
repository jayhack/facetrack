#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

#include "face.h"

using namespace std;
using namespace cv;




/* Function: main_function
 * ----------------------
 * given b/w frame, histogram-normalized, this function will return a vector of 'face' rectangles
 */
int main( int argc, const char** argv )
{

	/*### Step 1: set up the webcam ###*/
	VideoCapture webcam (0);
	if (!webcam.isOpened ()) return -1;

	/*### Step 2: set up the display ###*/
	namedWindow ("DISPLAY", CV_WINDOW_AUTOSIZE);

	/*### Step 3: set up the user's face ###*/
	Face *face = NULL;

	/*### Step 4: read the video stream ###*/
	while (true) {

		Mat frame;
		webcam >> frame;

		if (face == NULL) 
			face = new Face (frame);

		face->detect (frame);


    	/*### draw the face on the display frame ###*/
    	if (face->exists ()) {	
    		cout << "---------- DRAWING EXISTING FACE ------------" << endl;	
    		face->print_info ();
	    	circle( frame, face->get_center(), face->get_boundary().width/2, Scalar( 0, 0, 255 ), 4, 8, 0);
	    }



	    imshow ("DISPLAY", frame);
		int key = waitKey (30);
		if (key == 'q') break;
	}
	return 0;
}
