#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_
#include <SPPoint.h>
#include <SPConfig.h>
/**
 * SPKDArray Summary
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

/** Type for defining the array **/
typedef struct sp_kdarray_t* KDArray;

/**
 * Init KDArray to array pointer.
 * Given SSPoint arr in length of size
 *
 * the KDArry will contain the dim of each point, the size of KDArray,
 * copy of the SPPoint arr and KDdb that in each row will sort by the value in this dim.
 *
 * @return- the response of creating the KDArray
 */
SP_CONFIG_MSG Init(KDArray array,SPPoint* arr, int size);

/**
 * Allocates a copy of the given point.
 *
 * Given the point source, the functions returns a
 * new pint P = (P_1,...,P_{dim-1}) such that:
 * - P_i = source_i (The ith coordinate of source and P are the same)
 * - dim(P) = dim(source) (P and source have the same dimension)
 * - index(P) = index(source) (P and source have the same index)
 *
 * @param source - The source point
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
double Split(KDArray kdArr, int coor,KDArray kdLeft ,KDArray kdRight);
/**
 * Free all memory allocation associated with KDArray,
 * if array is NULL nothing happens.
 */
void spKDArrayDestroy(KDArray kdArr);
int KDArrayGetSize(KDArray kdarr);
SPPoint* KDArrayGetFirst(KDArray kdarr);
int calcCoor(KDArray kdarr,SP_SPLIT_METHOD extractionMode, int i);
int maxDiff(KDArray kdarr);



#endif /* SPKDARRAY_H_ */
