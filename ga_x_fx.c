//linux
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define EPOCH 50
#define NUMBER 100	//种群数量
#define	EPSILON	0.00001
#define DOWN 0
#define UP 2

#define P_C	0.99	//交叉概率
#define P_M	0.02	//变异概率

#define BITS ( (int)ceil( log((UP - DOWN)/EPSILON+1) / log(2) ) )


double random_()
{
	return (double)random()/RAND_MAX;
}

double decode(char *b)
{
	int temp = 0;
	int i;
	int j;
	int weight[33] = {0 , 2147483648 , 1073741824 , 536870912 , 268435456 , 134217728 , 67108864 , 33554432 , 16777216, 
						  8388608 , 4194304 , 2097152 , 1048576 , 524288 , 262144 , 131072 , 65536, 
						  32768 , 16384 , 8192 , 4096 , 2048 , 1024 , 512 , 256,
						  128 , 64 , 32, 16 , 8 , 4 , 2 , 1 };

	for(i=BITS-1 , j=32 ; i>=0 ; i -- , j --)
		temp += (b[i] == '1' ? weight[j] : 0);

	return DOWN + temp * EPSILON;
}

double fitness(double x)
{
	//return (x-0.5)*(x-0.5) + fabs(x+3);

	//return sin(2*x) + cos(x) + log(x);

    //return sin(2*x)+exp(-x)+log(x);

    //return sin(x)+cos(x*x)+log(x);

    return sin(x)+cos(x*x*x*x*x);
}

void sum_p(double *p)
{
	int i;

	for(i=1 ; i<NUMBER ; i ++)
		p[i] = p[i] + p[i-1];
}

int gamble(double *p)
{
	int i;
	for(i=0 ; i<NUMBER ; i ++)
		if(random_() <= p[i])
			return i;

	return -1;
}

void cross_over(char *father , char *mother)
{
	//单点交叉
	int cross_pos;
	char *temp;

	if(P_C >= random_())
	{
		temp = (char *)malloc(BITS + 1);

		cross_pos = random() % BITS;	//随机产生交叉点

		strcpy(temp , father + cross_pos);
		strcpy(father + cross_pos , mother + cross_pos);
		strcpy(mother + cross_pos , temp);

		free(temp);
	}
}

void mutation(char *children)
{
	int j;

	for(j=0 ; j < BITS ; j ++)
		if(P_M >= random_())
		{
			if(children[j] == '1')
				children[j] = '0';
			else
				children[j] = '1';
		}
}

void choose(char *parents[] , char *children[])
{
	int i;
	int count = 0;
	double p[NUMBER];
	double sum_fitness = 0.0;

	for(i=0 ; i < NUMBER ; i ++)
		sum_fitness += fitness(decode(parents[i]));

	for(i=0 ; i < NUMBER ; i ++)
		p[i] = fitness(decode(parents[i])) / sum_fitness;
	
	sum_p(p);	//计算累加概率

	for(i=0 ; i < NUMBER ; i +=2)
	{
		//轮盘赌选择
		strcpy(children[count] , parents[gamble(p)]);
		strcpy(children[count + 1] , parents[gamble(p)]);
		//进行交叉
		cross_over(children[count] , children[count + 1]);
		mutation(children[count]);
		mutation(children[count + 1]);

		count += 2;
	}
}

void init(char *parents[])
{
	int i;
	int j;
	char gene[2] = {'0' , '1'};


	for(i=0 ; i<NUMBER ; i ++)
	{
		for(j=0 ; j<BITS; j ++)
			parents[i][j] = gene[random()%2];

		parents[i][j] = '\0';
	}
}

int get_best(char *parents[])
{
	int i;
	int max = 0;

	for(i=1 ; i<NUMBER ; i ++)
		if(fitness(decode(parents[i])) > fitness(decode(parents[max])))
			max = i;

	return max;	
}

double best(double *max)
{
    int i;
    int best;

    for(i=0;i<EPOCH;i++)
        if(max[i]>=DOWN && max[i]<=UP)
        {
            best = i;
            break;
        }

    for(i=0 ; i<EPOCH ; i ++)
    {
        if(max[i]>=DOWN &&(max[i]<=UP))
            if(fitness(max[i]) > fitness(max[best]))
                best = i;
    }

    return max[best];
}

int main(void)
{
	char **parents = (char **)malloc(sizeof(char *) * NUMBER);
	char **children = (char **)malloc(sizeof(char *) * NUMBER);
	char **save = parents;
	int i;
	int max;
    double max_x[EPOCH];
    int counter = 0;

	for(i=0 ; i<NUMBER ; i ++)
	{
		//多一位存结束符
		parents[i] = (char *)malloc(BITS+1);
		children[i] = (char *)malloc(BITS+1);
	}

	//初始化种群
	init(parents);

	for(i=0 ; i<EPOCH ; i ++)
	{
		//选择 交叉 变异
		choose(parents , children);

		parents = children;

		max = get_best(children);

        max_x[counter ++] = decode(children[max]);

        printf("best in this epoch:[x:%lf fx:%lf]\n", decode(children[max]) , fitness(decode(children[max])));
	}

	for(i=0;i<NUMBER;i ++)
	{
		free(parents[i]);
		free(save[i]);
	}
	free(parents);
	free(save);

    double best_x = best(max_x);
    printf("best_x:%lf best_fx:%lf\n", best_x , fitness(best_x));

	return 0;
}