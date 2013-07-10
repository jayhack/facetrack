#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "face.h"

using namespace std;
using namespace cv;

/*########################################################################################################################*/
/*############################[--- UTILITIES ---] ########################################################################*/
/*########################################################################################################################*/
/* Function: get_center_of_rectangle
 * ---------------------------------
 * given a rectangle, this function will return the coordinates of its center
 */
Point get_center_of_rectangle (Rect rect) {
	Point center;
	center.x = rect.x + rect.width*0.5;
	center.y = rect.y + rect.height*0.5;
	return center;
}










/*########################################################################################################################*/
/*############################[--- FACE CLASS ---] #######################################################################*/
/*########################################################################################################################*/

/* Function: print_info
 * --------------------
 * this function will print all relevant info on this face to standard out
 */
void Face::print_info () {
	cout << "### Face: print info" << endl;
	cout << "### 	- center_coordinates: (" << current_location.x << ", " << current_location.y << ")" << endl;
	cout << "### 	- boundary width, height: " << boundary.width << ", " << boundary.height << endl;
	cout << endl;
}










/*########################################################################################################################*/
/*############################[--- CONSTRUCTOR/DESTRUCTOR ---] ###########################################################*/
/*########################################################################################################################*/

/* Function: constructor
 * ---------------------
 * will set the location of the face.
 */
Face::Face () {}
Face::Face (Rect face_rect) {
	boundary = face_rect;
	current_location = get_center_of_rectangle (boundary);
	print_info ();
}

/* Function: constructor
 * ---------------------
 * will set the location of the face.
 */
Face::~Face () {
 	cout << "--- DESTROYING FACE: (info printed below) ---" << endl;
 	print_info ();
 }







/*########################################################################################################################*/
/*############################[--- UPDATING ---] #########################################################################*/
/*########################################################################################################################*/
/* Function: get_best_face_index
 * -----------------------------
 * given a vector of face_rects, this function returns the index of the one that best matches this face object.
 * takes into account both 
 * TODO: modify this so that it takes velocity into account as well!
 * TODO: modify this to return an error value if the discrepancy is too large!
 */
int Face::get_best_face_index (vector<Rect> face_rects) {
	int min_discrepancy = 100000;
	int best_index = -1;

	/*### Get the best-fitting index ###*/
	for (int i=0;i<face_rects.size (); i++) {
		Rect current_face_rect = face_rects[i];

		/*### get location discrepancy: squared euclidean distance ###*/
		/*		NOTE: you can modify this to take into account velocity, as well! */
		Point current_face_center = get_center_of_rectangle (current_face_rect);
		int x_disc = (current_location.x - current_face_center.x);
		int y_disc = (current_location.y - current_face_center.y);
		int location_discrepancy = x_disc*x_disc + y_disc*y_disc;

		/*### get size discrepancy ###*/
		int size_discrepancy = abs(boundary.width - current_face_rect.width) * abs(boundary.height - current_face_rect.height);

		/*### total discrepancy ###*/
		int current_discrepancy = location_discrepancy * size_discrepancy;

		/*### update the min stats ###*/
		if (current_discrepancy < min_discrepancy) {
			min_discrepancy = current_discrepancy;
			best_index = i;
		}

	}

	return best_index;
}


/* Function: update
 * -------------------------
 * given the vector of all faces just detected, this function will determine which face is the best match for it,
 * then will update its location/velocity accordingly
 */
void Face::update (vector<Rect> face_rects) {
	int match_index = get_best_face_index (face_rects);
	Rect best_face_boundary = face_rects[match_index];
	boundary = best_face_boundary;
	current_location = get_center_of_rectangle (boundary);
}







/*########################################################################################################################*/
/*############################[--- GETTERS ---] ##########################################################################*/
/*########################################################################################################################*/

/* Function: getters
 * -----------------
 * you know the drill.
 */
Rect Face::get_boundary () { return boundary;}
Vec2b Face::get_velocity () { return velocity;}
