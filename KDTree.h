#ifndef KDTREE_H_
#define KDTREE_H_
#include <SPPoint.h>
#include <SPConfig.h>
#include <SPKDarray.h>

/**
 * KDTREENODE_H_ Summary
 * Encapsulates a point with variable length dimension. The coordinates
 * values are double types, and each point has a non-negative index which
 * represents the image index to which the point belongs.
 *
 * The following functions are supported:
 *
 * Init			        	- Creates a new kdArray
 * spKDArrayDestroy 	   - Free all resources associated with a array
 * Split						- A getter of the dimension of a point
 * spKDArrayGetSize			- A getter of the index of a point
 * spKDArrayGetDim		   - A getter of a given coordinate of the point
 * spKDArrayGetDB		   - Calculates the L2 squared distance between two points
 *
 */

/** Type for defining the point **/
typedef struct kd_tree_node* KDTreeNode;




#endif /* KDTREE_H_ */
