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

typedef enum sp_kdarray_msg_t {
	SP_KDARRAY_FAIL_ALLOCATION,
	SP_KDARRAY_FAIL,
	SP_KDARRAY_SUCCESS
} SP_KDARRAY_MSG;
/**
 * Init KDArray to array pointer.
 * Given SSPoint arr in length of size
 *
 * the KDArry will contain the dim of each point, the size of KDArray,
 * copy of the SPPoint arr and KDdb that in each row will sort by the value in this dim.
 *
 * @return- the response of creating the KDArray
 */
KDArray KdArrayInit(SPPoint* arr, int size);
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
double Split(KDArray kdArr, int coor,KDArray* kdLeft ,KDArray* kdRight);
/**
 * Free all memory allocation associated with KDArray,
 * if array is NULL nothing happens.
 */
void spKDArrayDestroy(KDArray kdArr);

/**
 * Getter of KDArry size
 * @param kdarr - source kdarry
 * @return 
 * KdArry's size
 */
int KDArrayGetSize(KDArray kdarr);

/**
 * Getter for first element in KDArray
 *
 * @pararm
 * kdarry - the source kdarry
 * @return
 * pointter to the first SPPount in kdarry
 */
SPPoint* KDArrayGetFirst(KDArray kdarr);

/**
 * Getter for the next coordinate the kdarry split with.
 *
 * @pararm
 * kdarry - the source kdarry
 * splitMethod - the relevant way the kdarry split with
 * i - privious coordinate the kdarry split with
 *      in the first time i will define to (-1)
 * @return
 * Next coordinite the kdarry split with
 */
int calcCoor(KDArray kdarr,SP_SPLIT_METHOD splitMethod, int i);
int maxDiff(KDArray kdarr);
int KDArrayGetAxis(KDArray kdarr,int i,int j);



#endif /* SPKDARRAY_H_ */
