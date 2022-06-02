#include"image_Editor.h"


void writeAsRawFile(unsigned char* image, int size, unsigned char *fileName)
{
    FILE *filePtr;    
    filePtr=fopen(fileName,"wb");
    if(filePtr==NULL) {
	printf("Sorrry file Error\n");
	return ;
    }
    fwrite(image,sizeof(unsigned char),size,filePtr);
    fclose(filePtr);
}


void rotate(unsigned char* old, unsigned char* new,int *width,int *height)
{
    int xPosition,yPosition,oldLocation,newLocation;
    for(oldLocation=0 ; oldLocation<(*width)*(*height) ; oldLocation++) {
	yPosition=oldLocation/(*width);
	xPosition=oldLocation%(*width);
	newLocation=((*height)*xPosition)+yPosition;
	*(new+newLocation)=*(old+oldLocation);
    }
    return;
}

void strRev(unsigned char* string,long size)
{
    int i=0,j;
    unsigned char temp;
    for(i=0,j=size-1 ; i<j ;i++,j-- ) {
	temp=*(string+i);
	*(string+i)=*(string+j);
	*(string+j)=temp;
    }
    return;
}

void horizontalFilp(unsigned char* charPtr,int *width, int *height)
{
    int i=0;
    for(i=0 ; i<(*height) ; i++) {
	strRev(charPtr+(i*(*width)),*width);
    }
    return;
}
    
void verticalFilp(unsigned char* new,unsigned char *old,int width,int height)
{
    int i=0,j=0;
    for(i=0;i<(height);i++) {
	for(j=0 ; j< (width); j++) {
	    *(new+((width*(height-i-1))+j))=*(old+(width*i)+j);
	}
    }
    return;
}

void cropImage(unsigned char *destination,unsigned char *source, int start, int stop, int *destinationLoc)
{
    int i=0;
    //printf("Enter %d\n",*destinationLoc);
    for(i=0 ; i<=stop-start ; i++,(*destinationLoc)++) {
	*(destination+(*destinationLoc))=*(source+start+i);
	//`printf("%d\n",*(destination+(*destinationLoc)));
    }
    return ;
}


/*
 *Function Name: flushstdin
 *Description  : To clear the stdin buffer
 *Arguments    : -
 *Return Type  : void
*/
void flushstdin()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF); 
}//flushstdin

/*
 *Function Name: checkChoice
 *Description  : To check whether the given choice is true or not
 *Arguments    : choice
	int choice represents the choice obtained from the user
 *Return Type  : int
	return PASS if the choice is correct and FAIL if the choice is ot correct
*/
int checkChoice(int choice,int number)
{
    if(choice>=1 && choice<=number)
	return PASS;
    else 
	return FAIL;
}//checkChoice



void croP(unsigned char **crop,unsigned char *charPtr, int* width, int* height)
{
    int cropStartXPosition,cropStartYPosition,cropStopXPosition,cropStopYPosition,cropRow,cropCol,cropLocation=0,i=0,j=0;
    printf("\t\tEnter the point where to start the crop in the Format (x coordinate <SPACE> y coordinate)\t");
    scanf("%d %d",&cropStartXPosition,&cropStartYPosition);
    printf("\t\tEnter the point where to stop the crop in the Format (x coordinate <SPACE> y coordinate)\t");
    scanf("%d %d",&cropStopXPosition,&cropStopYPosition);
    if(cropStopXPosition-cropStartXPosition<0) 	
	printf("Invalid"); 
    cropLocation=(cropStartYPosition*(*width))+cropStartXPosition;
    cropRow=cropStopXPosition-cropStartXPosition;
    cropCol=cropStopYPosition-cropStartYPosition;
    *crop=(unsigned char*)calloc(cropRow*cropCol,sizeof(unsigned char));
    //memset(crop,255,fileSize);
    printf("%d\n",cropLocation);

    for(i=0 ; i<cropCol ; i++) {
	cropImage(*crop,charPtr,cropLocation+(i*(*width)),cropLocation+(i*(*width))+cropRow-1,&j );
    }
    *width=cropRow;
    *height=cropCol;
    //cropImage(crop,charPtr,cropRow,cropCol,cropLocation);
    return ;
}


/*  strCpy
    parameter 1    source    char*
    parameter 2    destination    char*
    parameter 3    start    int
    parameter 4    stop    int
    To copy the source string to the destination string
*/

void strCpy(unsigned char *destination,unsigned char *source, int start, int stop)
{
    int i=0;
    for(i=0 ; i<=stop-start ; i++) destination[i]=source[start+i];
}//strCpy

void swape(int *width, int *height)
{
    int temp;
    temp=*width;
    *width=*height;
    *height=temp;
    return ;
}

