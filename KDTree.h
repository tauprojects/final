#ifndef KDTREE_H_
#define KDTREE_H_
#include <SPKDarray.h>
#include <SPBPriorityQueue.h>

/**
 * KDTree Summary
 * Encapsulates a KDTree with KDTreeNode root,
 * each KDTreeNode has split dimension, mid-value of spliting,
 * left and right sub-trees(or NULL on leafs).
 * leafs has SPPoint data(non-leaf KDTreeNode's data is NULL)
 *
 * The following functions are supported:
 *
 * kdTreeInit			        	- Creates a new KDTree
 * KDTreeDestroy 	   - Free all resources associated with a tree
 *
 */

/** Type for defining the point **/
typedef struct kd_tree_node* KDTreeNode;


typedef enum sp_list_msg_t {
	KD_TREE_SUCCESS,
	KD_TREE_NULL_ARGUMENT,
	KD_TREE_INVALID_CURRENT,
} KD_TREE_MSG;
/**
 * Creates a new KDTree from specific KDArray
 *
 * @param arr - The KDArray that we create the tree from it.
 * @param extractionMode - the Extraction Mode of creating the tree
 * @param i - the indekdTreeInitx of parent dim split
 * @return
 * NULL in case of memory allocation fails.
 * A new KDTreeNode with specific data on leafs(according to arr)
 */
KDTreeNode kdTreeInit(KDArray arr, SP_SPLIT_METHOD splitMethod,int i);


KD_TREE_MSG kNearestNeighbors(SPBPQueue bpq,KDTreeNode root, SPPoint point);

bool isLeaf(KDTreeNode root);

/**
 * Destroys an KDTree.
 * All memory allocation associated with the KDTree will be freed
 *
 * @param root - the target KDTree which will be freed.
 * 			   if root is NULL, then nothing is done
 */
void KDTreeDestroy(KDTreeNode root);



#endif /* KDTREE_H_ */
