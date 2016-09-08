#include "SPConfig.h"
#include <assert.h>
#define LINE_MAX_LENGTH 1025
#define SUFFIX_MAX_LENGTH 5

#define DEFAULT_PCA_DIMENSION 20
#define DEFAULT_PCA_FILENAME "pca.yml"
#define DEFAULT_NUM_OF_FEATURES 100
#define DEFAULT_EXTRACTION_MODE true
#define DEFAULT_MINIMAL_GUI false
#define DEFAULT_NUM_OF_SIMILAR_IMAGES 1
#define DEFAULT_KNN 1
#define DEFAULT_KD_SPLIT_METHOD MAX_SPREAD
#define DEFAULT_LOGGER_LEVEL 3
#define DEFAULT_LOGGER_FILENAME "stdout"

#define ERROR_FORMAT "File: %s\nLine: %d\nMessage: "

#define SPIMAGESDIRECTORY "spImagesDirectory"
#define SPIMAGESPREFIX "spImagesPrefix"
#define SPIMAGESSUFFIX "spImagesSuffix"
#define SPNUMOFIMAGES "spNumOfImages"
#define SPPCADIMENSION "spPCADimension"
#define SPPCAFILENAME "spPCAFilename"
#define SPNUMOFFEATURES "spNumOfFeatures"
#define SPEXTRACTIONMODE "spExtractionMode"
#define SPNUMOFSIMILARIMAGES "spNumOfSimilarImages"
#define SPKDTREESPLITMETHOD "spKDTreeSplitMethod"
#define SPKNN "spKNN"
#define SPMINIMALGUI "spMinimalGui"
#define SPLOGGERLEVEL "spLoggerLevel"
#define SPLOGGERFILENAME "spLoggerFilename"

#define isSuffix(str) ((strcmp((str),".jpg")==0) || (strcmp((str),".png")==0) || (strcmp((str),".bmp")==0) ||(strcmp((str),".gif")==0))
#define isLoggerLevel(x) ((x)==1 || (x)==2 || (x)==3 ||(x)==4)
#define inPCADimensionRange(x) (10<=(x) && (x)<=128)
#define isBool(str) ((strcmp((str),"true")==0) || (strcmp((str),"false")==0))
#define isKDSplitMethod(str) ((strcmp((str),"RANDOM")==0) || (strcmp((str),"MAX_SPREAD")==0) || (strcmp((str),"INCREMENTAL")==0))

#define handleBoolCase(sysvar) 		if(!isBool(right)){\
			printErrorMessage(filename, lineNumber, INVALID_VALUE,NULL);\
			*msg = SP_CONFIG_INVALID_STRING;\
			return false;\
		}else{\
			if(strcmp(right,"true")==0){\
				sysvar = true;\
			}else{\
				sysvar = false;\
			}\
		}
#define handlePositiveIntegerCase(sysvar) 		if(!onlyNumeric(right) || atoi(right) < 1){\
			printErrorMessage(filename, lineNumber, INVALID_VALUE, NULL);\
			*msg = SP_CONFIG_INVALID_INTEGER;\
			return false;\
		}else{\
			sysvar = atoi(right);\
		}

struct sp_config_t {
	char spImagesDirectory[LINE_MAX_LENGTH]; //the string contains no spaces
	char spImagesPrefix[LINE_MAX_LENGTH]; //the string contains no spaces
	char spImagesSuffix[SUFFIX_MAX_LENGTH]; //the string contains no spaces
	int spNumOfImages; //positive integer
	int spPCADimension; //integer in range [10,128]
	char spPCAFilename[LINE_MAX_LENGTH]; //the string contains no spaces
	int spNumOfFeatures; //positive integer
	bool spExtractionMode; //true/false
	int spNumOfSimilarImages; //positive integer
	SP_SPLIT_METHOD spKDTreeSplitMethod; //in the set {RANDOM,MAX_SPREAD,INCREMENTAL}
	int spKNN; //positive integer
	bool spMinimalGui; //true/false
	int spLoggerLevel; //in the set {1,2,3,4}
	char spLoggerFilename[LINE_MAX_LENGTH]; //the string contains no spaces
};

/**
 * simply checks if file == NULL
 * if so, then updates msg accordingly
 */

