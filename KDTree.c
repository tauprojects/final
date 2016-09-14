/*
 *
 *
 *  Created on: May 20, 2016
 *      Author: MatanGizunterman and LironGazit
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "KDTree.h"


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
	KDTreeNode root = (KDTreeNode)malloc(sizeof(*root));
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
	KDArray leftArray;
	KDArray rightArray;
	root->val  =Split(arr,coor,&leftArray,&rightArray);
	root->left =kdTreeInit(leftArray,splitMethod, coor);
	root->right=kdTreeInit(rightArray,splitMethod,coor);
	return root;

}

bool isLeaf(KDTreeNode curr){
	if(curr->data!=NULL)
		return true;
	else
		return false;
}
KD_TREE_MSG kNearestNeighbors(SPBPQueue bpq,KDTreeNode curr, SPPoint point){
	if(curr==NULL)
		return KD_TREE_INVALID_CURRENT;
	if(isLeaf(curr)){
		double dist = spPointL2SquaredDistance(point,curr->data[0]);
		int index = spPointGetIndex(curr->data[0]);
		SPListElement e=spListElementCreate(index,dist);
		SP_BPQUEUE_MSG bpeMsg = spBPQueueEnqueue(bpq,e);
		spListElementDestroy(e);
		if(bpeMsg==SP_BPQUEUE_FULL || bpeMsg ==SP_BPQUEUE_SUCCESS){
			return KD_TREE_SUCCESS;
		}
		else
			return KD_TREE_INIT_FAIL;
	}
    bool isLeft=true;
	if(spPointGetAxisCoor(point,curr->dim)<=curr->val){
		KD_TREE_MSG msg = kNearestNeighbors(bpq,curr->left,point);
		if(msg!=KD_TREE_SUCCESS)
			return KD_TREE_INIT_FAIL;

	}
	else{
		isLeft = false;
		KD_TREE_MSG msg = kNearestNeighbors(bpq, curr->right, point);
		if (msg != KD_TREE_SUCCESS)
			return KD_TREE_INIT_FAIL;
    }
    double temp = ((curr->val)-spPointGetAxisCoor(point,curr->dim));
    temp=temp*temp;
    if(!spBPQueueIsFull(bpq) || temp<spBPQueueMaxValue(bpq)){
        if(isLeft){
         	KD_TREE_MSG msg = kNearestNeighbors(bpq,curr->right,point);
    		if (msg != KD_TREE_SUCCESS)
    			return KD_TREE_INIT_FAIL;
        }
        else{
        	KD_TREE_MSG msg = kNearestNeighbors(bpq,curr->left,point);
    		if (msg != KD_TREE_SUCCESS)
    			return KD_TREE_INIT_FAIL;
        }
    }
    return KD_TREE_SUCCESS;
}

void KDTreeDestroy(KDTreeNode root){
	if(root->data!=NULL){
		spPointDestroy(root->data[0]);
		free(root);
		return ;
	}
	KDTreeDestroy(root->left);
	KDTreeDestroy(root->right);
//	free(root->left);
//	free(root->right);
}


