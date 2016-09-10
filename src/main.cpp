#define INVALID_CMD_LINE_ERROR "Invalid command line : use -c <config_filename>\n"
#define DEFAULT_CONFIG_FILENAME "spcbir.config"
#define CONFIG_COULDNT_OPEN_ERROR "The configuration file %s couldn’t be open\n"
#define DEFAULT_CONFIG_COULDNT_OPEN_ERROR "The default configuration file spcbir.config couldn’t be open\n"
#define MAX_PATH_LENGTH 1025
#define MAXLEN 1025

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
	SP_CONFIG_MSG msg;
	SPConfig config;
	char imagePath[MAX_PATH_LENGTH];
	int nImages, *nFeaturesPerImage;
	puts("checkpoint 1");
	fflush(NULL);
	if (argc == 1) {
		config = spConfigCreate(DEFAULT_CONFIG_FILENAME, &msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(DEFAULT_CONFIG_COULDNT_OPEN_ERROR);
			return 0;
		} else if (msg != SP_CONFIG_SUCCESS) {
			return 0;
			/**choose what to fucking do about it*/
		}
	} else if (argc == 3 && strcmp(argv[1], "-c") == 0) {
		config = spConfigCreate(argv[2], &msg);
		if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {
			printf(CONFIG_COULDNT_OPEN_ERROR, argv[2]);
			return 0;
		} else if (msg != SP_CONFIG_SUCCESS) {
			return 0; //A+B
		}
	} else {
		printf(INVALID_CMD_LINE_ERROR);
		return 0;
	}
	puts("checkpoint 2");
	fflush(NULL);
	toString(config);
	nImages = spConfigGetNumOfImages(config, &msg); //A+B
	nFeaturesPerImage = (int*) malloc(nImages * sizeof(int));
	ImageProc imageProc = ImageProc(config);
	puts("checkpoint 3 - imageProc C++ Object Created");
	fflush(NULL);
	SPPoint* totalResPoints;
	int sizeOfTotalFeat;
	int dim = spConfigGetPCADim(config, &msg);
	KDArray kdarray;
	KDTreeNode root;
	int numSimilarImg = spConfigGetSimilarImages(config, &msg);
	int spKNN = spConfigGetKNN(config, &msg);
	int numOfImg = spConfigGetNumOfImages(config, &msg);
	int* numOfFeats = (int*) malloc(sizeof(int)); //Temporary int pointer for the creating of .feat files
	SPBPQueue bpq = spBPQueueCreate(spKNN);
	//	toString(config);
	char temp[MAX_PATH_LENGTH];
	SPPoint* resPoints;
	puts("checkpoint 4 - Before the loop of feat Files");
	fflush(NULL);
	if (spConfigIsExtractionMode(config, &msg) && msg == 10) { //if extractionMode=true-->creating .feat files
		for (int i = 0; i < numOfImg; i++) {
			printf("Inside the FeatFiles Loop: Itr Num: %d", i);
			fflush(NULL);
			msg = spConfigGetImagePath(temp, config, i);  //generate img path
			puts(temp);
			fflush(NULL);
//			if (msg != SP_CONFIG_SUCCESS)
//				continue; //????????????????
			resPoints = imageProc.getImageFeatures(temp, i, numOfFeats); //get img feat for the feat file
			printf("Inside the FeatFiles Loop After IMGPROC: Itr Num: %d", i);
			fflush(NULL);
			createFeatFiles(config, temp, i, numOfFeats, resPoints);
			for (int j = 0; j < *numOfFeats; j++) {
				spPointDestroy(resPoints[j]);
			}
			free(resPoints);
		}
	}
	puts("checkpoint 4 - Before the loop of feat Files");
	fflush(NULL);
	//	Creation of point array with features of all images.
	totalResPoints = createTotalFeatArray(config, numOfImg, dim,
			&sizeOfTotalFeat);
	puts("\n checkpoint 5 - After Creating TotalResPoints Array");
	fflush(NULL);
	fflush(NULL);

	//Creation of KDArray with all point features - with image attribute
	kdarray = KdArrayInit(totalResPoints, sizeOfTotalFeat);
	printf("\n\nCheck Print %d \n ", KDArrayGetSize(kdarray));
	fflush(NULL);
	for (int i = 0; i < KDArrayGetSize(kdarray); i++) {
		spPointDestroy(totalResPoints[i]);
	}
	free(totalResPoints);
	//Creation of KDTree DB
	SP_SPLIT_METHOD method;
	spConfigGetKDSplitMethod(&method, config);
	puts("checkpoint 6 - After Creating KDarray");
	fflush(NULL);

	root = kdTreeInit(kdarray, method, -1);
	fflush(NULL);
	puts("checkpoint 7 - After Creating KDTree");
	fflush(NULL);
	spKDArrayDestroy(kdarray);
//---------------------------------------------------------------------//
	//Search By Query
	puts("Please enter an image path:");
	fflush(NULL);
	scanf("%s", &imagePath);
	fflush(NULL);
	while (strcmp(imagePath, "exit") != 0) {
		struct featHits* countHits = (featHits*) malloc(
				sizeof(featHits) * numOfImg);
//				// Init of Hits
		for (int i = 0; i < numOfImg; i++) {
			countHits[i].hits = 0;
			countHits[i].index = i;
		}
		resPoints = imageProc.getImageFeatures(imagePath, BAD_INDEX,
				numOfFeats); //get img feat for the feat file
		puts("CheckPoint 8 - after ImgProc query");
		fflush(NULL);
		//work until here,thanks.
		for (int i = 0; i < *numOfFeats; i++) {
			kNearestNeighbors(bpq, root, resPoints[i]);

			for (int j = 0; j < spKNN; j++) {
				int index = spBPQueuePeekIndex(bpq); //Creates Copy - Remember to destroy.
				countHits[index].hits++;
				spBPQueueDequeue(bpq);
			}
		}
		fflush(NULL);
		puts("CheckPoint 9 - after Nearesttt query");
		fflush(NULL);
		//		Sorting and getting the K best hits
		qsort((void*) countHits, numOfImg, sizeof(featHits), hitsComp);

		//		Show Image
		if (spConfigMinimalGui(config, &msg)) {
			for (int i = 0; i < numSimilarImg; i++) {
				msg = spConfigGetImagePath(imagePath, config,
						countHits[i].index);
				imageProc.showImage(imagePath);
				printf("%d", i);
			}
		} else {
			for (int i = 0; i < numSimilarImg; i++) {
				printf("%d Printing the Pic\n", countHits[i].index);
			}
		}

		fflush(NULL);
		puts("Please enter an image path:");
		fflush(NULL);
		scanf("%s", &imagePath);
		fflush(NULL);
	}
	//Exit Msg-Logger
	//	destroyAll();
	return 0;
}

