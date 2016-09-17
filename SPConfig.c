#include "SPConfig.h"
#include <assert.h>

struct sp_config_t {
	char spImagesDirectory[MAXLEN];
	char spImagesPrefix[MAXLEN];
	char spImagesSuffix[MAXLEN];
	int spNumOfImages;
	int spPCADimension;
	char spPCAFilename[MAXLEN];
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	SP_SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN;
	bool spMinimalGui;
	int spLoggerLevel;
	char spLoggerFilename[MAXLEN];
};

#define getName(var) #var

#define DEFAULT_PCADIM 20
#define DEFAULT_PCAFILE "pca.yml"
#define DEFAULT_FEATNUM 100
#define DEFAULT_EXTRACTIONMODE true
#define DEFAULT_MINIMALGUI false
#define DEFAULT_SIMILARIMGNUM 1
#define DEFAULT_KNN 1
#define DEFAULT_KD_SPLITMETHOD MAX_SPREAD
#define DEFAULT_LOGGERLEVEL 3
#define DEFAULT_LOGGERFILE "stdout"

bool isNum(char* str);
bool removeSpaces(char* source);
bool assignVal(SPConfig config,SP_CONFIG_MSG* msg,char* key,char* val,const char* filename,int lineNum);
bool checkingConfigFile(SPConfig config,SP_CONFIG_MSG* msg,int lineNum,const char* filename);
bool hasSpace(char* source);
void spConfigDestroy(SPConfig config);
bool isCommentLine(char tempLine[]);
void defualtVal(SPConfig config);
void printError(const char* filename, int lineNum, SP_CONFIG_ERROR error,char* param);

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	SPConfig config=(SPConfig) malloc(sizeof(*config));
	if(config==NULL){return NULL;}    //checking allocation
	*msg=SP_CONFIG_SUCCESS;
	if(filename==NULL){return NULL;}  //checking valid file name
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {return NULL;}    //checking file
	defualtVal(config);               //set default vals on the config file
	//temporary variables
	int lineNum=0;
	char key[MAXLEN];
	char val[MAXLEN];
	char tempLine[MAXLEN];
	char* temp;
	bool isValidLine=true;
	while (fgets(tempLine,MAXLEN, fp) != NULL) {
		if(isCommentLine(tempLine)){
			lineNum++;
			continue;
		}
		tempLine[strlen(tempLine)-1]='\0';
		temp = strchr(tempLine, '=');
		if(temp==NULL){ isValidLine=false;}
		else{
			*temp='\0';
			strcpy(key,tempLine);
			strcpy(val,temp+1);
			isValidLine= removeSpaces(key) && removeSpaces(val);
		}
		if(!isValidLine){  //checking line
			*msg = SP_CONFIG_INVALID_STRING;
			spConfigDestroy(config);
			return NULL;
		}
		if(!assignVal(config,msg,key,val,filename,lineNum)){
			spConfigDestroy(config);
			return NULL;
		}
		lineNum++;
	}
	if(!checkingConfigFile(config,msg,lineNum,filename)){
		spConfigDestroy(config);
		return NULL;
	}
	return config;
}

