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
#include <math.h>

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
KDArray KdArrayInit(SPPoint* arr, int size){
	int i=0;
	KDArray kdarray = (KDArray)malloc(sizeof(*kdarray));
	if(kdarray==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
		return NULL;
	}
	kdarray->dim=spPointGetDimension(arr[0]);
	kdarray->kdDB  =(int**)malloc(sizeof(int*)*kdarray->dim);
	if(kdarray->kdDB==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
		return NULL;
	}
	for(int i=0;i<kdarray->dim;i++){
		kdarray->kdDB[i]=(int*)malloc(sizeof(int)*size);
		if(kdarray->kdDB[i]==NULL){
			puts(SP_KDARRAY_FAIL_ALLOCATION);
		}
	}
	if(kdarray==NULL || arr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
		return NULL;
	}
	kdarray->size=size;
	kdarray->arr=(SPPoint*)malloc(sizeof(SPPoint)*size);
	if(kdarray->arr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
		return NULL;
	}

	//allocate array
	if(kdarray->arr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
	}
	for(int i=0;i<size;i++){
		kdarray->arr[i]=spPointCopy(arr[i]);
	}

	KDArrayCoor* temparr = (KDArrayCoor*)malloc(sizeof(KDArrayCoor)*size); //creating a temporary arr
	if(temparr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
	}
	if(kdarray->kdDB==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
	}

   //Copy PointArray
	for(int i=0;i<kdarray->dim;i++){
		for(int j=0;j<size;j++){
			temparr[j].data=spPointGetAxisCoor(arr[j],i);
			temparr[j].index=j;
		}
		qsort((void*)temparr, size, sizeof(temparr[0]), cmpfunc);
		for(int j=0;j<size;j++){
			kdarray->kdDB[i][j]=temparr[j].index;
		}
	}

	return kdarray;
}

int KDArrayGetAxis(KDArray kdarr,int i,int j){
	int result = kdarr->kdDB[i][j];
//	printf("\nresult: %d",result);
	return result;
}

//Split Method - preduces two arrays into pointed arrays
double Split(KDArray kdArr, int coor,KDArray* kdLeft ,KDArray* kdRight){
	int tempSize;
	double median=0;
	//Initialize left array
	tempSize = round_div(kdArr->size);
	SPPoint* tempArr = (SPPoint*)malloc(sizeof(SPPoint)*tempSize);
	if(tempArr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
	}
	for(int i=0,index;i<tempSize;i++){
		index =KDArrayGetAxis(kdArr,coor,i);
		tempArr[i]=spPointCopy(kdArr->arr[index]);
	}

	*kdLeft=KdArrayInit(tempArr,tempSize);
	for(int j=0;j<tempSize;j++)
		spPointDestroy(tempArr[j]);
	free(tempArr);
	//Initialize right array
	tempSize = (kdArr->size)/2;
	tempArr = (SPPoint*)malloc(sizeof(SPPoint)*tempSize);
	if(tempArr==NULL){
		puts(SP_KDARRAY_FAIL_ALLOCATION);
	}
	int index = KDArrayGetSize(*kdLeft)-1;
	int medindex;
	medindex=KDArrayGetAxis(kdArr,coor,index);
	median = spPointGetAxisCoor(kdArr->arr[medindex],coor);

	for(int i=0,index;i<tempSize;i++){
		index = KDArrayGetAxis(kdArr,coor,i+KDArrayGetSize(*kdLeft));
		tempArr[i]=spPointCopy(kdArr->arr[index]);
		if(tempArr[i]==NULL){
			puts("SP_KDARRAY_FAIL");
		}
	}
	*kdRight=KdArrayInit(tempArr,tempSize);
	if(*kdRight==NULL){
		return (double)INFINITY;
	}
	for(int j=0;j<tempSize;j++)
			spPointDestroy(tempArr[j]);
	free(tempArr);
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
//
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
int calcCoor(KDArray kdarr,SP_SPLIT_METHOD splieMethod, int i){
	int dim = kdarr->dim;
	time_t t;
	switch(splieMethod) {
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



