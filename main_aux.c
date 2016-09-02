

#include<main_aux.h>


void createFeatFiles(SPConfig config, char* path,int i,int* numOfFeats,SPPoint* resPoints ){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
	msg = genarateFeatPath(path, config, i);  //generate feat path
	if (msg != SP_CONFIG_SUCCESS); //Do Somthing
	tempFile = fopen(path, "w+");   //creating feat file


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


int createTotalFeatArray(SPConfig config, int numOfImg,char* path,SPPoint* totalResPoints,int dim){
	FILE *tempFile;  //Temporary file for the creating of .feat files
	SP_CONFIG_MSG msg;
	int size = 0;
	for (int i = 0; i < numOfImg; i++) {
		int numOfFeat = 0;
		msg = genarateFeatPath(path, config, i);  //generate feat path
		if (msg != SP_CONFIG_SUCCESS)
			continue; //????????????????
		tempFile = fopen(path, "r");   //getting feat file
		fscanf(tempFile, "%d", numOfFeat); //Verifiy conversion from char text to int.
		size = size + numOfFeat;
		realloc(totalResPoints, sizeof(SPPoint) * size);
		for (int j = 1; j < numOfFeat; j++) {
			char tempChar[10];  //Size - Some Constant or known value
			double valArr[dim];
			for (int k = 0; k < dim; k++) {
				fscanf(tempFile, "%s", tempChar);
				valArr[k] = atof(tempChar);
			}
			totalResPoints[size + j - 1] = spPointCreate(valArr, dim, i);
		}
		fclose(tempFile);  // closing the feat file
	}
	return size;
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


