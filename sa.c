#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define delta_t 0.98
#define k 0.1
#define DOWN 2
#define UP 5

#define COUNT 1000


double fx(double x)
{
    return sin(2*x) + log(x);
}

void start()
{
    srand((unsigned int)time(NULL));

    int i;
    double p;
    double t = UP;
	
    double new_x;
    new_x = random() % (UP - DOWN + 1) + DOWN;
	double best = fx(new_x);
    double current;
    double dE;

    while( t  > DOWN )
    {
        for(i=0;i<COUNT;i++)
        {
            new_x = random() % (UP - DOWN + 1) + DOWN;
            current = fx(new_x);
            dE = current - best;

            if(dE < 0)
            {
                best = current;
            }
            else
            {
                p = (random() % 100 + 1) / 100.0;
                if( exp(dE / (t*k)) > p)
                {
                    best = current;
                }
            }
        }
        t = t * delta_t;
    }
	
	printf("min %f\n", best);
}

int main(void)
{
    start();

    return 0;
}
