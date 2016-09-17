#define DEFAULT_FILE "spcbir.config"

#include "SPImageProc.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
extern "C" {
#include "main_aux.h"
#include "SPBPriorityQueue.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPListElement.h"
#include "KDTree.h"
}
using namespace sp;

int main(int argc, char* argv[]) {
	//Declaring and Initializing Variable and Objects
	SP_CONFIG_MSG msg; 				 //msg pointer
	SPConfig config;  				 //config struct instance
	char* imagePath = (char*)malloc(sizeof(char)*MAXLEN);
	SP_SPLIT_METHOD method;
	SPPoint* resPoints;				//temporary point array for each img
	SPPoint* totalResPoints;		//Point Array for all features
	KDArray kdarray;
	KDTreeNode root;

	int sizeOfTotalFeat;

	//Checking argument state
	if (argc == 1) {
		config = spConfigCreate(DEFAULT_FILE, &msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(DEFAULT_ERROR_FILE_OPEN_MSG);
			return 1; //exit(1)
		} else if (msg != SP_CONFIG_SUCCESS)
			return 1; //exit(1)
	} else if (argc == 3 && strcmp(argv[1], "-c") == 0) {
		config = spConfigCreate(argv[2], &msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(ERROR_FILE_OPEN_MSG, argv[2]);
			return 1; //exit(1)
		} else if (msg != SP_CONFIG_SUCCESS)
			return 1; //exit(1)
	} else {
		printf(INVALID_CMD_LINE_ERROR_MSG);
		return 1; //exit(1)
	}

	//Declaring and Initializing Variable from configuration
	int numOfImg = spConfigGetNumOfImages(config, &msg);
	int numSimilarImg = spConfigGetSimilarImages(config, &msg);
	int spKNN = spConfigGetKNN(config, &msg);
	int dim = spConfigGetPCADim(config, &msg);
	//Init Logger
	char* loggerFile = (char*)malloc(sizeof(char)*MAXLEN);
	SP_LOGGER_LEVEL loglevel = spConfigGetLoggerLevel(config);
	msg = spConfigGetLoggerFilename(loggerFile,config);
	spLoggerCreate(loggerFile,loglevel);

	//Init and allocate after logger created.
	int* numOfFeats = (int*) malloc(sizeof(int)); //Temporary int pointer for the creating of .feat files
	if(numOfFeats==NULL){
		puts("FAIL_ALOC_MSG"); //not by the ben-dod. HEREEE
		return 1; //exit(1)
	}
	spLoggerPrintInfo(CONFIG_INIT_SUCCESS);

	//Creating new ImageProc instance by using configuration.
	ImageProc imageProc = ImageProc(config);
	spLoggerPrintInfo(IMGPROC_INIT_SUCCESS_MSG);

//	if(imageProc==NULL){
//		return 1; //exit(1)
//	}
    /**
    * Checking Extraction Mode State:
    * if extractionMode=true-->creating .feat files
    * else -->use feat files DB.
    */
	if (spConfigIsExtractionMode(config, &msg)) {
		if(msg != SP_CONFIG_SUCCESS){
			spLoggerPrintError(INVALID_EXTRAC_ARG_MSG,__FILE__,__func__,__LINE__);
			return 1; //exit(1)
		}
		for (int i = 0; i < numOfImg; i++) {
			msg = spConfigGetImagePath(imagePath, config, i);  //generate img path
			if (msg != SP_CONFIG_SUCCESS){
				spLoggerPrintError(INVALID_EXTRAC_ARG_MSG,__FILE__,__func__,__LINE__);
				return 1; //exit(1)
			}
		   //Extracting Image features for each image into SPPoint Array.
			resPoints = imageProc.getImageFeatures(imagePath, i, numOfFeats); //get img feat for the feat file
			if(resPoints==NULL){
				spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
				return 1; //exit(1)
			}

			//Auxilary function - extracting features values to .feat files.
			msg = createFeatFiles(config, imagePath, i, numOfFeats, resPoints); //CHECKK HERE/ LIRON GET CALM
			if(msg!=SP_CONFIG_SUCCESS){
				spLoggerPrintError(FEAT_FIlLES_INIT_FAIL_MSG,__FILE__,__func__,__LINE__);
				return 1; //exit(1)
			}
			//Destroying the SPPoint Array for reusing.
			for (int j = 0; j < *numOfFeats; j++)
				spPointDestroy(resPoints[j]);
			free(resPoints);
		}
	}
//	spLoggerPrintInfo(EXIT_MSG);

	//	Creation of point array with features of all images.
	totalResPoints = createTotalFeatArray(config, numOfImg, dim,&sizeOfTotalFeat); //CHECKK HERE/ LIRON GET CALM
	if(totalResPoints==NULL){
		spLoggerPrintError(FEAT_ARRAY_INIT_FAIL_MSG,__FILE__,__func__,__LINE__);
		return 1; //exit(1)
	}
	//Creation of KDArray with all point features - with image attribute
	kdarray = KdArrayInit(totalResPoints, sizeOfTotalFeat);

	//Destroying the SPPoint Total features Array.
	for (int i = 0; i < KDArrayGetSize(kdarray); i++)
		spPointDestroy(totalResPoints[i]);
	free(totalResPoints);

	//Checking Split Method from configuration file
	spConfigGetKDSplitMethod(&method, config);

	//Initializing KDTree DB
	root = kdTreeInit(kdarray, method, -1);

	//Search By Query
	puts("Please enter an image path:");
	fflush(NULL);
	scanf("%s", imagePath);
	fflush(NULL);
//strcpy(imagePath,"./queryA.png");
//strcpy(imagePath,"C:/dev/cproject/final/queryA.png");

	//Declaring countHits for count similar features per image.
	struct featHits* countHits = (featHits*) malloc(sizeof(featHits) * numOfImg);
	if(countHits==NULL){
		spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
		return 1; //exit(1)
	}
	//Request Query Image Path until 'exit' enter

	while (strcmp(imagePath, "<>") != 0) {
		for (int i = 0; i < numOfImg; i++) {
			countHits[i].hits = 0;
			countHits[i].index = i;
		}
		//Extracting Image features for query image into SPPoint Array.
		resPoints = imageProc.getImageFeatures(imagePath, BAD_INDEX,numOfFeats);
		if(resPoints==NULL){
			spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
			return 1; //exit(1)
		}

		//Searching for nearest neighbors of each feature
		for (int i = 0; i < *numOfFeats; i++) {
			//Creating new BPQueue MaxSize spKNN
			SPBPQueue bpq = spBPQueueCreate(spKNN);
			if(bpq==NULL){
				spLoggerPrintError(BPQUEUE_FAILURE,__FILE__,__func__,__LINE__);
				return 1; //exit(1)
			}
			KD_TREE_MSG treeMsg = kNearestNeighbors(bpq, root, resPoints[i]);
			if(treeMsg!=KD_TREE_SUCCESS){
				spLoggerPrintError(KD_TREE_FAIL_MSG,__FILE__,__func__,__LINE__);
				return 1; //exit(1)
			}
			for (int j = 0; j < spKNN; j++) {
				SP_BPQUEUE_MSG bpqMsg;
				int index = spBPQueuePeekIndex(bpq);
				countHits[index].hits++;
				bpqMsg = spBPQueueDequeue(bpq);
				if(bpqMsg!=SP_BPQUEUE_SUCCESS){
					spLoggerPrintError(BPQUEUE_FAILURE,__FILE__,__func__,__LINE__);
					return 1; //exit(1)
				}
			}
			spBPQueueDestroy(bpq);
		}

		//Sorting and getting the K best hits
		qsort((void*) countHits, numOfImg, sizeof(featHits), hitsComp);

		//	Checking Show Image State
		if (spConfigMinimalGui(config, &msg)) {
			for (int i = 0; i < numSimilarImg; i++) {
				msg = spConfigGetImagePath(imagePath, config,
						countHits[i].index);
				if(msg!=SP_CONFIG_SUCCESS){
					spLoggerPrintError(CONFIG_FAIL_MSG,__FILE__,__func__,__LINE__);
					return 1; //exit(1)
				}
				imageProc.showImage(imagePath);
			}
		} else {
			printf("Best candidates for - %s - are:\n", imagePath);
			for (int i = 0; i < numSimilarImg; i++){
				spConfigGetImagePath(imagePath,config,countHits[i].index);
				printf("%s\n",imagePath);
			}
		}

		puts("Please enter an image path:");
		fflush(NULL);
		scanf("%s", imagePath);
		fflush(NULL);
		strcpy(imagePath,"<>");
	}
	free(loggerFile);
	spLoggerDestroy();
	free(countHits);
	KDTreeDestroy(root);
	free(imagePath);
	free(numOfFeats);
	return 0;
}

