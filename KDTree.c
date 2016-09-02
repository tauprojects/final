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

//define an INVALID value for the edge case
#define INVALID NULL

struct kd_tree_node{
	int dim;
	double val;
	KDTreeNode left;
	KDTreeNode right;
	SPPoint* data;
};

//In the main function int i=-1 at the first call
//check the function that will cal the recursive func with the needed value
KDTreeNode kdTreeInit(KDArray arr, SP_SPLIT_METHOD splitMethod,int i){
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
	int coor=calcCoor(arr,splitMethod,i);
	root->dim=coor; //calculate coor from extraction mode
	KDArray leftArray,rightArray;
	//msg?
	root->val=Split(arr,coor,leftArray,rightArray);
	root->left=kdTreeInit(leftArray,splitMethod, i);
	root->right=kdTreeInit(rightArray,splitMethod,i);
	return root;

}
bool isLeaf(KDTreeNode curr){
	if(curr->data!=NULL)
		return true;
	else
		return false;
}
KD_TREE_MSG kNearestNeighbors(SPBPQueue bpq,KDTreeNode curr, SPPoint point){
	KD_TREE_MSG msg;
	if(curr==NULL){
		return KD_TREE_NULL_ARGUMENT;
	}
	if(isLeaf(curr)){
		double dist = spPointL2SquaredDistance(point,curr->data[0]);
		int index = spPointGetIndex(curr->data[0]);
		spBPQueueEnqueue(bpq,spListElementCreate(dist,index));
		return KD_TREE_SUCCESS;
	}
	if(spPointGetAxisCoor(point,curr->dim)<=curr->val){
		msg = kNearestNeighbors(bpq,curr->left,point);
		double temp = (curr->val-spPointGetAxisCoor(point,curr->dim))*(curr->val-spPointGetAxisCoor(point,curr->dim));
		if(!spBPQueueIsFull(bpq) || temp<spBPQueueMaxValue(bpq))
			msg = kNearestNeighbors(bpq,curr->right,point);

	}
	else{
		msg = kNearestNeighbors(bpq,curr->right,point);
		double temp = (curr->val-spPointGetAxisCoor(point,curr->dim))*(curr->val-spPointGetAxisCoor(point,curr->dim));
		if(!spBPQueueIsFull(bpq) || temp<spBPQueueMaxValue(bpq))
			msg = kNearestNeighbors(bpq,curr->left,point);	}

}

void KDTreeDestroy(KDTreeNode root){
	//To-Do
}


