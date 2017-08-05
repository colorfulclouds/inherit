#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX 20

#define CITY 4
#define PRECISION 0.000001

#define ANT_COUNT 4
typedef struct graph
{
    double d[CITY][CITY];
    double phen[CITY][CITY];
    double cnn;
}graph;

typedef struct ant
{
    int deny[CITY];
    int deny_count;

    int allow[CITY];
    int allow_count;

    int trace[CITY];
    int trace_count;

}ant;

typedef struct ant_colony
{
    graph map;//city map

    ant colony[ANT_COUNT];

}ant_colony;

void init_map(ant_colony *ac)
{
    srand((unsigned int)time(NULL));
    ac->map.cnn = 0;
    int i,j;
/*
    for(i = 0 ; i < CITY - 1 ; i ++)
    {
        ac->map.d[i][i] = 0.0;
        for(j = i + 1 ; j < CITY ; j ++)
        {
            ac->map.d[j][i] = ac->map.d[i][j] = (random() % 50) / 3.0;//get deci
            
            if( fabs(ac->map.d[i][j] - ac->map.d[i][j]) <= PRECISION)
            {
                ac->map.cnn += 2.0;
            }
        }
    }

    ac->map.d[CITY-1][CITY-1] = 0.0;
    */
    //if length is ifi,set 0xffffffff
    //or -1(impossible)

    ac->map.d[0][0]= 0.0;
    ac->map.d[0][1]= 1.0;
    ac->map.d[0][2]= 0.5;
    ac->map.d[0][3]= 1.0;

    ac->map.d[1][0]= 1.0;
    ac->map.d[1][1]= 0.0;
    ac->map.d[1][2]= 1.0;
    ac->map.d[1][3]= 1.0;
    
    ac->map.d[2][0]= 1.5;
    ac->map.d[2][1]= 5.0;
    ac->map.d[2][2]= 0.0;
    ac->map.d[2][3]= 1.0;
    
    ac->map.d[3][0]= 1.0;
    ac->map.d[3][1]= 1.0;
    ac->map.d[3][2]= 1.0;
    ac->map.d[3][3]= 0.0;

    ac->map.cnn = 12;
}

void init_phen(ant_colony *ac)
{
    int i,j;
    double phen_zero = 1.0 / ac->map.cnn ;//ANT_COUNT / ac->map.cnn too
    
    for(i = 0 ; i < CITY - 1 ; i ++)
    {
        for(j = i + 1 ; j < CITY ; j ++)
        {
            ac->map.phen[j][i] = ac->map.phen[i][j] = phen_zero;
        }

        ac->map.phen[i][i] = 0.0;
    }
    
    ac->map.phen[CITY - 1][CITY - 1] = 0.0;
}

void init_ant(ant_colony *ac)
{
    srand((unsigned int)time(NULL));

    int i,j;


    for(i = 0 ; i < ANT_COUNT ; i ++)
    {
        ac->colony[i].deny_count = 0;
        ac->colony[i].trace_count = 0;
        ac->colony[i].allow_count = CITY;
    
        for(j=0;j<CITY;j++)
        {
            ac->colony[i].deny[j] = -1.0 ;
            ac->colony[i].allow[j] = 1.0 ;
        }
    
    }

    for(i = 0 ; i < ANT_COUNT ; i ++)
    {
        ac->colony[i].trace[ac->colony[i].trace_count] = (random() % CITY);
        ac->colony[i].deny[ac->colony[i].trace[ac->colony[i].trace_count]] = 1;
        ac->colony[i].allow[ac->colony[i].trace[ac->colony[i].trace_count]] = -1;

        ac->colony[i].deny_count ++;
        ac->colony[i].trace_count ++;
        ac->colony[i].allow_count --;
    }

}


int choose_which(double *p_choose , double p_all)
{
    srand((unsigned int)time(NULL));
    
    double p[CITY];
    
    int i;
    for(i = 0 ; i < CITY ; i ++)
    {
        if( fabs(p_choose[i] + 1) < PRECISION)
        {
            p[i] = -1.0;
        }
        else
        {
            p[i] = p_choose[i] / p_all;
        }
    }

    double p_rand = (random() % 100 + 1) / 100.0;
    double p_sum = 0.0;

    for(i = 0 ; i < CITY ; i ++)
    {
        if( fabs(p[i] + 1) < PRECISION )
        {
            continue ;
        }
        else
        {
            p_sum += p[i];
            if(p_sum >= p_rand)
            {
                return i;//choose i_city
            }
        }
    }
}

