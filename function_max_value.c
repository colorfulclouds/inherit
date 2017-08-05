#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GATHER 30
#define LENGTH 17

#define MAX_GENE 50

void init_gather(int *array)
{
    int i;

    for(i = 0 ; i<GATHER ; i ++)
    {
        array[i] = random() % 9 + 1;
        printf("%d ", array[i]);
    }

    puts("");
}

float get_fx(int x)
{
    float y;
    
    //y = log(x) + 10*sin(5*x) + 7*cos(4*x);
//y=sin(2*x)*cos(tan(x));
    y=sin(x) + cos(x);
    return y;

}

void trim(int *array , int *mother , int *father)
{
    int i;
    float fx[GATHER];

    for(i = 0 ; i < GATHER ; i ++)
    {
        if(array[i] != -1)
            fx[i] = get_fx(array[i]);
        else
            fx[i] = -12345.0;
    }

    int max_one;
    int max_two;

    max_one = max_two = 0;

    for(i = 0 ; i<GATHER ; i ++)
    {
        if(fx[max_one] < fx[i])
        {
            max_two = max_one;
            max_one = i;
        }
        else if(fx[max_two] < fx[i])
        {
            max_two = i;
        }
    }

    (*mother) = array[max_two];
    (*father) = array[max_one];

    array[max_one] = array[max_two] = -1;

}

void sex(int mother , int father , int *son , int *daughter)
{
    int array[5] = {1 , 3 , 7 , 15 , 31};

    (*son) = (mother & 12 ) + ( father & 3 ); 
    (*daughter) = (father & 12 ) + ( mother & 3 ); 
}

void swap_array(int *src , int *des)
{
    int i;

    for(i = 0 ; i<GATHER ; i ++)
    {
        des[i] = src[i];
    }
}

int main(void)
{
    int i;

    int count = 0;

    int gather[GATHER];
    int gather_valid_count;

    int gather_bak[GATHER];
    int gather_bak_pointer;

    init_gather(gather_bak);

    int mother;
    int father;
    int son;
    int daughter;

    while(count < MAX_GENE)
    {
        //replace gather
        swap_array(gather_bak , gather);
        
        //memcpy(gather , gather_bak , GATHER);
        gather_bak_pointer = 0;
        gather_valid_count = GATHER;

        while(gather_valid_count > 0 )
        {
            trim(gather , &mother , &father);
            gather_valid_count -= 2;

            sex(mother , father , &son , &daughter);

            gather_bak[gather_bak_pointer ++] = son;
            gather_bak[gather_bak_pointer ++] = daughter;

        }
        puts("=====");
        printf("%d %d %f %f\n", mother , father , get_fx(mother) , get_fx(father));
//        for(i=0;i<GATHER;i++)
//            printf("%d ", gather_bak[i]);
        puts("\n======");
        count ++;

        printf("count = %d\n", count);
    }


    return 0;
}
