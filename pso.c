#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define BIRD_COUNT 10

#define GENE 500
//============
#define w 0.9
#define v_0 0.1
//============

#define DOWN 2
#define UP 5

#define c_1 1.4961
#define c_2 1.4961

typedef struct bird
{
    double x;
    double best_x;
    double best_fitness;
    double v;
}bird_t;

typedef struct particle
{
    bird_t bird[BIRD_COUNT];
    bird_t best_bird;
    
}particle_t;

double get_fitness(double x)
{
    return sin(2*x) + log(x);
}

void get_best_bird(particle_t *inst)
{
    int i;
    
    inst->best_bird = inst->bird[0];

    for(i = 1 ; i < BIRD_COUNT ; i ++)
    {
        if(inst->best_bird.best_fitness < inst->bird[i].best_fitness)
        {
            inst->best_bird = inst->bird[i];
        }
    }
}

void init_particle(particle_t *inst)
{
    srand((unsigned int)time(NULL));
    int i;

    for(i = 0 ; i < BIRD_COUNT ; i ++)
    {
        inst->bird[i].x = (random() % (int)(UP - DOWN + 1)) + DOWN ;
        inst->bird[i].best_x = inst->bird[i].x ;
        inst->bird[i].best_fitness = get_fitness(inst->bird[i].x);
        inst->bird[i].v = v_0;
    }

    get_best_bird(inst);
}

void start(particle_t *inst)
{
    srand((unsigned int)time(NULL));

    double r_1 = (random() % 100 + 1) / 100.0;
    double r_2 = (random() % 100 + 1) / 100.0;

    double x_temp;
    int i;
    int j;

    for(i = 0 ; i < GENE ; i ++)
    {
        for(j = 0 ; j < BIRD_COUNT ; j ++)
        {
            inst->bird[j].v = w * inst->bird[j].v + c_1 * r_1 * (inst->bird[j].best_x - inst->bird[j].x) + \
            c_2 * r_2 * (inst->best_bird.x - inst->bird[j].x);

            x_temp = inst->bird[j].x + inst->bird[j].v;
            inst->bird[j].x = (x_temp > UP ? UP : (x_temp < DOWN ? DOWN: x_temp) );

            if(get_fitness(inst->bird[j].x) > inst->bird[j].best_fitness)
            {
                inst->bird[j].best_x = inst->bird[j].x;
                inst->bird[j].best_fitness = get_fitness(inst->bird[j].x);
            }
        }

        get_best_bird(inst);
        printf("test %f   %f\n", inst->best_bird.x , inst->best_bird.best_fitness);
    }
}

int main(void)
{
    particle_t part;
    init_particle(&part);

    start(&part);

    printf("number %f fitness %f\n", part.best_bird.x , part.best_bird.best_fitness);

    return 0;
}
