/*
 *
 *
 *  Created on: May 20, 2016
 *      Author: MatanGizunterman and LironGazit
 */

#include "SPKDarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

//Declarations of helper func
int round_div(int dividend);
int maxDiff(KDArray kdarr);

struct sp_kdarray_t{
	SPPoint* arr;
	int size;
	int dim;
	int** kdDB;
};

typedef struct kd_array{
	int index;
	double data;
} KDArrayCoor;

int cmpfunc(const void * A, const void* B){
	double dif=((KDArrayCoor*)A)->data-((KDArrayCoor*) B)->data;
	if(dif==0){
		return 0;
	}
	if(dif<0){
		return -1;
	}
	else{
		return 1;
	}
}

//Empty Array'Size of 0 array and other mikrey katze
void Init(KDArray kdarray,SPPoint* arr, int size){
	kdarray->size=size;
	kdarray->arr=(SPPoint*)malloc(sizeof(SPPoint)*size);
	for(int i=0;i<size;i++){
		kdarray->arr[i]=spPointCopy(arr[i]);
	}
	KDArrayCoor* temparr = (KDArrayCoor*)malloc(sizeof(KDArrayCoor)*size);
	kdarray->dim=spPointGetDimension(arr[0]);
	//Aloccating kdDB

	kdarray->kdDB  =(int**)malloc(sizeof(int*)*kdarray->dim);
	for(int i=0;i<kdarray->dim;i++){
		kdarray->kdDB[i]=(int*)malloc(sizeof(int)*size);
	}
   //Copy PointArray

	for(int i=0;i<kdarray->dim;i++){
		for(int j=0;j<size;j++){
			temparr[j].data=spPointGetAxisCoor(arr[j],i);
			temparr[j].index=i;
		}
		qsort((void*)temparr, size, sizeof(temparr[0]), cmpfunc);
		for(int j=0;j<size;j++){
			kdarray->kdDB[i][j]=temparr[j].index;
		}

	}
}


//Split Method - preduces two arrays into pointed arrays
double Split(KDArray kdArr, int coor,KDArray kdLeft ,KDArray kdRight){
	int tempSize;
	double median;
	//Initialize left array
	tempSize = round_div(kdArr->size);
	SPPoint* tempArr = (SPPoint*)malloc(sizeof(SPPoint)*tempSize);
	for(int i=0,index;i<tempSize;i++){
		index =kdArr->kdDB[coor][i];
		tempArr[i]=spPointCopy(kdArr->arr[index]);
	}
	Init(kdLeft,tempArr,tempSize);
	for(int j=0;j<tempSize;j++)
		spPointDestroy(tempArr[j]);
	//Initialize right array
	tempSize = kdArr->size/2;
	tempArr = (SPPoint*)malloc(sizeof(SPPoint)*tempSize);
	int medianInded=kdArr->kdDB[coor][(kdLeft->size)-1];
	median = spPointGetAxisCoor(kdArr->arr[medianInded],coor);
	for(int i=0,index;i<tempSize;i++){
		index =kdArr->kdDB[coor][i+(kdLeft->size)];
		tempArr[i]=spPointCopy(kdArr->arr[index]);
	}
	Init(kdRight,tempArr,tempSize);
	for(int j=0;j<tempSize;j++)
		spPointDestroy(tempArr[j]);

	return median;
}


void spKDArrayDestroy(KDArray kdArr){
	for(int i=0;i<kdArr->dim;i++)
		free(kdArr->kdDB[i]);
	for(int j=0;j<kdArr->size;j++)
		spPointDestroy(kdArr->arr[j]);
	free(kdArr->arr);
	free(kdArr->kdDB);
}

int KDArrayGetSize(KDArray kdarr){
	return kdarr->size;
}
int round_div(int dividend){
    return (dividend +1)/2;
}

//@pre assume size of array =1
SPPoint* KDArrayGetFirst(KDArray kdarr){
	return kdarr->arr;
}
int calcCoor(KDArray kdarr,SP_SPLIT_METHOD extractionMode, int i){
	int dim = kdarr->dim;
	time_t t;
	switch(extractionMode) {
	      case MAX_SPREAD :
	    	  return maxDiff(kdarr);
	      case RANDOM :
	    	  srand((unsigned) time(&t));
	    	  return rand()%dim;
	      case INCREMENTAL :
	    	  return (i+1)%dim;
	      default :
	         return 0;
	   }
}

int maxDiff(KDArray kdarr){
	double max;
	double min;
	double maxDiff=0;
	int maxCoor=0;
	for(int coor=0;coor<kdarr->dim;coor++){
		min=spPointGetAxisCoor(kdarr->arr[0],coor);
		max=spPointGetAxisCoor(kdarr->arr[0],coor);

		for(int i=1;i<kdarr->size;i++){
			if(spPointGetAxisCoor(kdarr->arr[i],coor)<min)
				min=spPointGetAxisCoor(kdarr->arr[i],coor);
			if(spPointGetAxisCoor(kdarr->arr[i],coor)>max)
				max=spPointGetAxisCoor(kdarr->arr[i],coor);
		}
		if(abs(max-min)>maxDiff){
			maxDiff=abs(max-min);
			maxCoor=coor;
		}

	}
	return maxCoor;
}




