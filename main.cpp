/*
 * main.cpp
 *
 *  Created on: Jul 23, 2016
 *      Author: Liron
 */

//#include <string.h>
//#include <stdlib.h>
#include <stdio.h>
//#include <iostream>
//#include <SPImageProc.h>



#define MAX_LENGTH_PATH 1024


int main(){
	printf("mati");
	//	const char* filename = "C:/final/myconfig.config";
//	SP_CONFIG_MSG* msg=NULL;
//	SP_CONFIG_MSG pathmsg;
//	fflush(NULL);
//	puts("1");
//	fflush(NULL);
//	SPConfig config= spConfigCreate(filename,msg);
//	puts("2");
//	fflush(NULL);
//	char* path=(char*) malloc(sizeof(char)*MAX_LENGTH_PATH);
//	char* sufFeat= (char*) malloc(sizeof(char)*10);
//	strcpy( sufFeat, ".feat" );
//	FILE *tempFile;
//	SPPoint* resPoints;
//	int* numOfFeats=(int*) malloc(sizeof(int));
//	sp::ImageProc imgP (config);   //Creates new instance if ImageProc
//	//Enum Values comparsion - need to get done
//	if(spConfigIsExtractionMode(config,msg) && *msg==10){
//		for(int i=0;i<spConfigGetNumOfImages(config,msg)&& *msg==10;i++){
//			puts("inLoop");
//			genarateFeatPath(path,config,i);
//			tempFile=fopen(path, "w+");
//			pathmsg = spConfigGetImagePath(path,config,i);
//		// check pathmsg
//			resPoints=imgP.getImageFeatures(path,i,numOfFeats);
//			fwrite((const void*)numOfFeats[0],sizeof(int),1,tempFile);
//			fwrite("\n",sizeof(char),1,tempFile);
//			for(int j=0;j<numOfFeats[0];i++){
//				for(int k=0;k<127;i++){
//					double temp = spPointGetAxisCoor(resPoints[j],k);
//					fwrite((const void*)&temp,sizeof(double),1,tempFile);
//					fwrite("\t",sizeof(char),1,tempFile);
//				}
//				double temp = spPointGetAxisCoor(resPoints[j],127);
//				fwrite((const void*)&temp,sizeof(double),1,tempFile);
//				fwrite("\n",sizeof(char),1,tempFile);
//			}
//			fclose(tempFile);
//		}
//
//	}
	return 0;
}


