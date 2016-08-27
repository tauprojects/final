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

/** Type for defining the point **/
typedef struct sp_kdarray_t* KDArray;

/**
 * Allocates a new KDArray in the memory.
 * Given data array, dimension dim and an index.
 * The new point will be P = (p_0,p_2,...,p_{dim-1})
 * such that the following holds
 *
 * - The ith coordinate of the P will be p_i
 * - p_i = data[i]
 * - The index of P = index
 *
 * @return
 * NULL in case allocation failure ocurred OR data is NULL OR dim <=0 OR index <0
 * Otherwise, the new point is returned
 */
void Init(KDArray array,SPPoint* arr, int size);
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
