#include "PPM.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *  LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 *  usage: GLubyte myImg = LoadPPM("myImg.ppm", &width, &height, &max);
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max){
    GLubyte* image;
    FILE *f;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    f = fopen(file,"r");
    fscanf(f,"%[^\n] ",b);
    
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(f, "%c",&c);
    while(c == '#')
    {
        fscanf(f, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(f, "%c",&c);
    }
    ungetc(c,f);
    fscanf(f, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    nm = n*m;
    
    image = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(f,"%d %d %d",&red, &green, &blue );
        image[3*nm-3*i-3]=red*s;
        image[3*nm-3*i-2]=green*s;
        image[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *max = k;
    
    return image;
}