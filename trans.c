#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct result
{
    int result_one;
    int result_two;
    
    double result_one_f;
    double result_two_f;

    char result_one_c[sizeof(int) * 8 + 2];
    char result_two_c[sizeof(int) * 8 + 2];
}result_t;


typedef struct result_f
{
    result_t result_int;
    result_t result_deci;
}result_f_t;

void d2b(int d_number , char *b_number , int b_bit)
{
    memset(b_number , (int)'0' , b_bit);
    b_number[0] = d_number > 0 ? '0' : (d_number *= (-1) , '1');
    b_number[b_bit - 1] = '\0';

    int pointer = b_bit - 2;
    char bit[2] = {'0' , '1'};

    while(d_number != 0)
    {
        b_number[pointer --] = bit[d_number % 2];
        d_number /= 2;
    }

//    puts(b_number);
}

int b2d(char *b_number , int b_bit)
{
    int bin_weight[33] = {0 , 2147483648 , 1073741824 , 536870912 , 268435456 , 134217728 , 67108864 , 33554432 , 16777216, 
						  8388608 , 4194304 , 2097152 , 1048576 , 524288 , 262144 , 131072 , 65536, 
						  32768 , 16384 , 8192 , 4096 , 2048 , 1024 , 512 , 256,
						  128 , 64 , 32, 16 , 8 , 4 , 2 , 1 };
    int bit_limit = b_bit - 1;
    int i;
    int result = 0;
    for(i = 1 ; i < bit_limit ; i ++)
    {
        result += (b_number[i] == '1' ? bin_weight[i] : 0);
    }
    return b_number[0] == '0' ? result : result * -1;
}

//single cross point 
void op_cross(int number_a , int number_b , int pos , result_t *children)
{
    int b_bit = sizeof(int) * 8 + 2;
    char *number_a_c = (char *)malloc(b_bit);
    if(number_a_c == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char *number_b_c = (char *)malloc(b_bit);
    if(number_b_c == NULL)
    {
        free(number_a_c);
        exit(EXIT_FAILURE);
    }

    d2b(number_a , number_a_c , b_bit);
    d2b(number_b , number_b_c , b_bit);

    strcpy(children->result_one_c , number_a_c);
    strcpy(children->result_one_c + pos , number_b_c + pos);

    strcpy(children->result_two_c , number_b_c);
    strcpy(children->result_two_c + pos , number_a_c + pos);

    children->result_one = b2d(children->result_one_c , b_bit);
    children->result_two = b2d(children->result_two_c , b_bit);

    free(number_a_c);
    free(number_b_c);
}

void d2b_f(double d_number , char *b_number_int , char *b_number_deci , int b_bit_int , int b_bit_deci , int precision)
{
    int d_number_int = (int)d_number;
    double d_number_deci = d_number - (double)d_number_int;

    if(b_number_int != NULL)
        d2b(d_number_int , b_number_int , b_bit_int);
    
    memset(b_number_deci , '\0' , b_bit_deci);
    
    int i;
    int pointer = 0;
    for(i = 0 ; i < precision ; i ++)
    {
        b_number_deci[pointer ++] = ( (int)(d_number_deci *= 2) == 0 ? '0' : '1') ;
        d_number_deci -= ((int)d_number_deci) ;
    }
}

double b2d_f(char *b_number_deci , int b_bit)
{
    double bin_weight_f[32] = {0.5 , 0.25 , 0.125 , 0.0625 , 0.03125 , 0.015625};
    double result = 0.0;
    int i;

    for(i = 0 ; b_number_deci[i] != '\0' ; i ++)
    {
        result += (b_number_deci[i] == '1' ? bin_weight_f[i] : 0.0);
    }

    return result;
}

void op_cross_f(double number_a , double number_b , int pos_int , int pos_deci , result_f_t *children , int precision)
{
    op_cross((int)number_a , (int)number_b , pos_int , &(children->result_int));
    
    int b_bit = sizeof(int) + 2;

    char *number_a_deci_c = (char *)malloc(b_bit);
    if(number_a_deci_c == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char *number_b_deci_c = (char *)malloc(b_bit);
    if(number_b_deci_c == NULL)
    {
        free(number_a_deci_c);
        exit(EXIT_FAILURE);
    }
    
    d2b_f(number_a , NULL , number_a_deci_c , -1 , b_bit , precision);
//printf("str1 %s\n", number_a_deci_c);
    d2b_f(number_b , NULL , number_b_deci_c , -1 , b_bit , precision);
//printf("str2 %s\n", number_b_deci_c);
    pos_deci = (pos_deci > precision ? precision : pos_deci) - 1;//start with 0
    strcpy(children->result_deci.result_one_c , number_a_deci_c);
    strcpy(children->result_deci.result_one_c + pos_deci , number_b_deci_c + pos_deci);

    strcpy(children->result_deci.result_two_c , number_b_deci_c);
    strcpy(children->result_deci.result_two_c + pos_deci , number_a_deci_c + pos_deci);

    children->result_deci.result_one_f = b2d_f(children->result_deci.result_one_c , b_bit);
    children->result_deci.result_two_f = b2d_f(children->result_deci.result_two_c , b_bit);

    free(number_a_deci_c);
    free(number_b_deci_c);

}

int main(void)
{
    /*
    result_t test;
    int a;
    int b;

    scanf("%d%d", &a , &b);

    op_cross(a , b , 29 , &test);

    puts("-===");
    puts(test.result_one_c);
    puts(test.result_two_c);

    printf("number %d %d\n", test.result_one , test.result_two);
    */
    result_f_t test;

    double number_a;
    double number_b;
    scanf("%lf %lf", &number_a , &number_b);
op_cross_f(number_a , number_b , 29  , 3 , &test , 5);


printf("%d %d\n", test.result_int.result_one , test.result_int.result_two);
puts(test.result_int.result_one_c);
puts(test.result_int.result_two_c);

    puts("deci");
    printf("%f %f\n", test.result_deci.result_one_f , test.result_deci.result_two_f);
    puts(test.result_deci.result_one_c);
    puts(test.result_deci.result_two_c);

    return 0;
}
