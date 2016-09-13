#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define ERROR_MSG "---ERROR---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n"
#define WARNING_MSG "---WARNING---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n"
#define INFO_MSG "---INFO---\n- message: %s\n"
#define DEBUG_MSG "---DEBUG---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n"

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

/*
 * Checks validity of arguments for functions, and that logger is defined
 * @return SP_LOGGER_UNDIFINED if logger is not defined
 * 		   SP_LOGGER_INVAlID_ARGUMENT if arguments are invalid
 * 		   SP_LOGGER_SUCCESS if logger is defined and arguments are valid
 */
SP_LOGGER_MSG inputIsValid(const char* msg, const char* file,
		const char* function, const int line) {
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if (line < 0 || msg == NULL || function == NULL || file == NULL)
		return SP_LOGGER_INVAlID_ARGUMENT;
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) { //Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger); //free allocation
	logger = NULL;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line) {
	//check validity of arguments
	SP_LOGGER_MSG valid = inputIsValid(msg, file, function, line);
	if (valid != SP_LOGGER_SUCCESS)
		return valid;

	//print Error message to user, if proper level (in this case - always)
	if (fprintf(logger->outputChannel, ERROR_MSG, file, function, line, msg) == 0)// write failure
		return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS;

}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line) {
	//check validity of arguments
	SP_LOGGER_MSG valid = inputIsValid(msg, file, function, line);
	if (valid != SP_LOGGER_SUCCESS)
		return valid;

	//print Warning message to user, if proper level
	if (logger->level >= SP_LOGGER_WARNING_ERROR_LEVEL)
		if (fprintf(logger->outputChannel, WARNING_MSG, file, function, line, msg) == 0)// write failure
			return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS;

}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {
	//check validity of arguments
	SP_LOGGER_MSG valid = inputIsValid(msg, " ", " ", 0);
	if (valid != SP_LOGGER_SUCCESS)
		return valid;

	//print Info message to user, if proper level
	if (logger->level >= SP_LOGGER_INFO_WARNING_ERROR_LEVEL)
		if (fprintf(logger->outputChannel, INFO_MSG, msg) == 0)// write failure
			return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line) {
	//check validity of arguments
	SP_LOGGER_MSG valid = inputIsValid(msg, file, function, line);
	if (valid != SP_LOGGER_SUCCESS)
		return valid;

	//print Debug message to user, if proper level
	if (logger->level >= SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
		if (fprintf(logger->outputChannel, DEBUG_MSG, file, function, line, msg)== 0)// write failure
			return SP_LOGGER_WRITE_FAIL;

	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {
	//check validity of argument
	SP_LOGGER_MSG valid = inputIsValid(msg, " ", " ", 0);
	if (valid != SP_LOGGER_SUCCESS)
		return valid;

	//print the message
	if (fprintf(logger->outputChannel,"%s\n" , msg) == 0) // write failure
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;

}

