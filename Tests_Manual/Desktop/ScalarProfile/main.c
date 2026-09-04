#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "ScalarProfile.h"


void Plot_Create(float *y,int32_t y_len)
{

    FILE *gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot, "plot '-'\n");
    for (int32_t i = 0; i < y_len; i++)
        fprintf(gnuplot, "%d %f\n", i,y[i]); //g
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);

    system("pause");
}

int main()
{
    ScalarProfile_Init(230.0f, 50.0f,50.0f, 5.0f);
    float y[70];

    for(int sample = 0; sample < 60; sample++)
    {
        y[sample] = ScalarProfile_GetVoltage((float)sample);
    }
    Plot_Create(y,60);

    return 0;
}