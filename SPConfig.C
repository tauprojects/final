#include <SPConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct sp_config_t{
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

/*
 * trim: get rid of trailing and leading whitespace...
 *       ...including the annoying "\n" from fgets()
 */
void strToLower(char* s);
void remCharEnd(char* s);
void remCharBeg(char* s);
bool isSpace(char* string);
void printConstrainMsg(const char* filename,int line);


void helperFunc(char* key,char* val,SPConfig config){
   printf("key is %s, val is %s !\n",key,val );
	if  (strcmp(key,"spImagesDirectory")==0) {
		  if(isSpace(val)){
				printConstrainMsg(__FILE__,__LINE__);
	    	 }
		  else
		  config->spImagesDirectory=val;
		}

    if  (strcmp(key,"spImagesPrefix")==0) {
		  if(isSpace(val)){
				printConstrainMsg(__FILE__,__LINE__);
	    	 }
		  else
		  config->spImagesPrefix=val;
			}

    if  (strcmp(key,"spImagesSuffix")==0) {
		  if(strcmp(val,".bmp")==0||strcmp(val,".jpg")==0||strcmp(val,".png")==0||strcmp(val,".gif")==0){
			  config->spImagesSuffix=val;
	    	 }
		  else
		     printConstrainMsg(__FILE__,__LINE__);
			}
	if  (strcmp(key,"spNumOfImages")==0) {
		   //Check if val is a valid string->int
		   int intVal = atoi(val);
			if(intVal<=0)
					printConstrainMsg(__FILE__,__LINE__);
			else
			 config->spNumOfImages=intVal;
			}

		if(strcmp(key,"spPCADimension")==0) {
			int intVal = atoi(val);
			if(intVal<10 || intVal>128){
				printConstrainMsg(__FILE__,__LINE__);
         }
			else
         config->spPCADimension=intVal;
		}
//
	  if(strcmp(key,"spPCAFilename")==0) {
		  if(isSpace(val)){
				printConstrainMsg(__FILE__,__LINE__);
			 }
		  else
		  config->spPCAFilename=val;
		}

	  if (strcmp(key,"spNumOfFeatures")==0) {
			 int intVal = atoi(val);
				if(intVal<=0)
					printConstrainMsg(__FILE__,__LINE__);
				else
			 config->spNumOfFeatures=intVal;
		}

	  if  (strcmp(key,"spExtractionMode")==0) {
		//lowercase
		  if(strcmp(val,"true")==0)
			 config->spExtractionMode=true;
		 else if(strcmp(val,"false")==0)
					 config->spExtractionMode=false;
		 else
			 printConstrainMsg(__FILE__,__LINE__);
	  }
      if  (strcmp(key,"spNumOfSimilarImages")==0) {
			 int intVal = atoi(val);
				if(intVal<=0)
					printConstrainMsg(__FILE__,__LINE__);
				else
			 config->spNumOfSimilarImages=intVal;
			}
      if  (strcmp(key,"spKDTreeSplitMethod")==0) {
        if(strcmp(val,"RANDOM")==0)
        	config->spKDTreeSplitMethod=RANDOM;
        else if(strcmp(val,"INCREMENTAL")==0)
        	config->spKDTreeSplitMethod=INCREMENTAL;
        else if(strcmp(val,"MAX_SPREAD")==0)
        	config->spKDTreeSplitMethod=MAX_SPREAD;
        else
        	printConstrainMsg(__FILE__,__LINE__);
      }


      if  (strcmp(key,"spKNN")==0) {
	         int intVal = atoi(val);
				if(intVal<=0)
					printConstrainMsg(__FILE__,__LINE__);
				else
	         config->spKNN=intVal;
			}

	  if  (strcmp(key,"spMinimalGUI")==0) {
			//lowercase
			  if(strcmp(val,"true")==0)
				 config->spMinimalGUI=true;
			else if(strcmp(val,"false")==0)
						 config->spMinimalGUI=false;
			else
				 printConstrainMsg(__FILE__,__LINE__);
			}

	  if  (strcmp(key,"spLoggerLevel")==0) {
			 int intVal = atoi(val);
				if(intVal<=0 || intVal>4)
					printConstrainMsg(__FILE__,__LINE__);
				else
			 config->spKNN=intVal;
			}

	  if  (strcmp(key,"spLoggerFilename")==0) {
		  if(isSpace(val)){
				printConstrainMsg(__FILE__,__LINE__);
			 }
		  else
		  config->spLoggerFilename=val;
		}

}
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	SPConfig config;
	char* s;
	char buff[256];
	FILE *fp = fopen (filename, "r");
	  if (fp == NULL)
	  {
	    puts("bad file path");
	  }
	  while ((s = fgets(buff, sizeof buff, fp)) != NULL)
	  {
	    //Skip blank lines and comments
	    if (buff[0] == '\n' || buff[0] == '#')
	      continue;
	    //Skip # lines and comments
	    if (strchr(s,'#')!=NULL)
	    	continue;
	    //parse key and val
	    char key[MAXLEN], value[MAXLEN];
	    s = strtok (buff, "=");
	    if (s==NULL)
	      continue;
	    else
	      strncpy (key, s, MAXLEN);
//	    printf("key: %s ",key);
	    s = strtok (NULL, "=");
	    if (s==NULL)
	      continue;
	    else
	      strncpy (value, s, MAXLEN);
	    value[strlen(value)-1]='\0';
	    remCharBeg(key);
	    remCharEnd(key);
	    remCharBeg(value);
		 remCharEnd(value);
	    helperFunc(key,value,config);
	  }
	  fclose(fp);
	  return config;

}
//void strToLower(char* s){
//	for(int i = 0; s[i]!='\0'; i++){
//	  s[i] = tolower(s[i]);
//	}
//}
void remCharEnd(char* s){
	char *token = (char*)malloc(sizeof(char)*MAXLEN);
	int len = strlen(s);
	int i=0;
	while((i<len && s[i]!=' ') ){
		token[i]=s[i];
		i++;
	}
	token[i]='\0';
	strncpy(s,token,MAXLEN);
}
void remCharBeg(char* s){
	char *token;
	int len = strlen(s);
	int i=0;
	while(i<len && s[i]==' ')
		i++;
	token=&s[i];
	strncpy(s,token,MAXLEN);
}

bool isSpace(char* string){
	int len = strlen(string);
	for(int i=0;i<len;i++){
		if (string[i]==' ')
				return true;
	}
	return false;
}
void printConstrainMsg(const char* filename,int line){
	printf(" File: %s \n Line: %d \n Message: Invalid value - constraint not met\n",filename,line);
}