bool removeSpaces(char* source){
  char* i = source;
  char* j = source;
  while(*j != 0){
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
  return true;
}

bool assignVal(SPConfig config, SP_CONFIG_MSG* msg, char* key, char* val,
		const char* filename, int lineNum) {
	*msg = SP_CONFIG_SUCCESS; //because of unused warning
	if (strcmp(key, "spImagesDirectory") == 0) {
		if (hasSpace(val)) {
			printError(filename, lineNum, ERROR_VALUE, "spImagesDirectory");
			return false;
		} else
			strcpy(config->spImagesDirectory, val);
	}
	else if (strcmp(key, "spImagesPrefix") == 0) {
		if (hasSpace(val)) {
			printError(filename, lineNum, ERROR_VALUE, "spImagesPrefix");
			return false;
		} else
			strcpy(config->spImagesPrefix, val);
	}
	else if (strcmp(key, "spImagesSuffix") == 0) {
		if (strcmp(val, ".bmp") == 0 || strcmp(val, ".jpg") == 0
				|| strcmp(val, ".png") == 0 || strcmp(val, ".gif") == 0) {
			strcpy(config->spImagesSuffix, val);
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spImagesSuffix");
			return false;
		}
	}
	else if (strcmp(key, "spNumOfImages") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal <= 0){
				printError(filename, lineNum, ERROR_VALUE, "spNumOfImages");
				return false;
			}else
				config->spNumOfImages = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spNumOfImages");
			return false;
		}
	}
	else if (strcmp(key, "spPCADimension") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal < 10 || intVal > 128){
				printError(filename, lineNum, ERROR_VALUE, "spPCADimension");
				return false;
			}else
				config->spPCADimension = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spPCADimension");
			return false;
		}
	}
	else if (strcmp(key, "spPCAFilename") == 0) {
		if (hasSpace(val)) {
			printError(filename, lineNum, ERROR_VALUE, "spPCAFilename");
			return false;
		} else
			strcpy(config->spPCAFilename, val);
	}
	else if (strcmp(key, "spNumOfFeatures") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal <= 0){
				printError(filename, lineNum, ERROR_VALUE, "spNumOfFeatures");
				return false;
			} else
				config->spNumOfFeatures = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spNumOfFeatures");
			return false;
		}
	} else if (strcmp(key, "spExtractionMode") == 0) {
		//lowercase
//		strToLower(val);
		if (strcmp(val, "true") == 0)
			config->spExtractionMode = true;
		else if (strcmp(val, "false") == 0)
			config->spExtractionMode = false;
		else{
			printError(filename, lineNum, ERROR_VALUE, "spExtractionMode");
			return false;
		}
	} else if (strcmp(key, "spNumOfSimilarImages") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal <= 0){
				printError(filename, lineNum, ERROR_VALUE,
						"spNumOfSimilarImages");
				return false;
			}else
				config->spNumOfSimilarImages = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spNumOfSimilarImages");
			return false;
		}
	}

	else if (strcmp(key, "spKDTreeSplitMethod") == 0) {
		if (strcmp(val, "RANDOM") == 0)
			config->spKDTreeSplitMethod = RANDOM;
		else if (strcmp(val, "INCREMENTAL") == 0)
			config->spKDTreeSplitMethod = INCREMENTAL;
		else if (strcmp(val, "MAX_SPREAD") == 0)
			config->spKDTreeSplitMethod = MAX_SPREAD;
		else{
			printError(filename, lineNum, ERROR_VALUE, "spKDTreeSplitMethod");
			return false;
		}
	}

	else if (strcmp(key, "spKNN") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal <= 0){
				printError(filename, lineNum, ERROR_VALUE, "spKNN");
				return false;
			}else
				config->spKNN = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spKNN");
			return false;
		}
	}

	else if (strcmp(key, "spMinimalGui") == 0) {
		//lowercase
//		strToLower(val);
		if (strcmp(val, "true") == 0)
			config->spMinimalGui = true;
		else if (strcmp(val, "false") == 0)
			config->spMinimalGui = false;
		else{
			printError(filename, lineNum, ERROR_VALUE, "spMinimalGui");
			return false;
		}
	}

	else if (strcmp(key, "spLoggerLevel") == 0) {
		if (isNum(val)) {
			int intVal = atoi(val);
			if (intVal <= 0 || intVal > 4){
				printError(filename, lineNum, ERROR_VALUE, "spLoggerLevel");
				return false;
			}else
				config->spLoggerLevel = intVal;
		} else{
			printError(filename, lineNum, ERROR_VALUE, "spLoggerLevel");
			return false;
		}
	}
	else if (strcmp(key, "spLoggerFilename") == 0) {
		if (hasSpace(val)) {
			printError(filename, lineNum, ERROR_VALUE, "spLoggerFilename");
			return false;
		} else {
			strcpy(config->spLoggerFilename, val);
		}
	}
return true;
}

bool checkingConfigFile(SPConfig config,SP_CONFIG_MSG* msg,int lineNum,const char* filename){
	bool flag=true;
	if (strlen(config->spImagesDirectory) == 0) {
		printError(filename, lineNum, UNSET_PARAM, "spImagesDirectory");
		*msg = SP_CONFIG_MISSING_DIR;
		spConfigDestroy(config);
		flag=false;
	}
	if (strlen(config->spImagesPrefix) == 0) {
		printError(filename, lineNum, UNSET_PARAM, "spImagesPrefix");
		*msg = SP_CONFIG_MISSING_PREFIX;
		spConfigDestroy(config);
		flag=false;
	}
	if (strlen(config->spImagesSuffix) == 0) {
		printError(filename, lineNum, UNSET_PARAM, "spImagesSuffix");
		*msg = SP_CONFIG_MISSING_SUFFIX;
		spConfigDestroy(config);
		flag=false;
	}
	if (config->spNumOfImages == 0) {
		printError(filename, lineNum, UNSET_PARAM, "spNumOfImages");
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		spConfigDestroy(config);
		flag=false;
	}
	return flag;
}

void spConfigDestroy(SPConfig config){
	if (config == NULL)
		return;
	spLoggerDestroy();
	free(config);
}

bool isCommentLine(char* tempLine){
	while (isspace(*tempLine))
		tempLine++;
	if (*tempLine == '#' || *tempLine == '\0')
		return true;
	return false;
}

bool hasSpace(char* source){
	while (!isspace(*source) && *source != '\0')
		source++;
	if(*source == '\0')
		return false;
	return true;
}

