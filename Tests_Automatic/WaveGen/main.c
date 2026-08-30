#include "pbPlots.h"
#include "supportLib.h"
#include "stdint.h"
#include "string.h"

#include "WaveGen.h"

static WaveGen UUT;

void Plot_Create(int32_t *y,int32_t y_len);
void setup()
{
    UUT = WaveGen_Create(&SINE);
}
void teardown()
{
    WaveGen_Destroy(UUT);
}



void test_sinus50hz()
{
    setup();
    WaveGen_SetFrequency(UUT,50.0f,20000.0f);

    int32_t y[400];
    int32_t y_len = 400;
    for(uint32_t sample = 0; sample < 400;sample++)
        y[sample] = WaveGen_Get(UUT);

    Plot_Create(y,y_len);

    teardown();
}

void test_frequencySwitch()
{
    setup();
    WaveGen_SetFrequency(UUT,50.0f,20000.0f);

    int32_t y_len = 800;
    int32_t y[800];

    for(int32_t sample = 0; sample < y_len; sample++)
    {   
        y[sample] = WaveGen_Get(UUT);
        if(sample == 400)
            WaveGen_SetFrequency(UUT,100,20000);
    }

    Plot_Create(y,y_len);

    teardown();
}

void test_reset()
{
    setup();
    WaveGen_SetFrequency(UUT,50,20000);

    int32_t y[600];
    int32_t y_len = sizeof(y) / sizeof(y[0]);
    for(int32_t sample = 0; sample < y_len; sample++)
    {
        y[sample] = WaveGen_Get(UUT);
        if(sample == y_len /4)
            WaveGen_Reset(UUT);
    }
    
    Plot_Create(y,y_len);

    teardown();
}

int main()
{   
    test_sinus50hz();
    test_frequencySwitch();
    test_reset();

    return 0;
}

void Plot_Create(int32_t *y,int32_t y_len)
{

    FILE *gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot, "plot '-'\n");
    for (int32_t i = 0; i < y_len; i++)
        fprintf(gnuplot, "%d %d\n", i,y[i]); //g
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);

    system("pause");
}