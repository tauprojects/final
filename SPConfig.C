#include <SPConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define getName(var) #var

struct sp_config_t {
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension;
	char* spPCAFilename;
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	SP_SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
};

//Declarations of helper functions
void remCharEnd(char* s);
void remCharBeg(char* s);
bool hasSpace(char* string);
void printConstrainMsg(const char* filename, int line);
void configMapper(char* key, char* val, SPConfig config);
void strToLower(char* s);


void alocFunc(SPConfig config) {
	config->spImagesDirectory = (char*) malloc(sizeof(char) * MAXLEN);
	config->spImagesPrefix = (char*) malloc(sizeof(char) * MAXLEN);
	config->spImagesSuffix = (char*) malloc(sizeof(char) * MAXLEN);
	config->spPCAFilename = (char*) malloc(sizeof(char) * MAXLEN);
	config->spLoggerFilename = (char*) malloc(sizeof(char) * MAXLEN);
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	SPConfig config;
	alocFunc(config);
	char* s;
	char buff[256];
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		puts("bad file path");
	}
	while ((s = fgets(buff, sizeof buff, fp)) != NULL) {
		//Skip blank lines and comments
		if (buff[0] == '\n' || buff[0] == '#')
			continue;
		//Skip # lines and comments
		if (strchr(s, '#') != NULL)
			continue;
		//parse key and val
		char key[MAXLEN], value[MAXLEN];
		s = strtok(buff, "=");
		if (s == NULL)
			continue;
		else
			strncpy(key, s, MAXLEN);
//	    printf("key: %s ",key);
		s = strtok(NULL, "=");
		if (s == NULL)
			continue;
		else
			strncpy(value, s, MAXLEN);
		value[strlen(value) - 1] = '\0';
		remCharBeg(key);
		remCharEnd(key);
		remCharBeg(value);
		remCharEnd(value);
		configMapper(key, value, config);
	}
	fclose(fp);
	return config;

}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
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
	if (config->spMinimalGUI)
		return true;
	else
		return false;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfFeatures;
}
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spPCADimension;
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
	return SP_CONFIG_SUCCESS;
}



void printConstrainMsg(const char* filename, int line) {
	printf(
			" File: %s \n Line: %d \n Message: Invalid value - constraint not met\n",
			filename, line);
}

