#include <stdio.h>
#include <math.h>

float Sigmoid(float x)    //�ñ׸��̵��Լ�: Ȱ���Լ�
{
    float y;
    y = 1/(1+exp(-x));    // �ñ׸��̵��Լ�: y=1/(1+e^(-x))

    return y;
}

float DeltaSigmoid(float x)    // �ñ׸��̵��Լ��� �̺��� �Լ�
{
    float y;
    y = Sigmoid(1-Sigmoid(x));    // �ñ׸��̵�̺��Լ�:  y=(e^(-x))/((1+e^(-x))^2)
    
    return y;
}

int Random()    // �����Լ�: 0~9999������ ������ ������ ��� (�����յ��� �̿�) 
{
    int x;
    int a=1103515245, c=12345, m=10000;
    
    FILE *Seed = fopen("RandomSeed.txt", "r");    // "RandomSeed.txt"���� �õ尪�� ������ 
    fscanf(Seed, "%d", &x);
    fclose(Seed);
    
    x = a*x+c;
    if(x<0)    // �����÷ο쿡 ��ó�ϱ� ���Ͽ� 
    	x = -x;
    x %= m;
    
    FILE *NewSeed = fopen("RandomSeed.txt", "w");    // ���簪�� ������ �õ尪���� Ȱ���ϱ� ���� "RandomSeed.txt"�� ���� 
    fprintf(NewSeed, "%d", x);
    fclose(NewSeed);
    
    return x;
}