void defualtVal(SPConfig config){
	config->spImagesDirectory[0] = '\0';
	config->spImagesPrefix[0] = '\0';
	config->spImagesSuffix[0] = '\0';
	config->spNumOfImages=0;
	config->spPCADimension=DEFAULT_PCADIM;
	strcpy(config->spPCAFilename,DEFAULT_PCAFILE);
	config->spNumOfFeatures=DEFAULT_FEATNUM;
	config->spExtractionMode=DEFAULT_EXTRACTIONMODE;
	config->spNumOfSimilarImages=DEFAULT_SIMILARIMGNUM;
	config->spKDTreeSplitMethod=DEFAULT_KD_SPLITMETHOD;
	config->spKNN=DEFAULT_KNN;
	config->spMinimalGui=DEFAULT_MINIMALGUI;
	config->spLoggerLevel=DEFAULT_LOGGERLEVEL;
	strcpy(config->spLoggerFilename,DEFAULT_LOGGERFILE);
}

void printError(const char* filename, int lineNum, SP_CONFIG_ERROR error,char* param){
	printf("File: %s\nLine: %d\nMessage: ", filename, lineNum);
	switch(error){
	case ERROR_VALUE:
		printf("Invalid value - constraint not met\n");
		break;
	case ERROE_LINE:
		printf("Invalid configuration line\n");
		break;
	case UNSET_PARAM:
		printf("Parameter %s is not set\n", param);
		break;
	}
}

bool isNum(char* str) {
	while (isdigit(*str))
		str++;
	if (*str != '\0')
		return false;
	return true;
}

int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spKNN;
}

SP_CONFIG_MSG spConfigGetKDSplitMethod(SP_SPLIT_METHOD* method, const SPConfig config){
	if ( method == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	*method=config->spKDTreeSplitMethod;
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int index) {
	if (imagePath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	strcpy(imagePath, "");
	char* i = (char*) malloc(sizeof(char) * MAXLEN);
	if (i == NULL) {
		fflush(NULL);
		return SP_CONFIG_ALLOC_FAIL;
	}
	sprintf(i, "%d", index);
	strcat(imagePath, config->spImagesDirectory);
	strcat(imagePath, config->spImagesPrefix);
	strcat(imagePath, i);
	strcat(imagePath, config->spImagesSuffix);
	free(i);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetFeatsPath(char* featPath, const SPConfig config,int index){
	if (config == NULL || featPath == NULL)
			return SP_CONFIG_INVALID_ARGUMENT;
	if (index >= config->spNumOfImages)
			return SP_CONFIG_INVALID_ARGUMENT;
	strcpy(featPath, "");
	char* i = (char*) malloc(sizeof(char) * MAXLEN);
	if (i == NULL) {
		fflush(NULL);
		return SP_CONFIG_ALLOC_FAIL;
	}
	sprintf(i, "%d", index);
	strcat(featPath, config->spImagesDirectory);
	strcat(featPath, config->spImagesPrefix);
	strcat(featPath, i);
	strcat(featPath, ".feat");
	free(i);
	return SP_CONFIG_SUCCESS;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spPCADimension;
}
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (config == NULL || pcaPath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	strcat(pcaPath, config->spImagesDirectory);
	strcat(pcaPath, config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}
int spConfigGetSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfSimilarImages;
}
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfFeatures;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfImages;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return true; //Handling the Error in main function.
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spExtractionMode)
		return true;
	else
		return false;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spMinimalGui)
		return true;
	else
		return false;
}

SP_CONFIG_MSG spConfigGetLoggerFilename(char* logger, const SPConfig config){
	if (logger == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	strcpy(logger, config->spLoggerFilename);
	return SP_CONFIG_SUCCESS;
}

SP_LOGGER_LEVEL spConfigGetLoggerLevel(const SPConfig config){
	return config->spLoggerLevel;
}

void toString(SPConfig config){
	fflush(NULL);
	printf("key: %s \t value: %s\n",getName(config->spImagesDirectory),config->spImagesDirectory);
	printf("key: %s \t value: %s\n",getName(config->spImagesPrefix),config->spImagesPrefix);
	printf("key: %s \t value: %s\n",getName(config->spImagesSuffix),config->spImagesSuffix);
	printf("key: %s \t value: %d\n",getName(config->spNumOfImages),config->spNumOfImages);
	printf("key: %s \t value: %d\n",getName(config->spPCADimension),config->spPCADimension);
	printf("key: %s \t value: %s\n",getName(config->spPCAFilename),config->spPCAFilename);
	printf("key: %s \t value: %d\n",getName(config->spNumOfFeatures),config->spNumOfFeatures);
	printf("key: %s \t value: %d\n",getName(config->spExtractionMode),config->spExtractionMode);
	printf("key: %s \t value: %d\n",getName(config->spNumOfSimilarImages),config->spNumOfSimilarImages);
	printf("key: %s \t value: %d\n",getName(config->spKDTreeSplitMethod),config->spKDTreeSplitMethod);
	printf("key: %s \t\t\t value: %d\n",getName(config->spKNN),config->spKNN);
	printf("key: %s \t value: %d\n",getName(config->spMinimalGui),config->spMinimalGui);
	printf("key: %s \t value: %d\n",getName(config->spLoggerLevel),config->spLoggerLevel);
	printf("key: %s \t value: %s\n",getName(config->spLoggerFilename),config->spLoggerFilename);
	fflush(NULL);

}
