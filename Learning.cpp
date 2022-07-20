#include <stdio.h>
#include <math.h>

float Sigmoid(float x)
{
    float y;
    y = 1/(1+exp(-x));
    
    return y;
}

float DeltaSigmoid(float x)
{
    float y;
    y = Sigmoid(1-Sigmoid(x));
    
    return y;
}

int Random()
{
    int x;
    int a=1103515245, c=12345, m=10000;
    
    FILE *Seed = fopen("RandomSeed.txt", "r");    
    fscanf(Seed, "%d", &x);
    fclose(Seed);
    
    x = (a*x+c)%m;
    if(x<0)
    	x = -x;
    
    FILE *NewSeed = fopen("RandomSeed.txt", "w");
    fprintf(NewSeed, "%d", x);
    fclose(NewSeed);
    
    return x;
}

int DecideMode()
{
	printf("\n���ϴ� �������� ��ȣ�� �Է����ּ��� \n1)��������н� \n2)�������н� \n3)�н��������ʱ�ȭ \n4)���α׷����� \n\n�Է¶�: ");
	
	int Mode;
	while(true)
	{
		scanf("%d", &Mode);
		
		switch(Mode)
		{
			case 1: case 2: case 3: case 4:
				return Mode;
			default:
				printf("1�� 3������ �ڿ����� �Է����ּ���.\n");
		}
	}
}

int DecideRunningTimes(int Mode)
{
	int Run = 1;
	
	if(Mode==1 || Mode==2)
	{
		printf("\n\n��� �н���Ű�ڽ��ϱ�? : ");
		
		while(true)
		{
			scanf("%d", &Run);
			if(Run >= 1)
				break;
			else
				printf("�ڿ����� �Է����ּ���.\n"); 
		} 
	}
	
	return Run;
}

int LearningDataReset()
{
	int Check;
	
	printf("\n\n��� ����ġ�� �н�Ƚ���� �ʱ�ȭ�˴ϴ�.");
	printf("\n���� �ʱ�ȭ�Ͻðڽ��ϱ�?");
	printf("\n1)��	2)�ƴϿ�\n\n�Է¶�: ");
	
	while(true)
	{
		scanf("%d", &Check);
		if(Check==1)
		{
		    FILE *LearningTimeData = fopen("LearningTimes.txt", "w");
		    fprintf(LearningTimeData, "%d", 0);
		    fclose(LearningTimeData);
		    
		    FILE *WeightData = fopen("WeightData.txt", "w");
    		FILE *MinusWeightData = fopen("MinusWeightData.txt", "w");
		    for(int k=0; k<10; k++)
		    {
		        for(int i=0; i<28; i++)
		        {
		            for(int j=0; j<28; j++)
		            {
		                fprintf(WeightData, "%d ", 100);
		                fprintf(MinusWeightData, "%d ", 0);
		            }
		        }
		    }
		    fclose(WeightData);
		    fclose(MinusWeightData);
		    
		    return 0;
		    
		}
		else if(Check==2)
		{
			return 0;
		}
		else
		{
			printf("�ڿ����� �Է����ּ���.\n");
		}
	} 
}