int DecideMode()    // ���α׷����� �� �۾��� �����ϴ� �Լ� 
{
	printf("\n���ϴ� �������� ��ȣ�� �Է����ּ���");
	printf("\n1)��������н�");
	printf("\n2)�������н�");
	printf("\n3)�н��������ʱ�ȭ"); 
	printf("\n4)���α׷�����");
	printf("\n\n�Է¶�: ");
	
	int Mode;
	while(true)    // ����� �Է������� Ȯ�� �� �Է°� ��� 
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

int DecideRunningTimes(int Mode)    // �н�Ƚ�� �����Լ�: ��������н� �Ǵ� �������н��� ���������� �۵�
{
	int Run = 1;
	
	printf("\n\n��� �н���Ű�ڽ��ϱ�? : ");
	
	while(true)    // ����� �Է������� Ȯ�� �� �Է°� ��� 
	{
		scanf("%d", &Run);
		if(Run >= 1)
			return Run;
		else
			printf("�ڿ����� �Է����ּ���.\n"); 
	} 
}

int LearningDataReset()    // �н��������ʱ�ȭ �Լ�: �н��������ʱ�ȭ�� ���������� �۵�
{
	int Check;
	
	printf("\n\n��� ����ġ�� �н�Ƚ���� �ʱ�ȭ�˴ϴ�.");
	printf("\n���� �ʱ�ȭ�Ͻðڽ��ϱ�?");
	printf("\n1)��	2)�ƴϿ�");
	printf("\n\n�Է¶�: ");
	
	while(true)    // ����� �Է������� Ȯ�� �� �ʱ�ȭ ���� 
	{
		scanf("%d", &Check);
		if(Check==1)
		{ 
		    FILE *LearningTimeData = fopen("LearningTimes.txt", "w");    // �н�Ƚ�� �ʱ�ȭ 
		    fprintf(LearningTimeData, "%d", 0);
		    fclose(LearningTimeData);
		    
		    FILE *WeightData = fopen("WeightData.txt", "w");    // ����ġ�ʱ�ȭ  
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

void Learning(int Mode, int Seed)    //����н� �Լ�: ���α׷��� �ٽ� 
{
    FILE *Mnist = fopen("MNIST_test.txt", "r");    // MNIST�����Ͱ�  ����ִ� ������ �б���� ���� 
    float ImageData[28][28];    // MNIST�����͸� �����ϱ� ���� �迭 
    int Target;    // MNIST�������� ������ �����ϱ� ���� ���� 
    for(int k=0; k<=Seed; k++)    // (�����Լ����� ���� ��)��°�� ��ġ�� �����͸� ��� 
    {
        fscanf(Mnist, "%d ", &Target);
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(Mnist, "%f ", &ImageData[i][j]);
                if(k == Seed) 
                {
					if(Mode==1)   // ��������н��� ���������� 
                    	printf("%3d", (int)ImageData[i][j]);	
                    ImageData[i][j] /= 255;    // 0~255������ �����͸� 0~1���̷� ��ȯ 
                }
            }
			if(k == Seed && Mode==1)
                printf("\n");
            
        }
    }
    fclose(Mnist);
    
    printf("\n\n");
    
    FILE *WeightData = fopen("WeightData.txt", "r");    // ����ġ�� ����ִ� ������ �б���� ���� 
    FILE *MinusWeightData = fopen("MinusWeightData.txt", "r");    // ����ġ�� �������� ��������� ���� ������ ��� ������ �б���� ���� 
    float Weight[10][28][28];    // ����ġ�� �����ϱ� ���� �迭 
    int MinusWeight[10][28][28];    // ����ġ�� �������� ��������� ���� ������ �����ϱ� ���� �迭 
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(WeightData, "%f ", &Weight[k][i][j]);
                fscanf(MinusWeightData, "%d ", &MinusWeight[k][i][j]);
                
                Weight[k][i][j] /= 10000;    // ������ ������ �Ҽ��� ���� �� ��� ������ �����ص� ���� �Ǽ��� ���� (����ġ�� ������ �� 10000���Ͽ� ������ �����߱� ������) 
                if(MinusWeight[k][i][j] == 1)    // ������ ������ ������ ���� �� ��� ����� �����ص� ���� ������ ���� (MinusWeight�� 1�̶�� ����ġ�� ����, 0�̶�� ����ġ�� ���)
                {
                    Weight[k][i][j] = -Weight[k][i][j];
                }
            }
        }
    }
    fclose(WeightData);
    fclose(MinusWeightData);
    
    float OutputLayer[10];    // ������� ������� ������ �迭 
    float Sum;    // �Է���*����ġ�� ������ ������ �迭 
    float DeltaOutputOverSum[10];    // �������  ���տ� ���� �̺��� ��(��, Ȱ���Լ��� �̺��� ��)�� ������ �迭 
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
        OutputLayer[k] = Sigmoid(Sum);    // ������ Ȱ���Լ��� �־� ����� ���� 
        DeltaOutputOverSum[k] = DeltaSigmoid(Sum);    // �������  ���տ� ���� �̺��� �� ���� 
    }
    
    float Result[2] = {0, 0};    // ��������� �ִ��� �����ϱ� ���� �迭 (Result[0]: ������� �ִ� / Result[1]: �ִ��� ������ ������� ����� �ε���) 
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
    
    float ErrorLevel=0;    // �������� �����ϱ� ���� ���� (MSE �̿�) 
	float DeltaErrorOverOutput[10];    // �������� ������� �̺��� ���� ������ �迭 
    for(int k=0; k<10; k++)
    {
        if(k==Target)
        {
            DeltaErrorOverOutput[k] = (OutputLayer[k]-1)/5;    // �������� ������� �̺��� ���� ���� 
            ErrorLevel += pow(1-OutputLayer[k], 2);
        }
        else
        {
            DeltaErrorOverOutput[k] = OutputLayer[k]/5;    // �������� ������� �̺��� ���� ����
            ErrorLevel += pow(OutputLayer[k], 2);
        }
    }
    ErrorLevel /= 10;
    
    printf("\n\n[����%s] (����: %d) (��������: %f)", (int)Result[1]==Target ? "����":"����", Target, ErrorLevel); 
    
    FILE *NewWeightData = fopen("WeightData.txt", "w");    // ����ġ�� ����ִ� ������ ������� ���� 
    FILE *NewMinusWeightData = fopen("MinusWeightData.txt", "w");    // ����ġ�� �������� ��������� ���� ������ ��� ������ ������� ����
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                Weight[k][i][j] -= DeltaErrorOverOutput[k]*DeltaOutputOverSum[k]*ImageData[i][j]*3;    // ����ġ ������Ʈ (����ϰ��� �̿�) 
                
                if(Weight[k][i][j] < 0)
                {
                    Weight[k][i][j] = -Weight[k][i][j];    // ������ ������ ������ ���� �� ��� ����ġ�� ������� ����� ��ȯ 
                    MinusWeight[k][i][j] = 1;    // ����ġ�� ������� MinusWeight�� 1�� ���� 
                }
                else
                {
                    MinusWeight[k][i][j] = 0;    // ����ġ�� ������� MinusWeight�� 1�� ���� 
                }
                
                fprintf(NewWeightData, "%d ", (int)(Weight[k][i][j]*10000));    // ������ ������ �Ҽ��� ���� �� ��� 10000���Ͽ� ������ ����
                fprintf(NewMinusWeightData, "%d ", MinusWeight[k][i][j]);    // ����ġ�� �������� ��������� ���� ���� ���� 
            }
        }
    }
    fclose(NewWeightData);
    fclose(NewMinusWeightData);
    
    int LearningTimes;    // �н�Ƚ���� ������ ���� 
    FILE *LearningTimeData = fopen("LearningTimes.txt", "r");   // �н�Ƚ���� ����ִ� ������ �б���� ���� 
    fscanf(LearningTimeData, "%d", &LearningTimes);
    fclose(LearningTimeData);
    LearningTimes++;    // �н�Ƚ��+1 
    FILE *NewLearningTimeData = fopen("LearningTimes.txt", "w");    // �н�Ƚ���� ����ִ� ������ ������� ����
    fprintf(NewLearningTimeData, "%d", LearningTimes);
    fclose(NewLearningTimeData);
    
    printf("\n\n����ġ ������Ʈ �Ϸ�  <%dȸ �н�>\n", LearningTimes);
    printf("\n---------------------------------------------------------------------------------------------\n\n");
}

int main()
{
    printf("<Mnist �ӽŷ����� �����մϴ�>\n");
    
    int Mode, Run, Seed;    // (Mode: ���α׷����� �� �۾�) (Run: �н��� Ƚ��) (Seed: �����Լ��� �����) 
    while(true)
    {
        Mode = DecideMode();    // ���α׷����� �� �۾� ���� 
        
        switch (Mode)
        {
        	case 1: case 2:    // (Mode=1: ��������н�)  (Mode=2: �������н�) 
        		Run = DecideRunningTimes(Mode);    // �н�Ƚ�� ���� 
        		for(int i=0; i<Run; i++)
				{
					Seed = Random();
					Learning(Mode, Seed);
				}
				break;
			case 3:    // �н��������ʱ�ȭ 
				LearningDataReset();
				break;
			case 4:    // ���α׷� ���� 
				return 0;
		}	
    }
}
