#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);


/*### the range of sizes that work for detecting a face when I am sitting at the laptop. ###*
 *### Note: it's pretty liberal, with about ~75px padding on either side				 ###*/
Size min_face_size = Size (250, 250);
Size max_face_size = Size (400, 400);


/* Function: detect_faces
 * ----------------------
 * given b/w frame, histogram-normalized, this function will return a vector of 'face' rectangles
 */
vector<Rect> detect_faces(Mat frame_bw){
  vector<Rect> faces;
  Size min_size = Size(280, 280);
  Size max_size = Size(400, 400);
  face_cascade.detectMultiScale(frame_bw, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, min_face_size, max_face_size);
  return faces;
}





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

	/*### Step 2: load the face cascade ###*/
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };

	/*### Step 3: read the video stream ###*/
	Mat frame_bw;
	while (true) {

		Mat frame;
		webcam >> frame;

		/*### get the frame in grayscale, high contrast ###*/
		cvtColor (frame, frame_bw, CV_BGR2GRAY);
		equalizeHist(frame_bw, frame_bw);

		/*### get all of the faces ###*/
		vector<Rect> faces = detect_faces (frame_bw);


		/*### draw them on the display frame ###*/
  		for( int i = 0; i < faces.size(); i++ ) {
    		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    		cout << "face width, height = " << faces[i].width << ", " << faces[i].height << endl;
    		ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    	}


		imshow ("DISPLAY", frame);



		int key = waitKey (30);
		if (key == 'q') break;
	}
	return 0;
}
