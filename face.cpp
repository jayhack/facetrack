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


/* Function: crop_roi
 * ------------------
 * given a mat and a proposed roi rect, this function will crop the roi so that 
 * it fits correctly on top of the mat
 */
Rect crop_roi (Rect proposed_roi, Mat frame) {
	
	Point top_left = Point(proposed_roi.x, proposed_roi.y);
	Point bottom_right = Point(proposed_roi.x + proposed_roi.width, proposed_roi.y + proposed_roi.height);
	
	if (top_left.x < 0)
		top_left.x = 0;
	if (bottom_right.x > frame.cols) 
		bottom_right.x = frame.cols;
	if (top_left.y < 0)
		top_left.y = 0;
	if (bottom_right.y > frame.rows) 
		bottom_right.y = frame.rows;

	Rect cropped_roi;
	cropped_roi.x = top_left.x;
	cropped_roi.y = top_left.y;
	cropped_roi.width = bottom_right.x - top_left.x;
	cropped_roi.height = bottom_right.y - top_left.y;
	assert (cropped_roi.width > 0);
	assert (cropped_roi.height > 0);

	return cropped_roi;
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
	cout << "###	- velocity (x, y): (" << velocity[0] << ", " << velocity[1] << ")" << endl;
	cout << endl;
}









/*########################################################################################################################*/
/*############################[--- CONSTRUCTOR/DESTRUCTOR ---] ###########################################################*/
/*########################################################################################################################*/
/* Function: init_stats
 * ------------------------------------
 * function to initialize velocity and location to default values.
 * call this when you lose the face or haven't seen it yet.
 */
 void Face::init_stats () 
 {
 	
 	/*### boundary ###*/
 	boundary.width = 0;
 	boundary.height = 0;
 	boundary.x = -1;
 	boundary.y = -1;

 	/*### current location ###*/
 	current_location.x = -1;
 	current_location.y = -1;
 	
 	/*### velocity ###*/
 	velocity[0] = 0;
 	velocity[1] = 0;

 	/*### existence ###*/
 	is_on_screen = false;
 }


/* Function: constructor
 * ---------------------
 * will set the location of the face.
 */
Face::Face (Mat frame) {

	init_stats ();
	
	frame_width = frame.cols;
	frame_height = frame.rows;

	/*### Step 2: load the face cascade ###*/
	if( !face_cascade.load( FACE_CASCADE_NAME ) ){ printf("--(!)Error loading face cascade\n"); assert(false); };
}
Face::Face (Mat frame, Rect face_rect) {
	
	init_stats ();

	frame_width = frame.cols;
	frame_height = frame.rows;

	boundary = face_rect;
	current_location = get_center_of_rectangle (boundary);
	print_info ();

	/*### Step 2: load the face cascade ###*/
	if( !face_cascade.load( FACE_CASCADE_NAME ) ){ printf("--(!)Error loading face cascade\n"); assert(false); };
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
	int min_discrepancy = 10000000;
	int best_index = -1;

	/*### if the face does not yet exist, assume for now that the 0th index corresponds to it ###*/
	if (!exists()) {
		best_index = 0;
		return best_index;
	}

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
 * then will update its location/velocity accordingly. it removes the matching face from the face_rects vector.
 */
void Face::update (vector<Rect> face_rects) {

	/*### Step 1: no faces detected -> start over from square 1 ###*/
	if (face_rects.size () == 0) {
		init_stats ();
		return;
	}

	/*### Step 2: face does not exist yet, though some faces were detected ###*/
	if (!exists ()) {
		boundary = face_rects[0];
	}
	else {
		int match_index = get_best_face_index (face_rects);
		boundary = face_rects[match_index];
	}

	/*### location ###*/
	previous_location = current_location;
	current_location = get_center_of_rectangle (boundary);

	/*### velocity ###*/
	if (previous_location.x == -1 || previous_location.y == -1) {
		velocity[0] = 0;
		velocity[1] = 0;
	}
	else {		
		velocity[0] = current_location.x - previous_location.x;
		velocity[1] = current_location.y - previous_location.y;
	}
	is_on_screen = true;
}




/*########################################################################################################################*/
/*############################[--- DETECTING ---] ########################################################################*/
/*########################################################################################################################*/

/* Function: get_search_area
 * -------------------------
 * this function will compute and return the area of the image that one should apply the haar cascade to in order to find face
 * currently expands by a static 30% on all sides, then chops off for the image.
 */
Rect Face::get_search_area (Mat frame) {

	Rect search_area;
	search_area.width = boundary.width * 1.3;
	search_area.height = boundary.height * 1.3;

	int center_x = current_location.x;
	int center_y = current_location.y;

	search_area.x = center_x - search_area.width*0.5;
	search_area.y = center_y - search_area.height*0.5;

	/*### crop it so that its fits on the original image ###*/
	search_area = crop_roi (search_area, frame);

	return search_area;
}


/* Function: convert_face_rects_to_absolute
 * ----------------------------------------
 * given a pointer to a vector of face rects, as well as the frame and the search area 
 * in which they were detected, this function will set their true coordinates.
 */
 void convert_face_rects_to_absolute (vector<Rect> * face_rects, Rect search_area) {

 	for (int i=0;i<face_rects->size();i++) {
 		(*face_rects)[i].x += search_area.x;
 		(*face_rects)[i].y += search_area.y;
 	}

 	return;
 }


/* Function: detect
 * ----------------------
 * given a frame, this function will detect the face from it.
 */
void Face::detect (Mat frame) {

	int key;

	/*### Step 1: figure out the search area ###*/
	Mat ROI, ROI_bw;
	Rect search_area;
	if (exists()) {
		search_area = get_search_area (frame);
	}
	else {
		cout << "here!" << endl;
		search_area.x = 0;
		search_area.y = 0;
		search_area.width = frame.cols;
		search_area.height = frame.rows;
	}
	ROI = frame (search_area).clone ();


	/*### Step 2: preprocessing (get it into b/w, equalize histogram to improve contrast) ###*/
	cvtColor (ROI, ROI_bw, CV_BGR2GRAY);
	equalizeHist(ROI_bw, ROI_bw);


	/*### Step 3: detect all face-like patterns in the image ###*/
	vector<Rect> face_rects;
	Size min_face_size = Size(280, 280); //ideally, these should be learned!
	Size max_face_size = Size(400, 400);
	face_cascade.detectMultiScale(ROI_bw, face_rects, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, min_face_size, max_face_size);

	convert_face_rects_to_absolute (&face_rects, search_area);


	/*### Step 4: update this face's position and velocity ###*/
	update (face_rects);
	return;
}










/*########################################################################################################################*/
/*############################[--- GETTERS ---] ##########################################################################*/
/*########################################################################################################################*/

/* Function: getters
 * -----------------
 * you know the drill.
 */
Rect Face::get_boundary () { return boundary;}
Point Face::get_center () { return current_location;}
Vec2i Face::get_velocity () { return velocity;}
bool Face::exists () {return is_on_screen;}