bool getSpExtractionMode(SPConfig config) {
	puts("Im in extract");
	return true;
}
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (config == NULL || pcaPath == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	strcat(pcaPath, config->spImagesDirectory);
	strcat(pcaPath, config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

void genarateFeatPath(char* featPath, const SPConfig config, int index) {
	if (featPath == NULL || config == NULL)
		return;
	if (index >= config->spNumOfImages)
		return;
	strcpy(featPath, "");
	char* i = (char*) malloc(sizeof(char) * MAXLEN);
	if (i == NULL) {
		fflush(NULL);
		return;
	}
	sprintf(i, "%d", index);
	strcat(featPath, config->spImagesDirectory);
	strcat(featPath, config->spImagesPrefix);
	strcat(featPath, i);
	strcat(featPath, ".feat");
	return;
}
void configMapper(char* key, char* val, SPConfig config) {

	if (strcmp(key, "spImagesDirectory") == 0) {

		if (hasSpace(val)) {
			printConstrainMsg(__FILE__, __LINE__);
		} else
			strcpy(config->spImagesDirectory, val);
	}

	if (strcmp(key, "spImagesPrefix") == 0) {
		if (hasSpace(val)) {
			printConstrainMsg(__FILE__, __LINE__);
		} else
			strcpy(config->spImagesPrefix, val);
	}

	if (strcmp(key, "spImagesSuffix") == 0) {
		if (strcmp(val, ".bmp") == 0 || strcmp(val, ".jpg") == 0
				|| strcmp(val, ".png") == 0 || strcmp(val, ".gif") == 0) {
			strcpy(config->spImagesSuffix, val);

		} else
			printConstrainMsg(__FILE__, __LINE__);
	}
	if (strcmp(key, "spNumOfImages") == 0) {
		//Check if val is a valid string->int
		int intVal = atoi(val);
		if (intVal <= 0)
			printConstrainMsg(__FILE__, __LINE__);
		else
			config->spNumOfImages = intVal;
	}

	if (strcmp(key, "spPCADimension") == 0) {
		int intVal = atoi(val);
		if (intVal < 10 || intVal > 128) {
			printConstrainMsg(__FILE__, __LINE__);
		} else
			config->spPCADimension = intVal;
	}
//
	if (strcmp(key, "spPCAFilename") == 0) {
		if (hasSpace(val)) {
			printConstrainMsg(__FILE__, __LINE__);
		} else
			strcpy(config->spPCAFilename, val);

	}

	if (strcmp(key, "spNumOfFeatures") == 0) {
		int intVal = atoi(val);
		if (intVal <= 0)
			printConstrainMsg(__FILE__, __LINE__);
		else
			config->spNumOfFeatures = intVal;
	}

	if (strcmp(key, "spExtractionMode") == 0) {
		//lowercase
		strToLower(val);
		if (strcmp(val, "true") == 0)
			config->spExtractionMode = true;
		else if (strcmp(val, "false") == 0)
			config->spExtractionMode = false;
		else
			printConstrainMsg(__FILE__, __LINE__);
	}
	if (strcmp(key, "spNumOfSimilarImages") == 0) {
		int intVal = atoi(val);
		if (intVal <= 0)
			printConstrainMsg(__FILE__, __LINE__);
		else
			config->spNumOfSimilarImages = intVal;
	}
	if (strcmp(key, "spKDTreeSplitMethod") == 0) {
		if (strcmp(val, "RANDOM") == 0)
			config->spKDTreeSplitMethod = RANDOM;
		else if (strcmp(val, "INCREMENTAL") == 0)
			config->spKDTreeSplitMethod = INCREMENTAL;
		else if (strcmp(val, "MAX_SPREAD") == 0)
			config->spKDTreeSplitMethod = MAX_SPREAD;
		else
			printConstrainMsg(__FILE__, __LINE__);
	}

	if (strcmp(key, "spKNN") == 0) {
		int intVal = atoi(val);
		if (intVal <= 0)
			printConstrainMsg(__FILE__, __LINE__);
		else
			config->spKNN = intVal;
	}

	if (strcmp(key, "spMinimalGUI") == 0) {
		//lowercase
		strToLower(val);
		if (strcmp(val, "true") == 0)
			config->spMinimalGUI = true;
		else if (strcmp(val, "false") == 0)
			config->spMinimalGUI = false;
		else
			printConstrainMsg(__FILE__, __LINE__);
	}

	if (strcmp(key, "spLoggerLevel") == 0) {
		int intVal = atoi(val);
		if (intVal <= 0 || intVal > 4)
			printConstrainMsg(__FILE__, __LINE__);
		else
			config->spKNN = intVal;
	}

	if (strcmp(key, "spLoggerFilename") == 0) {
		if (hasSpace(val)) {
			printConstrainMsg(__FILE__, __LINE__);
		} else
			strcpy(config->spLoggerFilename, val);

	}

}
void remCharEnd(char* s) {
	char *token = (char*) malloc(sizeof(char) * MAXLEN);
	int len = strlen(s);
	int i = 0;
	while ((i < len && s[i] != ' ')) {
		token[i] = s[i];
		i++;
	}
	token[i] = '\0';
	strncpy(s, token, MAXLEN);
}
void remCharBeg(char* s) {
	char *token;
	int len = strlen(s);
	int i = 0;
	while (i < len && s[i] == ' ')
		i++;
	token = &s[i];
	strncpy(s, token, MAXLEN);
}

bool hasSpace(char* string) {
	int len = strlen(string);
	for (int i = 0; i < len; i++) {
		if (string[i] == ' ')
			return true;
	}
	return false;
}
void strToLower(char* s){
	for(int i = 0; s[i]!='\0'; i++){
	  s[i] = tolower(s[i]);
	}
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
	printf("key: %s \t value: %d\n",getName(config->spMinimalGUI),config->spMinimalGUI);
	printf("key: %s \t value: %d\n",getName(config->spLoggerLevel),config->spLoggerLevel);
	printf("key: %s \t value: %s\n",getName(config->spLoggerFilename),config->spLoggerFilename);
	fflush(NULL);

}

