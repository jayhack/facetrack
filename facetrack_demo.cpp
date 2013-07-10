#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

#include "face.h"

using namespace std;
using namespace cv;


/** Global variables */
String face_cascade_name = "data/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
RNG rng(12345);


/*### the range of sizes that work for detecting a face when I am sitting at the laptop. ###*
 *### Note: it's pretty liberal, with about ~75px padding on either side				 ###*/
Size min_face_size = Size (250, 250);
Size max_face_size = Size (400, 400);


/* Function: detect_faces
 * ----------------------
 * given b/w frame, histogram-normalized, this function will return a vector of 'face' rectangles
 */
vector<Rect> detect_face_rects(Mat frame_bw){
  vector<Rect> face_rects;
  Size min_size = Size(280, 280);
  Size max_size = Size(400, 400);
  face_cascade.detectMultiScale(frame_bw, face_rects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, min_face_size, max_face_size);
  return face_rects;
}


/* Function: update_faces
 * ----------------------
 * given pointers to vectors of faces and face rects, it will update the face_rects vector.
 */
void update_faces (vector<Face*> *faces, vector<Rect> *face_rects) {

	cout << "----- update faces -----" << endl;
	cout << "	current # of faces: " << faces->size () << endl;
	cout << "	current # of face_rects: " << face_rects->size() << endl;

	/*### for each current face, find the best matching face_rect ###*/
	for (int i=0;i<faces->size ();i++) {
		(*faces)[i]->update (face_rects);
		(*faces)[i]->print_info ();
	}

	/*### for all remaining face_rects, create a new face ###*/
	for (int i=0;i<face_rects->size();i++) {
		Face * new_face = new Face ((*face_rects)[i]);
		faces->push_back(new_face);
	}
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


	std::vector<Face*> faces;

	/*### Step 3: read the video stream ###*/
	Mat frame_bw;
	while (true) {

		Mat frame;
		webcam >> frame;

		/*### get the frame in grayscale, high contrast ###*/
		cvtColor (frame, frame_bw, CV_BGR2GRAY);
		equalizeHist(frame_bw, frame_bw);

		/*### get all of the faces ###*/
		vector<Rect> face_rects = detect_face_rects (frame_bw);


		/*### draw them on the display frame ###*/
  		for( int i = 0; i < face_rects.size(); i++ ) {

  			/*### print some info on all of the detected face rects ###*/
    		Point center( face_rects[i].x + face_rects[i].width*0.5, face_rects[i].y + face_rects[i].height*0.5 );
    		ellipse( frame, center, Size( face_rects[i].width*0.5, face_rects[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    		/*### update faces ###*/
    		update_faces (&faces, &face_rects);
    	}


		imshow ("DISPLAY", frame);



		int key = waitKey (30);
		if (key == 'q') break;
	}
	return 0;
}
