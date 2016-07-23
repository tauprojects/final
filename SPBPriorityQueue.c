/*
 * SPBPriorityQueue.c
 *
 *  Created on: May 20, 2016
 *      Author: lirongazit
 */
#include "SPBPriorityQueue.h"
#include "SPList.h"
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define MSG_MACRO { if(msg==SP_LIST_OUT_OF_MEMORY){ if(isFull) return SP_BPQUEUE_FULL; else return SP_BPQUEUE_OUT_OF_MEMORY;} else if(msg==SP_LIST_SUCCESS){ if(isFull) return SP_BPQUEUE_FULL; else return SP_BPQUEUE_SUCCESS; }}

struct sp_bp_queue_t{
	int maxSize;
	SPList qList;
};

SPBPQueue spBPQueueCreate(int maxSize){
	if(maxSize<0){
		return NULL;
	}
	SPBPQueue queue=(SPBPQueue) malloc(sizeof(*queue));
	if(queue==NULL){
		return NULL;
	}
	queue->qList=spListCreate();
	queue->maxSize=maxSize;
	return queue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	assert(source!=NULL);
	SPBPQueue queueCopy=(SPBPQueue) malloc(sizeof(*queueCopy));
	if(queueCopy==NULL){
			return NULL;
	}
	queueCopy->qList=spListCopy(source->qList);
	queueCopy->maxSize=spBPQueueGetMaxSize(source);
	return queueCopy;
}

void spBPQueueDestroy(SPBPQueue source){
	if(source==NULL){
		return;
	}
	spListDestroy(source->qList);
	free(source);
}

void spBPQueueClear(SPBPQueue source){
	if(source==NULL){
			return;
	}
	spListClear(source->qList);
}

int spBPQueueSize(SPBPQueue source){
	assert(source!=NULL);
	return spListGetSize(source->qList);
}

int spBPQueueGetMaxSize(SPBPQueue source){
	assert(source!=NULL);
	return source->maxSize;
}

void spBPQsetTailAsCurrent(SPList qlist){
	spListGetFirst(qlist); //Function returning unused element.
	for(int i=0; i<spListGetSize(qlist)-1;i++){
		spListGetNext(qlist);	    //Function returning unused element.
	}
}
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if(source==NULL || element==NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	bool isFull=false;
	if(spBPQueueIsFull(source)){
		if(spListElementGetValue(element)>spBPQueueMaxValue(source)){
			return SP_BPQUEUE_FULL;
		}
		else if(spListElementGetValue(element)==spBPQueueMaxValue(source)){
			spBPQsetTailAsCurrent(source->qList);
			if(spListElementGetIndex(element) < spListElementGetIndex(spListGetCurrent(source->qList))){
				spListRemoveCurrent(source->qList);
				isFull=true;
			}
			else{
				return SP_BPQUEUE_FULL;
			}
		}
		else{
			spBPQsetTailAsCurrent(source->qList);
			spListRemoveCurrent(source->qList);
			isFull=true;
		}
	}
	SP_LIST_MSG msg;
	if(spBPQueueIsEmpty(source) || (spListElementGetValue(element)<spBPQueueMinValue(source))){
		msg=spListInsertFirst(source->qList,element);
		MSG_MACRO
	}
	if(spListElementGetValue(element)>spBPQueueMaxValue(source)){
		msg=spListInsertLast(source->qList,element);
		MSG_MACRO
	}
	spListGetFirst(source->qList);
	while(spListElementGetValue(spListGetCurrent(source->qList))<spListElementGetValue(element)){
		spListGetNext(source->qList);
	}
	if(spListElementGetValue(spListGetCurrent(source->qList))==spListElementGetValue(element)){
		if(spListElementGetIndex(element) < spListElementGetIndex(spListGetCurrent(source->qList))){
			msg=spListInsertBeforeCurrent(source->qList,element);
		}
		else{
			msg=spListInsertAfterCurrent(source->qList,element);
		}
	}
	else{
		msg=spListInsertBeforeCurrent(source->qList,element);
	}
	MSG_MACRO
	return SP_BPQUEUE_OUT_OF_MEMORY;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if(source==NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(spBPQueueIsEmpty(source)){
		return SP_BPQUEUE_EMPTY;
	}
	spListGetFirst(source->qList);
	SP_LIST_MSG msg=spListRemoveCurrent(source->qList);
	if(msg==SP_LIST_NULL_ARGUMENT){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	else if(msg==SP_LIST_INVALID_CURRENT){
		return SP_BPQUEUE_EMPTY;
	}
	else{
		return SP_BPQUEUE_SUCCESS;
	}
}

SPListElement spBPQueuePeek(SPBPQueue source){
	assert(source!=NULL);
	return (spListElementCopy(spListGetFirst(source->qList)));
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	assert(source!=NULL);
	spBPQsetTailAsCurrent(source->qList);
	return (spListElementCopy(spListGetCurrent(source->qList)));
}

double spBPQueueMinValue(SPBPQueue source){
	assert(source!=NULL);
	return (spListElementGetValue(spListGetFirst(source->qList)));
}

double spBPQueueMaxValue(SPBPQueue source){
	assert(source!=NULL);
	spBPQsetTailAsCurrent(source->qList);
	return (spListElementGetValue(spListGetCurrent(source->qList)));
}

bool spBPQueueIsEmpty(SPBPQueue source){
	assert(source!=NULL);
	int size=spBPQueueSize(source);
	if (size==0){
		return true;
	}
	else{
		return false;
	}
}

bool spBPQueueIsFull(SPBPQueue source){
	assert(source!=NULL);
	return (spBPQueueSize(source)==spBPQueueGetMaxSize(source));
}

SPList spBPQueueGetList(SPBPQueue source){
	return source->qList;
}