void rotation(unsigned char *charPtr,int *width,int *height)
{
    int RotateChoice;
    unsigned char* newr;
    newr=(char*)calloc((*width)*(*height),sizeof(unsigned char));
    if(newr==NULL) {
        printf("NO memory found\n");
	return;
    }

	printf("\n\tChoose a Rotation Degree\n\t\t1 -----90 Degree\n\t\t2 -----180 Degree\n\t\t3 -----270 Degree\t");
    	scanf("%d",&RotateChoice);

    	/*  To check and get only the given shapes */
    	while(!checkChoice(RotateChoice,3)) {
	    printf("\nxxxxxxxxxxxxxxxxxxxxxxWrong Choice. Pease Re enter your choicexxxxxxxxxxxxxxxxxxxxxx\t");
	    flushstdin();
	    scanf("%d",&RotateChoice);
    	}
	switch(RotateChoice) {
	case 2: 	// 180 Degree
    	    strRev(charPtr,(*width)*(*height));
    	    writeAsRawFile(charPtr,(*width)*(*height),"180_Rotate.raw");
	    break;
 	case 1:		//90 Degree
    	    rotate(charPtr,newr,width,height);
    	    swape(width,height);
	    printf("\n\tCurrent Width : %d\tCurrent Height : %d\n",*width,*height);
    	    horizontalFilp(newr,width,height);
    	    strCpy(charPtr,newr,0,(*width)*(*height));
            writeAsRawFile(charPtr,(*width)*(*height),"90_Rotate.raw");
	    break;
	case 3:		//270 Degree
    	    strRev(charPtr,(*width)*(*height));
    	    rotate(charPtr,newr,width,height);
	    swape(width,height);
	    printf("\n\tCurrent Width : %d\tCurrent Height : %d\n",*width,*height);
	    horizontalFilp(newr,width,height);
	    strCpy(charPtr,newr,0,(*width)*(*height));
	    writeAsRawFile(newr,(*width)*(*height),"270_Rotate.raw");
	    break;
	}
    free(newr);
    return;

}
 

void flip(unsigned char *charPtr,int *width,int *height)
{
    int FlipChoice;
    unsigned char* new;
    new=(char*)calloc((*width)*(*height),sizeof(unsigned char));
    if(new==NULL) {
        printf("NO memory found\n");
	return;
    }


	printf("\n\tChoose an option\n\t\t1 -----Horizontal\n\t\t2 -----Vertical\t");
    	scanf("%d",&FlipChoice);

    	/*  To check and get only the given shapes */
    	while(!checkChoice(FlipChoice,2)) {
	    printf("\nxxxxxxxxxxxxxxxxxxxxxxWrong Choice. Pease Re enter your choicexxxxxxxxxxxxxxxxxxxxxx\t");
	    flushstdin();
	    scanf("%d",&FlipChoice);
    	}
	switch(FlipChoice) {

	case 1:		//Horizontal Flip
	    horizontalFilp(charPtr,width,height);
	    writeAsRawFile(charPtr,(*width)*(*height),"Horiflip.raw");
	    break;
	case 2:
	    verticalFilp(new,charPtr,*width,*height);
	    strCpy(charPtr,new,0,(*width)*(*height));
	    writeAsRawFile(charPtr,(*width)*(*height),"Vertiflip.raw");
	    break;
	}
	free(new);
	return;
}


void imageEditorOptions(unsigned char* charPtr, int *width ,int *height)
{
    unsigned char *new,*crop;
    int EditChoice,RotateChoice,FlipChoice,scale;
       
    printf("\n\tChoose an option\n\t\t1 -----Rotation\n\t\t2 -----Flip\n\t\t3 -----Crop\n\t\t4 -----Resize\t");
    scanf("%d",&EditChoice);

    /*  To check and get only the given shapes */
    while(!checkChoice(EditChoice,4)) {
	printf("\nxxxxxxxxxxxxxxxxxxxxxxWrong Choice. Pease Re enter your choicexxxxxxxxxxxxxxxxxxxxxx\t");
	flushstdin();
	scanf("%d",&EditChoice);
    }	
   
    switch(EditChoice) {
 
    case ROTATION:	//Rotation
	rotation(charPtr,width,height);
    	break;
	   
    case FLIP:		//Flip
	flip(charPtr,width,height);	
	break;

    case CROP:		//Crop
	new=(unsigned char*)calloc((*width)*(*height),sizeof(unsigned char));
	if(new==NULL) {
	    printf("NO memory found\n");
	    return;
    	}
	croP(&new,charPtr,width,height);
	printf("\n\tCurrent Width : %d\tCurrent Height : %d\n",*width,*height);
	charPtr=(unsigned char*)realloc(charPtr,(*width)*(*height)*sizeof(unsigned char));
	strCpy(charPtr,new,0,(*width)*(*height));
	writeAsRawFile(charPtr,(*width)*(*height),"AbiCrop.raw");
    case RESIZE:
	printf("Please enter the scale\n");
	scanf("%d",&scale);
	
    }
    return ;
}
