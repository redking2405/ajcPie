#include <stdio.h>
#include <gd.h>
#include <gdfontt.h>
#include <gdfontl.h>
#include <gdfontg.h>
#include <string.h>

int numArgs;
typedef struct Datalist Datalist;

struct Datalist{
    int percent;
    int index;
    char* data;
    Datalist* suivant;
};

///
/// \brief Init
/// \param percent
/// \param data
/// \return list of member
/// set up a list with the percentage and the label for the pie chart
Datalist* Init(int percent, char* data)
{
    Datalist* premier = malloc(sizeof(*premier));
    premier->percent = percent;
    premier->index = 0;
    premier->data = malloc(strlen(data));
    strcpy(premier->data, data);
    premier->suivant = NULL;
    return premier;
}

///
/// \brief Add
/// \param list
/// \param percent
/// \param data
/// add a new member to the list in parameter
void Add(Datalist* list, int percent, char* data)
{
    Datalist* new = malloc(sizeof(*new));
    Datalist* current=list;
    int i=list->index;
    new->data = malloc(sizeof(*data));
    strcpy(new->data, data);
    new->percent= percent;
    while(current->suivant !=NULL)
    {
        i=current->index;
        current = current->suivant;
    }
    new->index = current->index+1;
    current->suivant=new;
}

///
/// \brief Clear
/// \param list
///Clear the list after use
void Clear(Datalist* list){


    while(list!=NULL)
    {
        Datalist* aSupprimer = list;
        free(aSupprimer->data);
        list = list->suivant;
        free(aSupprimer);

    }
}
///
/// \brief CompileArgument
/// \param args
/// \return
/// Get the arguments passed in the terminal if there isn't enough data prepare a list with a placeholder pie chart
/// with 25% charts and placeholder labels
Datalist* CompileArgument(char** args, char* filename)
{
    int i;
    int size = (numArgs-2)/2;
    Datalist* list;
    if((numArgs-2)%2!=0 ||  numArgs<6)
    {
        printf("NOT ENOUGH ARGUMENT\n");
        //return 1;
        list = Init(25, "Placeholder");
        Add(list, 25, "placeholder2");
        Add(list, 25, "placeholder3");
        Add(list, 25, "placeholder4");
        filename = "test.png";
        return list;
    }
    int sum=0;
    int tempInt;
    for(i=2; i<size+1; i++)
    {
        tempInt = atoi(args[i]);
        if(i=2)
            list=Init(tempInt, args[i+size]);
        else
            Add(list, tempInt, args[i+size]);
        sum+=tempInt;
    }

    if(sum!=100)
    {
        printf("THERE IS DATA MISSING");
    }
    filename=args[numArgs];
    return list;
}

///
/// \brief LastIndex
/// \param list
/// \return
///Get the last item in the list is used to determine how many section the pie chat has
int LastIndex(Datalist* list)
{
    Datalist* current = list;
    while(current->suivant!=NULL){
        current= current->suivant;
    }

    return current->index;
}

///
/// \brief GetMember
/// \param list
/// \param i
/// \return
///get a specific member of the list to draw the pie chart
Datalist* GetMember(Datalist* list, int i)
{
    Datalist* current = list;
    while(current->suivant!=NULL)
    {
        if(current->index==i)
            return current;
        current = current->suivant;
    }
    return current;
}
///
/// \brief MakePieChart
/// \param im
/// \param f
/// \param data
/// \param colors
/// \param center
/// \param radius
///Draw the pie chart using the data given by the arguments in the program
void MakePieChart(gdImagePtr im, gdFontPtr f, Datalist* data, int colors[], int center, int radius)
{
    int len = LastIndex(data);
    Datalist* currentData;
    int i=0;
    int startP=0;
    int endP;
    for(i=0; i<len+1; i++)
    {
        currentData=GetMember(data,i);
        endP=startP+3.6*currentData->percent;
        gdImageFilledArc(im, center, center, radius*2, radius*2,startP, endP,colors[i],0);
        if(endP<=90)
            gdImageString(im, f, center+radius+strlen(currentData->data) , center,currentData->data,colors[i]);
        else if(endP>90 && endP<=180)
            gdImageString(im, f, center , center+radius+strlen(currentData->data),currentData->data,colors[i]);
        else if(endP>180 && endP<=270)
            gdImageString(im, f, center-(radius+9*strlen(currentData->data)) , center,currentData->data,colors[i]);
        else
            gdImageString(im, f, center+(radius/2) , center-radius,currentData->data,colors[i]);
        startP=endP;
    }
}

int main(int argc, char* argv[])
{
    numArgs = argc;
    gdImagePtr im;
    FILE* pngout;
    int sizeX=256;
    int sizeY=256;
    //colors
    int black;
    int white;
    int red;
    int blue;
    int green;
    char* file = malloc(100);
    im = gdImageCreate(sizeX, sizeY);
    white = gdImageColorAllocate(im, 255,255,255);
    black = gdImageColorAllocate(im, 0, 0 ,0);
    red = gdImageColorAllocate(im,255,0,0);
    blue = gdImageColorAllocate(im, 0,0,255);
    green = gdImageColorAllocate(im,0,255,0);
    gdFontPtr font;

    int colors[4] = {black, red, blue,green};

    Datalist* datas = CompileArgument(argv, &file);
    font = gdFontGetGiant();
    MakePieChart(im, font, datas, colors, sizeX/2, sizeX/4);
    /*gdImageFilledArc(im, sizeX/2, sizeX/2, sizeX/2, sizeX/2, 0,90,black,0);
    gdImageFilledArc(im, sizeX/2, sizeX/2, sizeX/2, sizeX/2, 90,180,red,0);
    gdImageFilledArc(im, sizeX/2, sizeX/2, sizeX/2, sizeX/2, 180,270,blue,0);
    gdImageFilledArc(im, sizeX/2, sizeX/2, sizeX/2, sizeX/2, 270,360,green,0);
    gdImageString(im, font, 200, 128,"Nigeria", black);
    gdImageString(im, font, 128, 200,"Espagne", red);
    gdImageString(im, font, 2, 128,"France", blue);
    gdImageString(im, font, 128, 45,"Italie", green);*/
    pngout = fopen(file, "wb");
    gdImagePng(im, pngout);
    fclose(pngout);
    free(file);
    gdImageDestroy(im);
    Clear(datas);
    return 0;
}
