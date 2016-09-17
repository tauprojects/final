
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPConfig.h"
#include "SPPoint.h"
#define FAIL_ALOC_MSG "Failed to allocate"
#define CONFIG_INIT_SUCCESS "Configuration Initialize"
#define IMGPROC_INIT_SUCCESS_MSG "ImagProc Object Created"
#define FEAT_FIlLES_INIT_FAIL_MSG "Failed to create .feat files"
#define FEAT_ARRAY_INIT_FAIL_MSG "Failed to create total features array"
#define BPQUEUE_FAILURE "Failed to initialize BPQueue"
#define CANNOT_OPEN_FILE_MSG "Input/Output error,Failed opening file"
#define ERROR_MSG "Failure"

#define EXIT_MSG "Exit..."

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
