/*
 *
 *
 *  Created on: May 20, 2016
 *      Author: MatanGizunterman and LironGazit
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <KDTree.h>

#define INVALID NULL

struct kd_tree_node{
	int dim;
	double val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint* data;
};

//In the main function int i=-1 at the first call
KDTreeNode kdTreeInit(KDArray arr, SP_SPLIT_METHOD extractionMode,int i){
	KDTreeNode root;
	root->data=NULL;
	if(KDArrayGetSize(arr)==1){
		root->dim=0;
		root->val =0;
		root->left=NULL;
		root->right=NULL;
		root->data=KDArrayGetFirst(arr);
		return root;
	}
	int coor=calcCoor(arr,extractionMode,i);
	root->dim=coor; //calculate coor from extraction mode
	KDArray leftArray,rightArray;
	//msg?
	root->val=Split(arr,coor,leftArray,rightArray);
	root->left=kdTreeInit(leftArray,extractionMode, i);
	root->right=kdTreeInit(rightArray,extractionMode,i);
	return root;

}




