
#include "stdafx.h"

#define L_RATE 0.1  // 학습계수

#define INPUT_UNIT_NUM 8 // Number of Input Unit
#define HIDDEN_UNIT_NUM 6 // Number of Hidden Unit
#define OUTPUT_UNIT_NUM 10 // Number of Output Unit
#define BIAS 1
#define ERROR 0.01 // 오차 한계
#define MAXLOOP 2000000// 발산방지 수

double v[INPUT_UNIT_NUM + BIAS][HIDDEN_UNIT_NUM + 1]; // Input Layer와 Hidden Layer 간의 weight
double w[HIDDEN_UNIT_NUM + BIAS][OUTPUT_UNIT_NUM + 1]; // Hidden Layer와 Output Layer 간의 weight
double delta_j[HIDDEN_UNIT_NUM + 1]; 
double delta_k[OUTPUT_UNIT_NUM + 1];
double delta_v[INPUT_UNIT_NUM + BIAS][HIDDEN_UNIT_NUM + 1]; // 오류 수정값 
double delta_w[HIDDEN_UNIT_NUM + BIAS][OUTPUT_UNIT_NUM + 1]; // 오류 수정값
double Tk[OUTPUT_UNIT_NUM + 1]; // 기대값

double sigmoid(double); // Sigmoid Function
double sigmoid_inverse(double); // Sigmoid Inverse Function

struct unit // UNIT 을 구성하는 구조체
{
    double input;
    double output;
    struct unit *next;
};
struct YSdata
{
	int ys[INPUT_UNIT_NUM]; 
	int class_num;
};

struct unit *input_unit, *hidden_unit, *output_unit; // 각 Layer의 첫번째 UNIT

void init_unit(void); // 각층의 UNIT 생성
void init_weight(void); // 각 arc의 초기 weight값 설정
double learning (struct YSdata , double *); // BP 학습알고리즘
void compute_output (struct YSdata , double *); //output 계산
void finish_unit(void); // 생성된 Neural Network 파괴

#define TRANINGFILE "traning.dat"
#define TESTINGFILE "testing.dat"

int file_scanning (struct YSdata *, FILE *);
int test_file_scanning (struct YSdata *, FILE *);

void main (void)
{
	FILE *fp, *tfp;
	double e = ERROR;
	struct YSdata ipk;
	
    printf ("----------------------------------------------\n\n");

    printf("Initialing Unit and Weight.....\n\n");
    init_unit();
    init_weight();

	printf("\nLearnig....\n");

	if ((tfp = fopen(TESTINGFILE, "r")) == NULL)
    {
	printf("Error: Cannot open testing data\n");
	exit(0);
	}


	unsigned long imsi;

	for (imsi = 0; imsi < MAXLOOP; imsi++)
	{
		if (e < ERROR)
	        break;
	    else
	        e = 0;

		if ((fp = fopen(TRANINGFILE, "r")) == NULL)
		{
			printf("Error: Cannot open traning data\n");
			exit(0);
		}

		while (file_scanning (&ipk, fp) != 0)
		{
			for (int i = 1; i <= OUTPUT_UNIT_NUM; ++i)
			{
				if (ipk.class_num == i)
					Tk[i] = (double) 1;
				else
					Tk[i] = (double) 0;
			}
			e += learning (ipk, Tk);
			
			
		}

		printf ("%d\t", imsi);
		fclose (fp);
	}
	
	if (imsi == MAXLOOP)
	{
		printf ("\n\n Deverge....\n");
		finish_unit();
		fclose (tfp);
		exit(1);
	}

	printf ("\nEnd learning...\n\n");

	int count = 1;
	while (test_file_scanning (&ipk, tfp) != 0)
    {
		compute_output (ipk, Tk);

		for (int i_vec = 0; i_vec < INPUT_UNIT_NUM; ++i_vec)
		{
			printf("%d ", ipk.ys[i_vec]);
		}

		printf("\t=\t");

		for (int i = 1; i <= OUTPUT_UNIT_NUM; ++i)
		{
			printf ("%f\n", Tk[i]);
		}
		
		if(count == 6){
			count = 0;
			printf ("\n");
		}
		count++;
	}

    finish_unit();
	fclose (tfp);
}

