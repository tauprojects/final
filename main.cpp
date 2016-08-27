/*
 * main.cpp
 *
 *  Created on: Jul 23, 2016
 *      Author: Liron
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SPImageProc.h>

#define MAX_LENGTH_PATH 1024

int main() {
	const char* filename = "C:/dev/cproject/final/myconfig.config";
//	SP_CONFIG_MSG msg ;
	SP_CONFIG_MSG pathmsg;
	SPConfig config = spConfigCreate(filename, &pathmsg);
	char* path = (char*) malloc(sizeof(char) * MAX_LENGTH_PATH);
	char* sufFeat = (char*) malloc(sizeof(char) * 10);
	strcpy(sufFeat, ".feat");
	FILE *tempFile;
	SPPoint* resPoints;
	int* numOfFeats = (int*) malloc(sizeof(int));
	sp::ImageProc imgP(config);   //Creates new instance if ImageProc
//	toString(config);
	//Enum Values comparsion - need to get done
	if (spConfigIsExtractionMode(config, &pathmsg) && pathmsg == 10) {
		for (int i = 0; i < spConfigGetNumOfImages(config, &pathmsg) && pathmsg == 10;
				i++) {

			genarateFeatPath(path, config, i);
			tempFile = fopen(path, "w+");
			pathmsg = spConfigGetImagePath(path, config, i);
			// check pathmsg
			resPoints = imgP.getImageFeatures(path, i, numOfFeats);
			fprintf(tempFile,"%d \n", (const void*)numOfFeats[0] );
			int dim = spConfigGetPCADim(config,&pathmsg)-1;
			for (int j = 0; j < numOfFeats[0]; j++) {
				double temp=0;
				for (int k = 0; k <dim ; k++) {
					temp = spPointGetAxisCoor(resPoints[j], k);
					fprintf(tempFile,"%f \t", temp );

				}
				temp = spPointGetAxisCoor(resPoints[j], dim);
				fprintf(tempFile,"%f \n", temp );
			}
			fclose(tempFile);
		}

	}
//	puts("Please enter an image path:");
//	scanf("%s",&path);


	return 0;
}

