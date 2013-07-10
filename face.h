#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/* TO DO:
	- compute statistics on the expected discrepancy, etc, factor that in
	- 


class Face {

	/*--- Global Variables ---*/
	Rect boundaries;					//boundaries of the face
	std::vector <float> velocity;		//current velocity 
	Point current_location;
	Point previous_location;

public:


	/* Function: get_center_of_rectangle
	 * ---------------------------------
	 * given a rectangle, this function will return the coordinates of its center
	 */
	Point get_center_of_rectangle (Rect rect) {
		Point center;
		center.x = rect.x + rect.width*0.5;
		center.y = rect.y + rect.height*0.5;
		return center
	}



	/* Function: get_best_face_index
	 * -----------------------------
	 * given a vector of face_rects, this function returns the index of the one that best matches this face object.
	 * takes into account both 
	 * TODO: modify this so that it takes velocity into account as well!
	 * TODO: modify this to return an error value if the discrepancy is too large!
	 */
	int get_best_face_index (std::vector<Rect> face_rects) {
		int min_discrepancy = 100000;
		int best_index = -1;

		/*### Get the best-fitting index ###*/
		for (int i=0;i<face_rects.size (); i++) {
			Rect current_face_rect = face_rects[i];

			/*### get location discrepancy: squared euclidean distance ###*/
			/*		NOTE: you can modify this to take into account velocity, as well! */
			Point current_face_center = get_center_of_rectangle (current_face_rect);
			int location_discrepancy = pow(current_location.x - current_face_center.x, 2) + pow(current_location.y - current_face_center.y, 2);

			/*### get size discrepancy ###*/
			int size_discrepancy = abs(boundaries.width - current_face_rect.width) * abs(boundaries.height - current_face_rect.height);

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


	/* Function: update_location
	 * -------------------------
	 * given the vector of all faces just detected, this function will determine which face is the best match for it,
	 * then will update its location/velocity accordingly
	 */
	void update (std::vector<Rect> face_rects) {

		int match_index = get_best_face_index (face_rects);
		Rect best_face_boundaries = face_rects[match_index];

		boundaries = best_face_boundaries;



	}

	Rect get_boundaries () { return boundaries;}
	Rect get_velocity () { return velocity; }


}