void init_unit(void)
{
    int i;
    struct unit *tmp_1, *tmp_2;

    input_unit = tmp_1 = (struct unit *) malloc (sizeof (struct unit));
    for (i = 2 ;i <= INPUT_UNIT_NUM; ++i)
    {
	    tmp_2 = (struct unit *) malloc (sizeof (struct unit));
	    tmp_1->next = tmp_2;
	    tmp_1 = tmp_2;
    }
    tmp_1->next = NULL;

    hidden_unit = tmp_1 = (struct unit *) malloc (sizeof (struct unit));
    for (i = 2 ;i <= HIDDEN_UNIT_NUM ; ++i)
    {
	    tmp_2 = (struct unit *) malloc (sizeof (struct unit));
	    tmp_1->next = tmp_2;
	    tmp_1 = tmp_2;
    }
    tmp_1->next = NULL;

    output_unit = tmp_1 = (struct unit *) malloc (sizeof (struct unit));
    for (i = 2 ;i <= OUTPUT_UNIT_NUM ; ++i)
    {
	    tmp_2 = (struct unit *) malloc (sizeof (struct unit));
	    tmp_1->next = tmp_2;
	    tmp_1 = tmp_2;
    }
    tmp_1->next = NULL;

}

void init_weight(void)
{
	int i,j;

    for(i=0; i <= INPUT_UNIT_NUM; i++)
		for(j=1; j <= HIDDEN_UNIT_NUM; j++)
		{
	  	   v[i][j]=(double)rand()/RAND_MAX-0.5;
		}

    for(i=0; i <= HIDDEN_UNIT_NUM; i++)
	   for(j=1; j <= OUTPUT_UNIT_NUM; j++)
	   {
	 	   w[i][j]=(double)rand()/RAND_MAX-0.5;	
	   }
}

double learning (struct YSdata pk, double *Tk)
{
    int i, j, k;
    double sum;
    double e = ERROR;
	
    struct unit *tmp_i, *tmp_h, *tmp_o;
    //step3
    tmp_i = input_unit;

	for (i = 0; i < INPUT_UNIT_NUM ; ++i)
	{
		tmp_i->output = tmp_i->input = pk.ys[i];
		tmp_i = tmp_i->next;
	}
    
	//step4
    tmp_h = hidden_unit;
	    
    for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
	{
        tmp_i = input_unit;
        for (i = 1, sum = 0.0; i <= INPUT_UNIT_NUM ; ++i)
		{
	        sum += (tmp_i->output) * v[i][j];
	        tmp_i = tmp_i->next;
		}
        tmp_h->input = v[0][j] + sum;
        tmp_h->output = sigmoid(tmp_h->input);

        tmp_h = tmp_h->next;
	}
	//step5
    tmp_o = output_unit;
    for (k = 1; k <= OUTPUT_UNIT_NUM ; ++k)
	{
        tmp_h = hidden_unit;
        for (j = 1, sum = 0.0; j <= HIDDEN_UNIT_NUM ; ++j)
		{
	        sum += (tmp_h->output) * w[j][k];
			
	        tmp_h = tmp_h->next;
		}
        tmp_o->input = w[0][k] + sum;
        tmp_o->output = sigmoid(tmp_o->input);
		
        tmp_o = tmp_o->next;
	}

	//step6
    tmp_o = output_unit;
    for (k = 1; k <= OUTPUT_UNIT_NUM ; ++k)
	{
        delta_k[k] = (Tk[k] - tmp_o->output) * sigmoid_inverse (tmp_o->input);
        delta_w[0][k] = L_RATE * delta_k[k];

        tmp_o = tmp_o->next;
	}
    for (k = 1; k <= OUTPUT_UNIT_NUM ; ++k)
	{
        tmp_h = hidden_unit;
        for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
		{
	        delta_w[j][k] = L_RATE * delta_k[k] * tmp_h->output;
	        tmp_h = tmp_h->next;
		}
    }

	//step7
    tmp_h = hidden_unit;
    for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
	{
        tmp_o = output_unit;
        for (k = 1, sum = 0.0; k <= OUTPUT_UNIT_NUM ; ++k)
		{
	        sum += delta_k[k] * w[j][k];
	        tmp_o = tmp_o->next;
		}
        delta_j[j] = sum * sigmoid_inverse (tmp_h->input);
        delta_v[0][j] = L_RATE * delta_j[j];

        tmp_h = tmp_h->next;
	}
    for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
	{
        tmp_i = input_unit;
        for (i = 1; i <= INPUT_UNIT_NUM ; ++i)
		{
	        delta_v[i][j] = L_RATE * delta_j[j] * tmp_i->input;
	        tmp_i = tmp_i->next;
		}
    }

	//step8
	{
        for (j = 0; j <= HIDDEN_UNIT_NUM ; ++j)
		{
	        for (k = 1; k <= OUTPUT_UNIT_NUM ; ++k)
		        w[j][k] = w[j][k] + delta_w[j][k];
		}
        for (i = 0; i <= INPUT_UNIT_NUM ; ++i)
		{
	        for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
			    v[i][j] = v[i][j] + delta_v[i][j];
		}
	}
	//Compute error    
	for (e = 0.0, k = 1; k <= OUTPUT_UNIT_NUM ; k++)
	{
        e += delta_k[k] * delta_k[k];
	}

	return e;
}

