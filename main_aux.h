
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPConfig.h"
#include "SPPoint.h"
#define FAIL_ALOC_MSG "Failed to allocate"
#define CONFIG_INIT_SUCCESS "Configuration Initialized Successfully"
#define IMGPROC_INIT_SUCCESS_MSG "ImagProc Object Created Successfully"
#define FEAT_FIlLES_INIT_FAIL_MSG "Failed to create .feat files"
#define FEAT_ARRAY_INIT_FAIL_MSG "Failed to create total features array"
#define BPQUEUE_FAILURE "Failed to initialize BPQueue"
#define CANNOT_OPEN_FILE_MSG "Input/Output error,Failed opening file"
#define ERROR_MSG "Failure"
#define ENTER_IMG_PATH "Please enter an image path:"
#define LOG_CURRENT_IMAGE_MSG "Working On Query Image In Path:"
#define EXIT_MSG "Exit..."


#define BAD_INDEX 90

struct featHits {
	int hits;
	int index;
};

//Declarations of Functions

//hitsComp Function - Compare between two hitStruct objects in order to sort them
int hitsComp(const void * A, const void* B);

//Create FeatFiles - Helper function that creates .feat files with all the features values
SP_CONFIG_MSG createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoint);

//createTotalFeatArray - Helper Function that creates an array of SPPoints
//Contains all features from all the images - sent to the KDArray for initialization.
SPPoint* createTotalFeatArray(SPConfig config, int numOfImg,int dim,int* sizeOfTotalFeat);
void destroyAll();
