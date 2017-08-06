#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define LIMIT 1000

#define DOWN 1
#define UP 20
#define delta_x 0.1
#define x_count 10

double fx(double x)
{
	return sin(2*x) + log(x);
}

double start(double x_0)
{
	int i = 0;
	
	double x_down;
	double x_up;
	
	while(i ++ < LIMIT)
	{
		x_down = x_0 - delta_x;
		if(x_down < DOWN)
		{
			x_down = DOWN;
		}
		
		x_up = x_0 + delta_x;
		if(x_up > UP)
		{
			x_up = UP;
		}
		
		if(fx(x_up) > fx(x_0))
		{
			x_0 = x_up;
			continue ;
		}			
		if(fx(x_down) > fx(x_0))
		{
			x_0 = x_down;
			continue ;
		}
		break ;
		
	}
	
//	printf("%f %f\n" , x_0 , fx(x_0));
	return x_0;
	
}

int choose(double *x_array)
{
    int i;
    int max = 0 ;

    for(i = 1 ; i < x_count ; i ++)
    {
        if( fx(x_array[i]) > fx(x_array[max]))
        {
            max = i;
        }
    }

    return max;
}

int main(void)
{
	srand((unsigned int)time(NULL));

    double solution[x_count];
	double x_array[x_count];
    int i;

    for(i = 0 ; i < x_count ; i ++)
    {
        x_array[i] = random() % (UP - DOWN + 1) + DOWN;
        solution[i] = start(x_array[i]);
    }

    int best_x = choose(solution);
    printf("%f %f\n", solution[best_x] , fx(solution[best_x]));
	
	return 0;
	
}
