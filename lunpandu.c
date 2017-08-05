#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TOTAL_FITNESS 10

int get_prob()
{
    int array_fitness[20];
    random(array_fitness);

    float slice = random() * TOTAL_FITNESS; 
    float total = 0.0f;
    int choice;

    for(int i = 0 ;i<20 ; i++)
    {
        total += array_fitness[i];

        if(total >= slice)
        {
            choice = i;
            break;
        }
    }

    return choice;

}
