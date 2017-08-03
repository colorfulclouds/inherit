#include <stdlib.h>
#include <stdio.h>

#define GENE_SCALE 20


typedef struct population
{
    int dna[GENE_SCALE];//indi info
    
    double sum_fitness;

    double fitness[GENE_SCALE];
    double p[GENE_SCALE];
}pop_t;

double get_fitness(int dna)
{
    //easy get

    return (double)dna;
}

void gen(pop_t *inst)
{
    int i;

    for(i = 0 ; i < GENE_SCALE ; i ++)
    {
        inst->fitness[i] = get_fitness(inst->dna[i]);
    }
}

void get_sum_fitness(pop_t *inst)
{
    inst->sum_fitness = 0.0;

    int i;
    for(i = 0 ; i < GENE_SCALE ; i ++)
    {
        inst->sum_fitness += inst->fitness[i];
    }
}

void get_p(pop_t *inst)
{
    get_sum_fitness(inst);

    int i;

    for(i = 0 ; i < GENE_SCALE ; i ++)
    {
        inst->p[i] = inst->fitness[i] / inst->sum_fitness;
    }
}


void choose(pop_t *inst)
{
    double p;
    double p_sum;

    int limit = GENE_SCALE;
    int i;
    srand((unsigned int)time(NULL));//random seed

    while(limit > 0)
    {
        p = (random() % 100 + 1) / 100.0;
        p_sum = 0.0;

        for(i = 0 ; i < GENE_SCALE ; i ++)
        {
            p_sum += inst->p[i];

            if(p_sum >= p)
            {
                //choose it
                printf("choose one:%d\n", inst->dna[i]);
                limit --;
                break ;
            }
        }
    }
}

void init_dna(pop_t *inst)
{
    srand((unsigned int)time(NULL));
    int i;

    for(i = 0 ; i < GENE_SCALE ; i ++)
    {
        inst->dna[i] = random() % 20 + 1;//[1,20]
    }
}

int main(void)
{
    pop_t inst;
    init_dna(&inst);//eff

    gen(&inst);
    
   
    get_p(&inst);
    
    int i;
    printf("random number:\n");
    for(i=0;i<GENE_SCALE;i++)
        printf("%d\n", inst.dna[i]);
    puts("==");
    
    choose(&inst);

    return 0;
};
