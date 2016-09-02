/*
 * main.cpp
 *
 *  Created on: Jul 23, 2016
 *      Author: Liron
 */

#include <iostream>
#include <SPImageProc.h>
extern "C"{
	#include <main_aux.h>
}


int main() {
	const char* filename = "C:/dev/cproject/final/myconfig.config"; //Argument***
	SP_CONFIG_MSG msg;   //Temporary msg
	SPConfig config = spConfigCreate(filename, &msg);   //Create config object
	char* path = (char*) malloc(sizeof(char) * MAX_LENGTH_PATH); //Temporary path
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SPPoint* resPoints;  //Temporary SPPoint for the creating of .feat files

	sp::ImageProc imgP(config);   //Creates new instance if ImageProc
//	Total Featrues SPPoints Array
	SPPoint* totalResPoints = (SPPoint*) malloc(sizeof(SPPoint));

	int sizeOfTotalFeat;
	int dim = spConfigGetPCADim(config, &msg);
	KDArray kdarray;
	KDTreeNode root;
	int numSimilarImg = spConfigGetNumSimilarImages(config, &msg);
	int spKNN = spConfigGetspKNN(config, &msg);
	int numOfImg = spConfigGetNumOfImages(config, &msg);
	int* numOfFeats = (int*) malloc(sizeof(int)); //Temporary int pointer for the creating of .feat files
	SPBPQueue bpq = spBPQueueCreate(spKNN);
	//	toString(config);


	//Enum Values comparsion - need to get done
	//Extraction Mode - Creation of .feat files.
	if (spConfigIsExtractionMode(config, &msg) && msg == 10) { //if extractionMode=true-->creating .feat files
		for (int i = 0; i < numOfImg && msg == 10;i++) {
			msg = spConfigGetImagePath(path, config, i);  //generate img path
			if (msg != SP_CONFIG_SUCCESS)
				continue; //????????????????
			resPoints = imgP.getImageFeatures(path, i, numOfFeats); //get img feat for the feat file
			createFeatFiles(config,path,i,numOfFeats,resPoints);

		}

	}


	//	Creation of point array with features of all images.
	sizeOfTotalFeat = createTotalFeatArray(config,numOfImg,path,totalResPoints,dim);
	//Creation of KDArray with all point features - with image attribute
	KdArrayInit(kdarray, totalResPoints, sizeOfTotalFeat);
	//Creation of KDTree DB
	kdTreeInit(kdarray, getSpSplitMethod(config), -1);

	//Search By Query
	puts("Please enter an image path:");
	scanf("%s", &path);
	while (strcmp(path, "exit") != 0) {
		struct featHits* countHits = (featHits*) malloc(
				sizeof(featHits) * numOfImg);
		// Init of Hits
		for (int i = 0; i < numOfImg; i++) {
			countHits[i].hits = 0;
			countHits[i].index = i;
		}
		resPoints = imgP.getImageFeatures(path, BAD_INDEX, numOfFeats); //get img feat for the feat file

		for (int i = 0; i < numOfFeats[0]; i++) {
			kNearestNeighbors(bpq, root, resPoints[i]);
			for (int j = 0; j < spConfigGetspKNN(config, &msg); j++) {
				int index = spListElementGetIndex(spBPQueuePeek(bpq)); //Creates Copy - Remember to destroy.
				countHits[index].hits++;
				spBPQueueDequeue(bpq);
			}
		}


//		Sorting and getting the K best hits
		qsort((void*) countHits, numOfImg, sizeof(featHits), hitsComp);


//		Show Image
		if (spConfigMinimalGui(config, &msg)) {
			for (int i = 0; i < numSimilarImg; i++) {
				msg = spConfigGetImagePath(path, config, countHits[i].index);
				imgP.showImage(path);
			}
		} else {
			for (int i = 0; i < numSimilarImg; i++) {
				printf("%d Printing the Pic", countHits[i].index);
			}
		}

		puts("Please enter an image path:");
		scanf("%s", &path);
	}
	//Exit Msg-Logger
//	destroyAll();
	return 0;
}


