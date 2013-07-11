#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

/*########################################################################################################################*/
/*############################[--- UTILITIES ---] ########################################################################*/
/*########################################################################################################################*/
/* Function: get_center_of_rectangle
 * ---------------------------------
 * given a rectangle, this function will return the coordinates of its center
 */
Point get_center_of_rectangle (Rect rect);










/*########################################################################################################################*/
/*############################[--- FACE CLASS ---] #######################################################################*/
/*########################################################################################################################*/
class Face {

	/*--- Location ---*/
	Rect boundary;					//boundary of the face
	Vec2i velocity;
	Point current_location;
	Point previous_location;
	bool is_on_screen;

	/*--- other ---*/
	int frame_width;
	int frame_height;


public:


	/* Function: print_info
	 * --------------------
	 * this function will print all relevant info on this face to standard out
	 */
	void print_info ();


	/*########################################################################################################################*/
	/*############################[--- CONSTRUCTOR/DESTRUCTOR ---] ###########################################################*/
	/*########################################################################################################################*/

	/* Function: constructor
	 * ---------------------
	 * will set the location of the face.
	 */
	Face ();
	Face (int frame_width, int frame_height);
	Face (Rect face_rect, int frame_width, int frame_height);

	/* Function: constructor
	 * ---------------------
	 * will set the location of the face.
	 */
	 ~Face ();




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
	int get_best_face_index (vector<Rect> face_rects);

	/* Function: update
	 * -------------------------
	 * given the vector of all faces just detected, this function will determine which face is the best match for it,
	 * then will update its location/velocity accordingly
	 */
	void update (vector<Rect> face_rects);



	/*########################################################################################################################*/
	/*############################[--- GETTING SEARCH AREA ---] ##############################################################*/
	/*########################################################################################################################*/
	Rect get_search_area ();




	/*########################################################################################################################*/
	/*############################[--- GETTERS ---] ##########################################################################*/
	/*########################################################################################################################*/
	/* Function: getters
	 * -----------------
	 * you know the drill.
	 */
	Rect get_boundary ();
	Point get_center ();
	Vec2i get_velocity ();
	bool exists ();


};