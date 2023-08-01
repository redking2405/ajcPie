#include <stdio.h>
#include <gd.h>
#include <gdfontt.h>
#include <gdfontl.h>
#include <gdfontg.h>
#include <string.h>

void MakePie(int p[], int colors[], gdImagePtr im, int center)
{
    int i=0;
    int startP;
    int endP;
    int length = sizeof(p[0]);
    for(i=0; i<length; i++)
    {
        if(i==0)
            startP=0;
        endP = startP+(3.6*p[i]);
        gdImageFilledArc(im, center, center, center, center, startP, endP, colors[i],0);
        startP=endP;
    }
}

void MakeLegend(gdImagePtr im, gdFontPtr f,char* data, int colors[], int center, int size)
{
    char* tempStr = strtok(data, " ");
    int i=0;
    while(tempStr!=NULL)
    {
        if(i==0)
            gdImageString(im, f, center+size , center,tempStr,colors[i]);
        if(i==1)
            gdImageString(im, f, center , center+size,tempStr,colors[i]);
        if(i==2)
            gdImageString(im, f, center-(size+8*strlen(tempStr)) , center,tempStr,colors[i]);
        if(i==3)
            gdImageString(im, f, center , center-(size+strlen(tempStr)),tempStr,colors[i]);

        i++;
        tempStr=strtok(NULL, " ");
    }
}

int main()
{
    gdImagePtr im;
    FILE* pngout;
    int sizeX=256;
    int sizeY=256;
    int center = sizeX/2;
    //colors
    int black;
    int white;
    int red;
    int blue;
    int green;

    im = gdImageCreate(sizeX, sizeY);
    white = gdImageColorAllocate(im, 255,255,255);
    black = gdImageColorAllocate(im, 0, 0 ,0);
    red = gdImageColorAllocate(im,255,0,0);
    blue = gdImageColorAllocate(im, 0,0,255);
    green = gdImageColorAllocate(im,0,255,0);
    gdFontPtr font;
    int percents[4] = {10, 25, 35, 30};
    int colors[4] = {black, red, blue,green};
    char country[] = "USA Espagne France Italie";




    font = gdFontGetGiant();

    MakePie(percents, colors, im, center);
    MakeLegend(im, font, country, colors, center, 72);
    pngout = fopen("test.png", "wb");
    gdImagePng(im, pngout);
    fclose(pngout);
    gdImageDestroy(im);
    return 0;
}