void choose_city(ant *inst , graph *map)
{
    int a = 2;//para
    int b = 3;//para
    int i;
    double p_all = 0.0;
    double p_choose[CITY];

    for(i=0;i<CITY;i++)
    {
        p_choose[i] = -1.0;
    }

    int current_city = inst->trace[inst->trace_count - 1];

    for(i = 0 ; i < CITY ; i ++)
    {
        if(inst->allow[i] == 1)
        {
            if( fabs(map->d[current_city][i] - map->d[current_city][i]) < PRECISION )
            {
                p_choose[i] = pow(map->phen[current_city][i] , a) * pow(1.0 / map->d[current_city][i] , b);
                p_all += p_choose[i];
            }
        }
    }

    int choice = choose_which(p_choose , p_all);
    inst->trace[inst->trace_count ++] = choice;
    inst->deny[choice] = 1;
    inst->deny_count ++;
    inst->allow[choice] = -1;
    inst->allow_count --;

}

double get_add_phen(ant *inst , graph *map , int src , int des)
{
    //easy get
    
    return 0.2 ;
}

void update_phen(ant *inst , graph *map)
{
    double p = 0.5;

    int i,j;
    double temp;

    for(i = 0 ; i < CITY - 1 ; i ++)
    {
        
        for(j = i+1 ; j < CITY ; j ++)
        {
            map->phen[i][j] = (1 - p) * map->phen[i][j] + get_add_phen(inst , map , i , j);
            map->phen[j][i] = (1 - p) * map->phen[j][i] + get_add_phen(inst , map , j , i);
        }
    }
}

//***
double get_fitest(ant *inst , graph *map , int *fitest_ant);
void print_path(ant *inst);
double get_length(ant *inst , graph *map);
//***

void start(ant_colony *ac)
{
    int i;

    int j = 0;
    int k;
    while(j ++ < MAX)
    {
        for(k=0;k<CITY;k++)
        {
            for(i = 0 ; i < ANT_COUNT ; i ++)
            {
                choose_city(ac->colony + i , &(ac->map) );//go to next city
            }
        
            update_phen(ac->colony , &(ac->map));
        }
        //========================
        if(j<MAX)
            init_ant(ac);//add add add
        else
            break ;
    }
    
    int fitest_ant;
    double fitest_length = get_fitest(ac->colony , &(ac->map) , &fitest_ant);
    
    printf("the best ant is %d , length %f\n", fitest_ant , fitest_length);
    puts("path:");
    print_path(ac->colony + fitest_ant);

}

double get_fitest(ant *inst , graph *map , int *fitest_ant)
{
    double path_length[ANT_COUNT];
    int i;


    for(i = 0 ; i < ANT_COUNT ; i ++)
    {
        path_length[i] = get_length(inst + i , map);
    }

    (*fitest_ant) = 0;
    for(i = 1 ; i < ANT_COUNT ; i ++)
    {
        if(path_length[i] > path_length[*fitest_ant])
        {
            (*fitest_ant) = i;
        } 
    }

    return path_length[*fitest_ant];
}

void print_path(ant *inst)
{
    int i;

    for(i = 0 ; i < CITY ; i ++)
    {
        printf("%d ", inst->trace[i]);
    }

    puts("\n");
}

double get_length(ant *inst , graph *map)
{
    double length = 0.0;
    int start = inst->trace[0];
    int i;

    for(i = 1 ; i < CITY ; i ++)
    {
        length += map->d[start][inst->trace[i]];
        start = inst->trace[i];
    }
    
    //cycle (hui lu)
    length += map->d[inst->trace[CITY - 1]][inst->trace[0]];

    return length;
}

void test(ant_colony *ac)
{
    int i;
    int j;

    printf("%f\n", ac->map.cnn);
    for(i=0;i<CITY;i++)
    {
        for(j=0;j<CITY;j++)
            printf("%-5.2f ", ac->map.d[i][j]);

        puts("\n");
    }

}

int main(void)
{
    ant_colony ac;
    init_map(&ac);
    init_phen(&ac);
    init_ant(&ac);
test(&ac);
puts("kk");
    start(&ac);

    return 0;
}