void compute_output (struct YSdata pk, double *Tk)
{
    int i, j, k;
    double sum;

    struct unit *tmp_i, *tmp_h, *tmp_o;

    tmp_i = input_unit;
	for (i = 0; i < INPUT_UNIT_NUM ; ++i)
	{
		tmp_i->output = tmp_i->input = pk.ys[i];
		tmp_i = tmp_i->next;
	}

    tmp_h = hidden_unit;
    for (j = 1; j <= HIDDEN_UNIT_NUM ; ++j)
    {
	    tmp_i = input_unit;
	    for (i = 1, sum = 0.0; i <= INPUT_UNIT_NUM ; ++i)
		{
	        sum += (tmp_i->output) * v[i][j];
	        tmp_i = tmp_i->next;
		}
	    tmp_h->input = v[0][j] + sum;
	    tmp_h->output = sigmoid(tmp_h->input);


	    tmp_h = tmp_h->next;
    }

    tmp_o = output_unit;
    for (k = 1; k <= OUTPUT_UNIT_NUM ; ++k)
    {
	    tmp_h = hidden_unit;
	    for (j = 1, sum = 0.0; j <= HIDDEN_UNIT_NUM ; ++j)
		{
	        sum += (tmp_h->output) * w[j][k];
	        tmp_h = tmp_h->next;
		}
	    tmp_o->input = w[0][k] + sum;
	    tmp_o->output = sigmoid(tmp_o->input);
		//printf("%f ",tmp_o->output);
		// 여기서 tmp_o->next == NULL 일 때만 결과값을 넣으니까 마지막놈만 제대로 출력되는거였어 ㅋㅋㅋ 아나 존 나.
	    //if (tmp_o->next == NULL)
	        Tk[k] = tmp_o->output;
		
	    tmp_o = tmp_o->next;

    }


}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-1 * x));
}

double sigmoid_inverse(double x)
{
    double tmp = sigmoid(x);
    return tmp * (1.0 - tmp);
}

void finish_unit(void)
{
    struct unit *tmp, *ttmp;
    tmp = input_unit;
    while (tmp != NULL)
    {
	    ttmp = tmp->next;
	    free (tmp);
	    tmp = ttmp;
    }
    tmp = hidden_unit;
    while (tmp != NULL)
    {
	    ttmp = tmp->next;
	    free (tmp);
	    tmp = ttmp;
    }
    tmp = output_unit;
    while (tmp != NULL)
    {
	    ttmp = tmp->next;
	    free (tmp);
	    tmp = ttmp;
    }
}

int file_scanning (struct YSdata *out_pk, FILE *fp)
{
    int temp, i;
    struct YSdata pk;

    for (i = 0; i < INPUT_UNIT_NUM; i++)
	{
		if (fscanf(fp,"%d", &pk.ys[i]) == EOF)
		{
			return 0;
		}
	}		
	fscanf(fp,"%d", &pk.class_num);
	
	*out_pk = pk;
	temp = 1;
			
    return temp;
}

int test_file_scanning (struct YSdata *out_pk, FILE *fp)
{
    int i;
    struct YSdata pk;

    for (i = 0; i < INPUT_UNIT_NUM; i++)
	{
		if (fscanf(fp,"%d", &pk.ys[i]) == EOF)
		{
			return 0;
		}
	}		
	
    *out_pk = pk;
    return 1;
}

