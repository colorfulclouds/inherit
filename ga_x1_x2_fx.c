//linux
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.1415926

#define EPOCH 1000
#define NUMBER 200	//种群数量
#define	EPSILON	0.00001

#define P_C	0.99	//交叉概率
#define P_M	0.02	//变异概率

//二变量
#define DOWN_X1 -3.0
#define UP_X1 12.1
#define BITS_X1 ( (int)ceil( log((UP_X1 - DOWN_X1)/EPSILON+1) / log(2) ) )

#define DOWN_X2 4.1
#define UP_X2 5.8
#define BITS_X2 ( (int)ceil( log((UP_X2 - DOWN_X2)/EPSILON+1) / log(2) ) )

double random_()
{
	return (double)random()/RAND_MAX;
}

double decode_3d(char *b , double down , int bits)
{
	int temp = 0;
	int i;
	int j;
	int weight[33] = {0 , 2147483648 , 1073741824 , 536870912 , 268435456 , 134217728 , 67108864 , 33554432 , 16777216, 
						  8388608 , 4194304 , 2097152 , 1048576 , 524288 , 262144 , 131072 , 65536, 
						  32768 , 16384 , 8192 , 4096 , 2048 , 1024 , 512 , 256,
						  128 , 64 , 32, 16 , 8 , 4 , 2 , 1 };

	for(i=bits-1 , j=32 ; i>=0 ; i -- , j --)
		temp += (b[i] == '1' ? weight[j] : 0);

	return down + temp * EPSILON;
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

double function_3d(double x1_ , double x2_)
{
	return 21.5 + x1_*sin(4*PI*x1_) + x2_*sin(20*PI*x2_);
}

double fitness_3d(char *b)
{
	char x1[BITS_X1 + 1];
	char x2[BITS_X2 + 1];

	strcpy(x1 , b);
	x1[strlen(x1) - 1] = '\0';
	strcpy(x2 , b+BITS_X1);
	x2[strlen(x2) - 1] = '\0';

	double x1_ = decode_3d(x1 , DOWN_X1 , BITS_X1);
	double x2_ = decode_3d(x2 , DOWN_X2 , BITS_X2);

	return function_3d(x1_ , x2_);
}



int init_3d(char *parents[])
{
	int i;
	int j;
	char gene[2] = {'0' , '1'};

	for(i=0 ; i<NUMBER ; i ++)
	{
		for(j=0 ; j<BITS_X1 + BITS_X2; j ++)
			parents[i][j] = gene[random()%2];

		parents[i][j] = '\0';
	}	
}

void cross_over_3d(char *father , char *mother)
{
	//单点交叉
	int cross_pos;
	char *temp;

	if(P_C >= random_())
	{
		temp = (char *)malloc(BITS_X1 + BITS_X2 + 1);

		cross_pos = random() % (BITS_X1 + BITS_X2);	//随机产生交叉点

		strcpy(temp , father + cross_pos);
		strcpy(father + cross_pos , mother + cross_pos);
		strcpy(mother + cross_pos , temp);

		free(temp);
	}
}

void mutation_3d(char *children)
{
	int j;

	for(j=0 ; j < BITS_X1 + BITS_X2 ; j ++)
		if(P_M >= random_())
		{
			if(children[j] == '1')
				children[j] = '0';
			else
				children[j] = '1';
		}
}

void choose_3d(char *parents[] , char *children[])
{
	int i;
	int count = 0;
	double p[NUMBER];
	double sum_fitness = 0.0;

	for(i=0 ; i < NUMBER ; i ++)
		sum_fitness += fitness_3d(parents[i]);

	for(i=0 ; i < NUMBER ; i ++)
		p[i] = fitness_3d(parents[i]) / sum_fitness;
	
	
	sum_p(p);	//计算累加概率

	for(i=0 ; i < NUMBER ; i +=2)
	{
		//轮盘赌选择
		strcpy(children[count] , parents[gamble(p)]);
		strcpy(children[count + 1] , parents[gamble(p)]);
		//进行交叉
		cross_over_3d(children[count] , children[count + 1]);
		mutation_3d(children[count]);
		mutation_3d(children[count + 1]);

		count += 2;
	}
}

int get_best3d(char *parents[])
{
	int i;
	int max = 0;

	for(i=1 ; i<NUMBER ; i ++)
	{
		if(fitness_3d(parents[i]) > fitness_3d(parents[max]))
			max = i;
	}

	return max;
}

void best_3d(double max[][2] , double *x1 , double *x2)
{
	int i;
    int best;

    for(i=0;i<EPOCH;i++)
    {
        if( (max[i][0]>=DOWN_X1) && (max[i][0]<=UP_X1) && (max[i][0]>=DOWN_X2) && (max[i][1]<=UP_X2) )
        {
            best = i;
			break;
		}
    }

    for(i=0 ; i<EPOCH ; i ++)
    {
        if( (max[i][0]>=DOWN_X1) && (max[i][0]<=UP_X1) && (max[i][0]>=DOWN_X2) && (max[i][1]<=UP_X2) )
        {
			if(function_3d(max[i][0] , max[i][1]) > function_3d(max[best][0] , max[best][1]))
				best = i;
		}
    }

    (*x1) = max[best][0];
	(*x2) = max[best][1];
}

void multi_x()
{
	char **parents = (char **)malloc(sizeof(char *) * NUMBER);
	char **children = (char **)malloc(sizeof(char *) * NUMBER);
	char **save = parents;
	int i;
	int max;

	char x1[BITS_X1 + 1];
	char x2[BITS_X2 + 1];

	double max_x[EPOCH][2];
	int counter = 0;

	for(i=0 ; i<NUMBER ; i ++)
	{
		parents[i] = (char *)malloc(BITS_X1 + BITS_X2 + 1);
		children[i] = (char *)malloc(BITS_X1 + BITS_X2 + 1);
	}

	//初始化种群
	init_3d(parents);

	for(i=0 ; i<EPOCH ; i ++)
	{
		//选择 交叉 变异
		choose_3d(parents , children);

		parents = children;

		max = get_best3d(children);
		
		strcpy(x1 , children[max]);
		x1[strlen(x1)-1] = '\0';
		strcpy(x2 , children[max]+BITS_X1);
		x2[strlen(x2)-1] = '\0';
		
		max_x[counter][0] = decode_3d(x1 , DOWN_X1 , BITS_X1);
		max_x[counter][1] = decode_3d(x2 , DOWN_X2 , BITS_X2);

		printf("best in this epoch:[x1:%lf x2:%lf fx:%lf]\n", max_x[counter][0] , max_x[counter][1] , fitness_3d(children[max]));

		counter ++;
	}

	for(i=0;i<NUMBER;i ++)
	{
		free(parents[i]);
		free(save[i]);
	}
	free(parents);
	free(save);

	double best_x1;
	double best_x2;

	best_3d(max_x , &best_x1 , &best_x2);

	printf("======final best_x1:%lf best_x2:%lf best_fx:%lf\n", best_x1 , best_x2, function_3d(best_x1 , best_x2));
}

int main(void)
{
	multi_x();
	return 0;
}