#define MAXLEN 1025

#include "main_aux.h"

SP_CONFIG_MSG createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoints ){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
	msg = spConfigGetFeatsPath(path, config, i);
	if(msg != SP_CONFIG_SUCCESS){
		spLoggerPrintError(CANNOT_OPEN_FILE_MSG,__FILE__,__func__,__LINE__);
		return msg;
	}
	tempFile = fopen(path, "wt");   //creating feat file
	if(!tempFile){
		spLoggerPrintError(CANNOT_OPEN_FILE_MSG,__FILE__,__func__,__LINE__);
		return SP_CONFIG_CANNOT_OPEN_FILE;
	}
	//writing all relevant details for feat file
	fprintf(tempFile, "%d \n", numOfFeats[0]);
	int dim = spConfigGetPCADim(config, &msg) - 1;
	for (int j = 0; j < numOfFeats[0]; j++) {
		double temp = 0;
		for (int k = 0; k < dim; k++) {
			temp = spPointGetAxisCoor(resPoints[j], k);
			fprintf(tempFile, "%f \t", temp);
		}
		temp = spPointGetAxisCoor(resPoints[j], dim);
		fprintf(tempFile, "%f \n", temp);
	}
	fclose(tempFile);  // closing the feat file
	return SP_CONFIG_SUCCESS;
}

SPPoint* createTotalFeatArray(SPConfig config, int numOfImg,int dim,int* sizeOfTotalFeat){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
	char* path = (char*) malloc(sizeof(char) * MAXLEN);
	if(path==NULL){
		spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
		return NULL; //exit(1)
	}
	int size = 0,j,k,numOfFeat;
	double* valArr = (double*)malloc(sizeof(double)*dim);
	if(valArr==NULL){
		spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
		return NULL; //exit(1)
	}
	char* tempChar = (char*)malloc(sizeof(char)*MAXLEN);
	if(tempChar==NULL){
		spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
		return NULL; //exit(1)
	}
	spLoggerPrintInfo("Creating Total Feat Array...");
	for (int i = 0; i < numOfImg; i++) {
		numOfFeat = 0;
		msg = spConfigGetFeatsPath(path, config, i);
		if(msg!=SP_CONFIG_SUCCESS){
			return NULL;
		}
		tempFile = fopen(path, "rt");   //getting feat file
		if(!tempFile){
			spLoggerPrintError(CANNOT_OPEN_FILE_MSG,__FILE__,__func__,__LINE__);
			return NULL;
		}
		fscanf(tempFile, "%s", tempChar);
		numOfFeat = atoi(tempChar);  //Verify conversion from char text to int.
		for (j = size; j < size+numOfFeat; j++) {
			for ( k = 0; k < dim; k++) {
				fscanf(tempFile, "%s", tempChar);
				valArr[k] = atof(tempChar);
			}
		}
		size = size + numOfFeat;
		fclose(tempFile);  // closing the feat file
	}
	*sizeOfTotalFeat=size;
	SPPoint* totalResPoints = (SPPoint*)malloc(sizeof(SPPoint) *size);
	if(totalResPoints==NULL){
		spLoggerPrintError(FAIL_ALOC_MSG,__FILE__,__func__,__LINE__);
		return NULL; //exit(1)
	}
	size=0;
	for (int i = 0; i < numOfImg; i++) {
		numOfFeat = 0;
		spConfigGetFeatsPath(path, config, i);
		tempFile = fopen(path, "rt");   //getting feat file
		if(!tempFile){
			spLoggerPrintError(CANNOT_OPEN_FILE_MSG,__FILE__,__func__,__LINE__);

			return NULL;
		}
		fscanf(tempFile, "%s", tempChar);
		numOfFeat = atoi(tempChar);//Verify conversion from char text to int.
		fflush(NULL);
		for (j = size; j < size+numOfFeat; j++) {
			for ( k = 0; k < dim; k++) {
				fscanf(tempFile, "%s", tempChar);
				valArr[k] = atof(tempChar);
				if(valArr[k]==0){
					spLoggerPrintError(ERROR_MSG,__FILE__,__func__,__LINE__);
						fflush(NULL);
					}
			}
			totalResPoints[j] = spPointCreate(valArr, dim, i);
		}
		size+= numOfFeat;
		fclose(tempFile);  // closing the feat file
	}
	free(valArr);
	free(path);
	free(tempChar);
	return totalResPoints;
}

int hitsComp(const void * A, const void* B) {
	int res = ((struct featHits*) A)->hits - ((struct featHits*) B)->hits;
	if (res == 0) {
		return ((struct featHits*) A)->index - ((struct featHits*) B)->index;
	}
	if (res < 0) {
		return 1;
	} else {
		return -1;
	}
}


