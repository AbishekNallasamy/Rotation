#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

void copyRow(unsigned char *string, int destinationStart, int sourceStart, int size)
{
    int i=0;
    //printf("%d %d %d\n",destinationStart,sourceStart,size);
    for(i=0 ; i<size ; i++,destinationStart++,sourceStart++) {
	*(string+destinationStart)=*(string+sourceStart);
    }
    return ;
}

int average(unsigned char *string, int location, int scale, int width)
{
    int i=0,j=0,averageReturn=0;
    printf("%d \t",location);
    for(i=0 ; i< scale ; i++) {
	for(j=0 ; j<scale ; j++) {
	    averageReturn+=*(string+location+i+(j*width));
	}
    }
   // printf("%X\n",averageReturn/(scale*scale));
    return averageReturn/(scale*scale);
}

void main(int agrc, char *argv[])
{
    unsigned char *charPtr, *new,*new1;
    int i=0,j=0,fileSize,original=0,width=1920,height=1080,scale=2,k=0;
    FILE *filePtr;     

    filePtr = fopen(argv[1],"rb");
    if(filePtr==NULL) {
        printf("NO data found or File doesn't exist\n");
        return;
    } else {
        fseek(filePtr,0,SEEK_END);
        fileSize=ftell(filePtr);
        charPtr=(unsigned char*)calloc(fileSize,sizeof(unsigned char));
        //new=(unsigned char*)calloc(fileSize,sizeof(unsigned char));
        if(charPtr==NULL) {
            printf("No enough memory found\n");
	    return;
	}
	fseek(filePtr,0,SEEK_SET);  
	fread(charPtr,1,fileSize,filePtr);
	fclose(filePtr);
    }

#if 1
    new=(unsigned char *)calloc(width*height*scale*scale,sizeof(unsigned char));
 
    for(original=(width*height)-1,i=(width*height*scale*scale)-1 ; original>=0 ; original--) {
	for( j=i ; j>i-scale; j-- ) {
	    *(new+j)=*(charPtr+original);
	}
	if(j%(width*scale)==(width*scale)-1) {
	    for(k=1 ; k<scale ; k++) {
	    	copyRow(new,j-(width*scale*k)+1,j+1,width*scale);
	    }
	    //copyRow(new,j-(width*scale)+1,j+1,width*scale);
	    i=j-(width*scale*(scale-1));
	}
	else i=j;
    }

    writeAsRawFile(new,width*height*scale*scale,"BadResize.raw");

#endif
#if 1
    width=width*scale;
    height=height*scale;
    scale=3;

//width=500  height=500

    new1=(unsigned char *)calloc(width*height/(scale*scale),sizeof(unsigned char));
    for(i=0,original=0 ; i<width*height ; i+=scale,original++) {
     	if(i==(width-1)*height)   break;
      	if( i/width!=0 && i%width==0)
	    i=i+width*(scale-1);
	*(new1+original)=average(charPtr,i,scale,width);
    }
    width=width/scale;
    height=height/scale;

  /* for(i=0 ; i<4 ; i++) {
	for(j=0 ; j<4 ; j++) {
	    printf("%2X ",*(new1+(i*4)+j));
	}
	printf("\n");
    }*/

    writeAsRawFile(new1,width*height,"BadResizehalf.raw");
#endif
    printf("Curremt width : %d    height : %d\n",width,height);
}


