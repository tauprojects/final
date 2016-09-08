#define MAXLEN 1025

#include<main_aux.h>

void createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoints ){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
//	msg = genarateFeatPath(path, config, i);  //generate feat path
	spConfigGetFeatsPath(path, config, i);


	if (msg != SP_CONFIG_SUCCESS); //Do Somthing
	tempFile = fopen(path, "wt");   //creating feat file
	//writing all relevant details for feat file
	fprintf(tempFile, "%d \n", (const void*) numOfFeats[0]);
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
}

SPPoint* createTotalFeatArray(SPConfig config, int numOfImg,int dim,int* sizeOfTotalFeat){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
	char* path = (char*) malloc(sizeof(char) * MAXLEN);
	int size = 0,j,k;
	int numOfFeat;
	double* valArr = (double*)malloc(sizeof(double)*dim);
	char* tempChar = (char*)malloc(sizeof(char)*1025);
	for (int i = 0; i < numOfImg; i++) {
		numOfFeat = 0;
		spConfigGetFeatsPath(path, config, i);
		tempFile = fopen(path, "rt");   //getting feat file
		fscanf(tempFile, "%s", tempChar);
		numOfFeat = atoi(tempChar);//Verifiy conversion from char text to int.
		printf("SizeTotalArray: %d ",size + numOfFeat);
		fflush(NULL);
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
	size=0;
	for (int i = 0; i < numOfImg; i++) {
		numOfFeat = 0;
		spConfigGetFeatsPath(path, config, i);
		tempFile = fopen(path, "rt");   //getting feat file
		fscanf(tempFile, "%s", tempChar);
		numOfFeat = atoi(tempChar);//Verifiy conversion from char text to int.
		fflush(NULL);
		for (j = size; j < size+numOfFeat; j++) {
			for ( k = 0; k < dim; k++) {
				fscanf(tempFile, "%s", tempChar);
				valArr[k] = atof(tempChar);
				if(valArr[k]==0){
						puts("ERROR");
						fflush(NULL);
					}
			}
			totalResPoints[j] = spPointCreate(valArr, dim, i);
			printf("\nInside CreateTotalFeat: %d with path: %s + numFeat: %d , k: %d ",i,path,numOfFeat,j);
			printf(" Point Index: %d ",spPointGetIndex(totalResPoints[j]));
			fflush(NULL);
		}
		size+= numOfFeat;
		fclose(tempFile);  // closing the feat file
	}
	free(valArr);
	free(path);
	free(tempChar);
	printf(" Point size:  %d ",size);
	fflush(NULL);

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


