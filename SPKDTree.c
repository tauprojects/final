/*
 * SPPoint.c
 *
 *  Created on: May 20, 2016
 *      Author: MatanGizunterman and LironGazit
 */

#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

SPPoint spPointCreate(double* data, int dim, int index){
	if(dim<=0 || index<0 || data==NULL){
		return NULL;
	}
	SPPoint point=(SPPoint) malloc(sizeof(*point));
	if(point==NULL){
		return NULL;
	}
	point->data=(double*) malloc(sizeof(double)*dim);
	if(point->data==NULL){
		return NULL;
	}
	for(int i=0;i<dim;i++){
		point->data[i]=data[i];
	}
	point->dim=dim;
	point->index=index;
	return point;
}

SPPoint spPointCopy(SPPoint source){
	assert(source!=NULL);
	int dim=spPointGetDimension(source);
	int index=spPointGetIndex(source);
	double* data=source->data;
	return spPointCreate(data, dim,index);
}

void spPointDestroy(SPPoint point){
	if (point == NULL) {
		return;
	}
	free(point->data);
	free(point);
}

int spPointGetDimension(SPPoint point){
	assert(point!=NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point){
	assert(point!=NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point!=NULL);
	assert(axis>=0);
	assert(axis<point->dim);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL);
	assert(q!=NULL);
	int dimP=spPointGetDimension(p);
	int dimQ=spPointGetDimension(q);
	assert(dimP==dimQ);
	double sum=0,temp;
	for(int i=0;i<dimP;i++){
		temp=spPointGetAxisCoor(p,i)-spPointGetAxisCoor(q,i);
		temp=temp*temp;
		sum+=temp;
	}
	return sum;
}



