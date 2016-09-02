
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <KDTree.h>


#define MAX_LENGTH_PATH 1024
#define BAD_INDEX 90

struct featHits {
	int hits;
	int index;
};

//Declarations of Functions
int hitsComp(const void * A, const void* B);
void createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoint);
int createTotalFeatArray(SPConfig config, int numOfImg,char* path,SPPoint* totalResPoints,int dim);
void destroyAll();
