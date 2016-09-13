
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPConfig.h"
#include "SPPoint.h"
#define FAIL_ALOC_MSG "Failed to allocate"


#define BAD_INDEX 90

struct featHits {
	int hits;
	int index;
};

//Declarations of Functions
int hitsComp(const void * A, const void* B);
SP_CONFIG_MSG createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoint);
SPPoint* createTotalFeatArray(SPConfig config, int numOfImg,int dim,int* sizeOfTotalFeat);
void destroyAll();