bool checkFile(FILE* file, SP_CONFIG_MSG* msg) {
	if (file == NULL) {
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		return false;
	}
	return true;
}
/**
 * simply checks if allocated == NULL
 * if so, then updates msg accordingly
 */
bool checkMalloc(void* allocated, SP_CONFIG_MSG* msg) {
	if (allocated == NULL) {
		*msg = SP_CONFIG_ALLOC_FAIL;
		return false;
	}
	return true;
}

/**
 * simply checks if filename == NULL
 * if so, then updates msg accordingly
 */
bool checkFilename(const char* filename, SP_CONFIG_MSG* msg) {
	if (filename == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	return true;
}

/**
 * assigned default values to config res
 */
void assignDefaultValues(SPConfig res) {
	res->spPCADimension = DEFAULT_PCA_DIMENSION;
	strcpy(res->spPCAFilename, DEFAULT_PCA_FILENAME);
	res->spNumOfFeatures = DEFAULT_NUM_OF_FEATURES;
	res->spExtractionMode = DEFAULT_EXTRACTION_MODE;
	res->spMinimalGui = DEFAULT_MINIMAL_GUI;
	res->spNumOfSimilarImages = DEFAULT_NUM_OF_SIMILAR_IMAGES;
	res->spKNN = DEFAULT_KNN;
	res->spKDTreeSplitMethod = DEFAULT_KD_SPLIT_METHOD;
	res->spLoggerLevel = DEFAULT_LOGGER_LEVEL;
	strcpy(res->spLoggerFilename, DEFAULT_LOGGER_FILENAME);

	//assign NULL to all other fields to invalid values for personal use
	res->spImagesDirectory[0] = '\0';
	res->spImagesPrefix[0] = '\0';
	res->spImagesSuffix[0] = '\0';
	res->spNumOfImages = 0;
}

bool isCommentOrEmptyLine(char* line) {
	while (isspace(*line))
		line++;
	if (*line == '#' || *line == '\0')
		return true;
	return false;
}

/**
 * lineNumber is either the actual line number, or the total number of lines
 */
void printErrorMessage(const char* filename, int lineNumber,
		CFG_ERROR_TYPE type, const char* parameter) {
	printf(ERROR_FORMAT, filename, lineNumber);
	switch (type) {
	case INVALID_LINE:
		printf("Invalid configuration line\n");
		break;
	case INVALID_VALUE:
		printf("Invalid value - constraint not met\n");
		break;
	case PARAMETER_NOT_SET:
		printf("Parameter %s is not set\n", parameter);
		break;
	}
}

/*
 * checks that there are only spaces on left or on right of string,
 * and that it only contains letters
 */
bool noSpacesInsideAndOnlyAlpha(char* line) {
	while (isspace(*line))
		line++;
	while (isalpha(*line))
		line++;
	while (isspace(*line))
		line++;

	//the above should now reach the end of the string
	if (*line != '\0')
		return false;
	return true;
}
/*
 * checks that there are only spaces on left or on right of string,
 * and that it only contains numeric digits
 */
bool noSpacesInsideAndOnlyNumeric(char* line) {
	while (isspace(*line))
		line++;
	while (isdigit(*line))
		line++;
	while (isspace(*line))
		line++;

	//the above should now reach the end of the string
	if (*line != '\0')
		return false;
	return true;
}

bool noSpacesInside(char* line) {
	while (isspace(*line))
		line++;
	while (!isspace(*line)&&*line!='\0')
		line++;
	while (isspace(*line))
		line++;
	if (*line != '\0')
		return false;
	return true;
}

//I assume empty strings are not a valid string
bool parseVariableName(char* variable, char* line) {
	if (!noSpacesInsideAndOnlyAlpha(line))
		return false;
	sscanf(line, "%s", variable);
	if (strlen(variable) == 0)
		return false;
	return true;
}

bool onlyAlpha(char* str) {
	while (isalpha(*str))
		str++;
	if (*str != '\0')
		return false;
	return true;
}

bool onlyNumeric(char* str) {
	while (isdigit(*str))
		str++;
	if (*str != '\0')
		return false;
	return true;
}
//I assume empty strings are not a valid string
bool parseRightSide(char* right, char* temp) {
	if (!noSpacesInside(temp))
		return false;

	sscanf(temp, "%s", right);
	if (strlen(right) == 0)
		return false;
	return true;
}

bool assignValue(SPConfig res, char* variable, char* right, SP_CONFIG_MSG* msg,
		const char* filename, int lineNumber) {
	if (strcmp(variable, SPIMAGESDIRECTORY) == 0) {
		strcpy(res->spImagesDirectory, right);
	} else if (strcmp(variable, SPIMAGESPREFIX) == 0) {
		strcpy(res->spImagesPrefix, right);
	} else if (strcmp(variable, SPIMAGESSUFFIX) == 0) {
		if (!isSuffix(right)) {
			printErrorMessage(filename, lineNumber, INVALID_VALUE, NULL);
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		} else {
			strcpy(res->spImagesSuffix, right);
		}
	} else if (strcmp(variable, SPNUMOFIMAGES) == 0) {
		handlePositiveIntegerCase(res->spNumOfImages)
	} else if (strcmp(variable, SPPCADIMENSION) == 0) {
		if (!onlyNumeric(right) || !inPCADimensionRange(atoi(right))) {
			printErrorMessage(filename, lineNumber, INVALID_VALUE, NULL);
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		} else {
			res->spPCADimension = atoi(right);
		}
	} else if (strcmp(variable, SPPCAFILENAME) == 0) {
		strcpy(res->spPCAFilename, right);
	} else if (strcmp(variable, SPNUMOFFEATURES) == 0) {
		handlePositiveIntegerCase(res->spNumOfFeatures)
	} else if (strcmp(variable, SPEXTRACTIONMODE) == 0) {
		handleBoolCase(res->spExtractionMode)
	} else if (strcmp(variable, SPNUMOFSIMILARIMAGES) == 0) {
		handlePositiveIntegerCase(res->spNumOfSimilarImages)
	} else if (strcmp(variable, SPKDTREESPLITMETHOD) == 0) {
		if (!isKDSplitMethod(right)) {
			printErrorMessage(filename, lineNumber, INVALID_VALUE, NULL);
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		} else {
			if (strcmp(right, "RANDOM") == 0) {
				res->spKDTreeSplitMethod = RANDOM;
			} else if (strcmp(right, "MAX_SPREAD") == 0) {
				res->spKDTreeSplitMethod = MAX_SPREAD;
			} else {
				res->spKDTreeSplitMethod = INCREMENTAL;
			}
		}
	} else if (strcmp(variable, SPKNN) == 0) {
		handlePositiveIntegerCase(res->spKNN)
	} else if (strcmp(variable, SPMINIMALGUI) == 0) {
		handleBoolCase(res->spMinimalGui)
	} else if (strcmp(variable, SPLOGGERLEVEL) == 0) {
		if (!onlyNumeric(right) || !isLoggerLevel(atoi(right))) {
			printErrorMessage(filename, lineNumber, INVALID_VALUE, NULL);
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		} else {
			res->spLoggerLevel = atoi(right);
		}
	} else if (strcmp(variable, SPLOGGERFILENAME) == 0) {
		strcpy(res->spLoggerFilename, right);
	} else {

		printErrorMessage(filename, lineNumber, INVALID_LINE, NULL);
		*msg = SP_CONFIG_INVALID_STRING;
		return false;
	}
	return true;
}
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	assert(msg != NULL); //or if msg==NULL return NULL;
	*msg = SP_CONFIG_SUCCESS; //stay this way unless ERROR

//check validity of arguments
	if (!checkFilename(filename, msg))
		return NULL;

//try to open file and check if opened
	FILE* file = fopen(filename, "r");
	;
	if (!checkFile(file, msg))
		return NULL;

//try to allocate memory for res - which is the return value in case of success
	SPConfig res = malloc(sizeof(*res));
	if (!checkFile((void*) res, msg))
		return NULL;

	char line[LINE_MAX_LENGTH];
	;
	char variable[LINE_MAX_LENGTH]; //where to store variable name
	char right[LINE_MAX_LENGTH]; //where to store right side of '='

	char* temp;
	int lineNumber = 0;
	bool invalidLine = false;
	assignDefaultValues(res);

	while (fgets(line, LINE_MAX_LENGTH, file) != NULL) {
		if (isCommentOrEmptyLine(line)) {
			lineNumber++;
			continue;
		}
		if ((temp = strchr(line, '=')) == NULL) {
			invalidLine = true;
		} else {
			*temp = '\0'; //split line to left and right of '=' sign
			if (!parseVariableName(variable, line))
				invalidLine = true;
			if (!parseRightSide(right, temp + 1)){
				invalidLine = true;
			}
		}
		if (invalidLine) {
			printErrorMessage(filename, lineNumber, INVALID_LINE, NULL);
			*msg = SP_CONFIG_INVALID_STRING;
			spConfigDestroy(res);
			return NULL;
		}

		if (!assignValue(res, variable, right, msg, filename, lineNumber)) {
			spConfigDestroy(res);
			return NULL;
		}

		lineNumber++;
	}
	if (strlen(res->spImagesDirectory) == 0) {
		printErrorMessage(filename, lineNumber, PARAMETER_NOT_SET,
				"spImagesDirectory");
		*msg = SP_CONFIG_MISSING_DIR;
		spConfigDestroy(res);
		return NULL;
	}
	if (strlen(res->spImagesPrefix) == 0) {
		printErrorMessage(filename, lineNumber, PARAMETER_NOT_SET,
				"spImagesPrefix");
		*msg = SP_CONFIG_MISSING_PREFIX;
		spConfigDestroy(res);
		return NULL;
	}
	if (strlen(res->spImagesSuffix) == 0) {
		printErrorMessage(filename, lineNumber, PARAMETER_NOT_SET,
				"spImagesSuffix");
		*msg = SP_CONFIG_MISSING_SUFFIX;
		spConfigDestroy(res);
		return NULL;
	}
	if (res->spNumOfImages == 0) {
		printErrorMessage(filename, lineNumber, PARAMETER_NOT_SET,
				"spNumOfImages");
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		spConfigDestroy(res);
		return NULL;
	}

	//if we get here, everything is good so far! now create the logger
	SP_LOGGER_MSG loggerMsg;
	if (strcmp(res->spLoggerFilename, "stdout") == 0) {
		loggerMsg = spLoggerCreate(NULL, res->spLoggerLevel - 1); //because the count starts at 0 in the enum
	} else {
		loggerMsg = spLoggerCreate(res->spLoggerFilename,
				res->spLoggerLevel - 1);
	}
	if (loggerMsg == SP_LOGGER_CANNOT_OPEN_FILE) {
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		spConfigDestroy(res);
		return NULL;
	} else if (loggerMsg == SP_LOGGER_OUT_OF_MEMORY) {
		*msg = SP_CONFIG_ALLOC_FAIL;
		spConfigDestroy(res);
		return NULL;
	} else if (loggerMsg == SP_LOGGER_DEFINED) {
		return res; //***i don't know what to do in this scenario at the moment***
	}

	//if we reach here, the msg stayed SP_CONFIG_SUCCESS
	return res;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	return config->spExtractionMode;
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	return config->spMinimalGui;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
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
	if (index < 0 || index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
//	puts("Inside the getImagePath");
//	fflush(NULL);
	sprintf(imagePath, "%s%s%d%s", config->spImagesDirectory,
			config->spImagesPrefix, index, config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (pcaPath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	sprintf(pcaPath, "%s%s", config->spImagesDirectory, config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config) {
	if (config == NULL)
		return;
	spLoggerDestroy();
	free(config);
}
/**MY SHIT**/

SP_CONFIG_MSG spConfigGetKDSplitMethod(SP_SPLIT_METHOD* method,
		const SPConfig config) {
	if (method == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	*method = config->spKDTreeSplitMethod;
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetFeatsPath(char* featsPath, const SPConfig config,
		int index){
	if (featsPath == NULL || config == NULL || index >= config->spNumOfImages)
		return SP_CONFIG_INVALID_ARGUMENT;
	sprintf(featsPath, "%s%s%d%s", config->spImagesDirectory,
			config->spImagesPrefix, index, ".feats");
	return SP_CONFIG_SUCCESS;

}

int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spKNN;
}

int spConfigGetSimilarImages(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	*msg = SP_CONFIG_SUCCESS;
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	return config->spNumOfSimilarImages;

}
