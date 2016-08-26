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

int genaratePath(char* dirPath, char* prefix, char* suffix, int index,char* path){
	strcpy(path, "" );
	char* i=(char*) malloc(sizeof(char)*MAX_LENGTH_PATH);
	if(i==NULL){
		fflush(NULL);
		return -1;
	}
	sprintf(i,"%d",index);
	strcat(path,dirPath);
	strcat(path,prefix);
	strcat(path,i);
	strcat(path,suffix);
	return 0;
}

int genaratePathWithoutIndex(char* dirPath, char* prefix, char* suffix,char* path){
	strcpy( path, "" );
	strcat(path,dirPath);
	strcat(path,prefix);
	strcat(path,suffix);
	return 0;
}

int main(){
	const char* filename = "./NAME";
	SP_CONFIG_MSG* msg=NULL;
	SPConfig config= spConfigCreate(filename,msg);
	char* path=(char*) malloc(sizeof(char)*MAX_LENGTH_PATH);
	char* sufFeat= (char*) malloc(sizeof(char)*10);
	strcpy( sufFeat, ".feat" );
	FILE *tempFile;
	SPPoint* resPoints;
	int* numOfFeats=(int*) malloc(sizeof(int));
	sp::ImageProc imgP (config);   //Creates new instance if ImageProc
	if(getSpExtractionMode(config)){
		for(int i=0;i<getSpNumOfImages(config);i++){
			genaratePath(getSpImagesDirectory(config), getSpImagesPrefix(config), sufFeat, i ,path);
			tempFile=fopen(path, "w+");
			genaratePathWithoutIndex(getSpImagesDirectory(config), getSpImagesPrefix(config), getSpImagesSuffix(config) ,path);
			resPoints=imgP.getImageFeatures(path,i,numOfFeats);
			fwrite((const void*)numOfFeats[0],sizeof(int),1,tempFile);
			fwrite("\n",sizeof(char),1,tempFile);
			for(int j=0;j<numOfFeats[0];i++){
				for(int k=0;k<127;i++){
					double temp = spPointGetAxisCoor(resPoints[j],k);
					fwrite((const void*)&temp,sizeof(double),1,tempFile);
					fwrite("\t",sizeof(char),1,tempFile);
				}
				double temp = spPointGetAxisCoor(resPoints[j],127);
				fwrite((const void*)&temp,sizeof(double),1,tempFile);
				fwrite("\n",sizeof(char),1,tempFile);
			}
			fclose(tempFile);
		}

	}
	return 0;
}