void Learning(int Mode, int Seed)
{
    FILE *Mnist = fopen("MNIST_test.txt", "r");
    float ImageData[28][28];
    int Target;
    for(int k=0; k<=Seed; k++)
    {
        fscanf(Mnist, "%d ", &Target);
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(Mnist, "%f ", &ImageData[i][j]);
                if(k == Seed)
                {
					if(Mode==1)
                    	printf("%3d", (int)ImageData[i][j]);	
                    ImageData[i][j] /= 255;
                }
            }
			if(k == Seed && Mode==1)
                printf("\n");
            
        }
    }
    fclose(Mnist);
    
    printf("\n\n");
    
    FILE *WeightData = fopen("WeightData.txt", "r");
    FILE *MinusWeightData = fopen("MinusWeightData.txt", "r");
    float Weight[10][28][28];
    int MinusWeight[10][28][28];
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(WeightData, "%f ", &Weight[k][i][j]);
                fscanf(MinusWeightData, "%d ", &MinusWeight[k][i][j]);
                
                Weight[k][i][j] /= 10000;
                if(MinusWeight[k][i][j] == 1)
                {
                    Weight[k][i][j] = -Weight[k][i][j];
                }
            }
        }
    }
    fclose(WeightData);
    fclose(MinusWeightData);
    
    float OutputLayer[10];
    float DeltaOutputOverSum[10];
    float Sum;
    for(int k=0; k<10; k++)
    {
        Sum = 0;
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                Sum += Weight[k][i][j]*ImageData[i][j];
            }
        }
        OutputLayer[k] = Sigmoid(Sum);
        DeltaOutputOverSum[k] = DeltaSigmoid(Sum);
    }
    
    float Result[2] = {0, 0};
    for(int k=0; k<10; k++)
    {
        printf("%f  ", OutputLayer[k]);
        if(Result[0] < OutputLayer[k])
        {
            Result[0] = OutputLayer[k];
            Result[1] = k;
        }
    }
    printf("\n\n������ ���� %d�Դϴ�.", (int)Result[1]);
    
    float DeltaErrorOverOutput[10];
    float ErrorLevel=0;
    for(int k=0; k<10; k++)
    {
        if(k==Target)
        {
            DeltaErrorOverOutput[k] = (OutputLayer[k]-1)/5;
            ErrorLevel += pow(1-OutputLayer[k], 2);
        }
        else
        {
            DeltaErrorOverOutput[k] = OutputLayer[k]/5;
            ErrorLevel += pow(OutputLayer[k], 2);
        }
    }
    ErrorLevel /= 10;
    
    printf("\n\n[����%s] (����: %d) (��������: %f)", (int)Result[1]==Target ? "����":"����", Target, ErrorLevel); 
    
    FILE *NewWeightData = fopen("WeightData.txt", "w");
    FILE *NewMinusWeightData = fopen("MinusWeightData.txt", "w");
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                Weight[k][i][j] -= DeltaErrorOverOutput[k]*DeltaOutputOverSum[k]*ImageData[i][j]/10;
                
                if(Weight[k][i][j] < 0)
                {
                    Weight[k][i][j] = -Weight[k][i][j];
                    MinusWeight[k][i][j] = 1;
                }
                else
                {
                    MinusWeight[k][i][j] = 0;
                }
                
                fprintf(NewWeightData, "%d ", (int)(Weight[k][i][j]*10000));
                fprintf(NewMinusWeightData, "%d ", MinusWeight[k][i][j]);
            }
        }
    }
    fclose(NewWeightData);
    fclose(NewMinusWeightData);
    
    int LearningTimes;
    FILE *LearningTimeData = fopen("LearningTimes.txt", "r");
    fscanf(LearningTimeData, "%d", &LearningTimes);
    fclose(LearningTimeData);
    LearningTimes++;
    FILE *NewLearningTimeData = fopen("LearningTimes.txt", "w");
    fprintf(NewLearningTimeData, "%d", LearningTimes);
    fclose(NewLearningTimeData);
    
    printf("\n\n����ġ ������Ʈ �Ϸ�  <%dȸ �н�>\n", LearningTimes);
    printf("\n---------------------------------------------------------------------------------------------\n\n");
}

int main()
{
    printf("<Mnist �ӽŷ����� �����մϴ�>\n");
    
    int Mode, Run, Seed;
    while(true)
    {
        Mode = DecideMode();
        Run = DecideRunningTimes(Mode);
        
        switch (Mode)
        {
        	case 1: case 2:
        		for(int i=0; i<Run; i++)
				{
					Seed = Random();
					Learning(Mode, Seed);
				}
				break;
			case 3:
				LearningDataReset();
				break;
			case 4:
				return 0;
		}	
    }
}
