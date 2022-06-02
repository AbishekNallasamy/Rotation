#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int findFile(char *directory, char *fileName, char *finalDirectory)
{
    DIR *openedDir;
    struct dirent *directoryStruct;
    char *tempDir;
    long int offset;
    tempDir=(char*)calloc(300,sizeof(char));
    strcat(directory,"/");
    printf("%s \n",directory);
    openedDir=opendir(directory);
    if(openedDir==NULL) {
	printf("NULL\n"); 
	return -1;
    }
    while(1) {
	directoryStruct=readdir(openedDir);
	printf("file Name : %s\tOffset : %ld\toffset-- : %ld\n",directoryStruct->d_name,offset=telldir(openedDir),offset--);
	if(directoryStruct==NULL) {
	    closedir(openedDir);  
	    free(tempDir); 
	    return -1;
	}
	if(!strcmp(directoryStruct->d_name, "..") || !strcmp(directoryStruct->d_name, ".")) {
	    printf("continue\n");
	    continue;
	}
	else if(!strcmp(directoryStruct->d_name,fileName)) {
	    //printf("file Name : %s\n",directoryStruct->d_name);
	    sprintf(finalDirectory,"%s%s",directory,directoryStruct->d_name);
	    return 0;
	} else if(directoryStruct->d_type==DT_DIR) {
	    printf("enter\n");
	    sprintf(tempDir,"%s%s",directory,directoryStruct->d_name);
	    //strcat(directory,directoryStruct->d_name);
	    if( findFile(tempDir,fileName,finalDirectory)==-1 ) {	
	    	closedir(openedDir);
		free(tempDir);
	    	return -1;
	    }
	}
    }
}


void main(int argc, char *argv[])
{
    char homeDirectory[500]="/home/waiss/C_prog/Test/Pixcels/Image Rotation",finalDirectory[300]={0};
    findFile(homeDirectory,argv[1],finalDirectory);
    printf("%s\n",finalDirectory);
    return ;
}

