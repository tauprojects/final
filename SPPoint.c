/*
 * SPPoint.c
 *
 *  Created on: May 17, 2016
 *      Author: alonfrydberg
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"

struct sp_point_t {
	double * data;
	int dim;
	int index;
};

SPPoint spPointCreate(double* data, int dim, int index) {
	int i;
	//check validity of input
	if (index < 0 || dim <= 0 || !data)
		return NULL;

	//allocate  memory
	double * temp = (double*) malloc(dim * sizeof(double));
	SPPoint point = (SPPoint) malloc(sizeof(struct sp_point_t));

	//check allocation failure
	if (!point || !temp) //if NULL
		return NULL;

	//copy data into temp
	for (i = 0; i < dim; i++) {
		temp[i] = data[i];
	}

	//assign fields
	point->data = temp;
	point->dim = dim;
	point->index = index;

	return point;
}

SPPoint spPointCopy(SPPoint source) {
	//check validity of argument
	assert(source != NULL);

	//call spPointCreate with the proper arguments taken from source
	return spPointCreate(source->data, source->dim, source->index);
}

void spPointDestroy(SPPoint point) {
	//check validity of argument
	if (point == NULL)
		return;

	//free the allocated memory
	free(point->data);
	free(point);
}

int spPointGetDimension(SPPoint point) {
	//check validity of argument
	assert(point!=NULL);

	return point->dim;
}

int spPointGetIndex(SPPoint point) {
	//check validity of argument
	assert(point!=NULL);

	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis) {
	//check validity of argumentS
	assert(point!=NULL);
	assert((axis >= 0) && (axis < point->dim));

	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q) {
	//check validity of argument
	assert(p!= NULL && q!= NULL);
	assert(p->dim == q->dim);

	//calculate L2distance
	double d = 0, res = 0;
	for (int i = 0; i < p->dim; i++) {

		d = p->data[i] - q->data[i];
		res += (d * d);
	}

	return res;
}
