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
	SP_CONFIG_MSG msg;   //Temporary msg
	SPConfig config = spConfigCreate(filename, &msg);   //Create config object
	char* path = (char*) malloc(sizeof(char) * MAX_LENGTH_PATH);  //Temporary path
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SPPoint* resPoints;  //Temporary SPPoint for the creating of .feat files
	int* numOfFeats = (int*) malloc(sizeof(int)); //Temporary int pointer for the creating of .feat files
	sp::ImageProc imgP(config);   //Creates new instance if ImageProc
//	toString(config);
	//Enum Values comparsion - need to get done
	if (spConfigIsExtractionMode(config, &msg) && msg == 10) {  //if extractionMode=true-->creating .feat files
		for (int i = 0; i < spConfigGetNumOfImages(config, &msg) && msg == 10;
				i++) {
			msg=genarateFeatPath(path, config, i);  //generate feat path
			if(msg!=SP_CONFIG_SUCCESS)
				continue; //????????????????
			tempFile = fopen(path, "w+");   //creating feat file
			msg = spConfigGetImagePath(path, config, i);  //generate img path
			if(msg!=SP_CONFIG_SUCCESS)
				continue; //????????????????
			resPoints = imgP.getImageFeatures(path, i, numOfFeats);  //get img feat for the feat file
			//writing all relevant details for feat file
			fprintf(tempFile,"%d \n", (const void*)numOfFeats[0] );
			int dim = spConfigGetPCADim(config,&msg)-1;
			for (int j = 0; j < numOfFeats[0]; j++) {
				double temp=0;
				for (int k = 0; k <dim ; k++) {
					temp = spPointGetAxisCoor(resPoints[j], k);
					fprintf(tempFile,"%f \t", temp );

				}
				temp = spPointGetAxisCoor(resPoints[j], dim);
				fprintf(tempFile,"%f \n", temp );
			}
			fclose(tempFile);  // closing the feat file
		}

	}
//	puts("Please enter an image path:");
//	scanf("%s",&path);


	return 0;
}

