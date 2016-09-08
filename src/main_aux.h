
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <KDTree.h>
#include <SPConfig.h>


#define BAD_INDEX 90

struct featHits {
	int hits;
	int index;
};

//Declarations of Functions
int hitsComp(const void * A, const void* B);
void createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoint);
SPPoint* createTotalFeatArray(SPConfig config, int numOfImg,int dim,int* sizeOfTotalFeat);
void destroyAll();
