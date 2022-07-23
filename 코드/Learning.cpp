#include <stdio.h>
#include <math.h>

float Sigmoid(float x)    //시그모이드함수: 활성함수
{
    float y;
    y = 1/(1+exp(-x));    // 시그모이드함수: y=1/(1+e^(-x))

    return y;
}

float DeltaSigmoid(float x)    // 시그모이드함수를 미분한 함수
{
    float y;
    y = Sigmoid(1-Sigmoid(x));    // 시그모이드미분함수:  y=(e^(-x))/((1+e^(-x))^2)
    
    return y;
}

int Random()    // 랜덤함수: 0~9999사이의 임의의 정수를 출력 (선형합동법 이용) 
{
    int x;
    int a=1103515245, c=12345, m=10000;
    
    FILE *Seed = fopen("RandomSeed.txt", "r");    // "RandomSeed.txt"에서 시드값을 가져옴 
    fscanf(Seed, "%d", &x);
    fclose(Seed);
    
    x = a*x+c;
    if(x<0)    // 오버플로우에 대처하기 위하여 
    	x = -x;
    x %= m;
    
    FILE *NewSeed = fopen("RandomSeed.txt", "w");    // 현재값을 다음번 시드값으로 활용하기 위해 "RandomSeed.txt"에 저장 
    fprintf(NewSeed, "%d", x);
    fclose(NewSeed);
    
    return x;
}

int DecideMode()    // 프로그램으로 할 작업을 선택하는 함수 
{
	printf("\n원하는 선택지의 번호를 입력해주세요");
	printf("\n1)과정출력학습");
	printf("\n2)결과출력학습");
	printf("\n3)학습데이터초기화"); 
	printf("\n4)프로그램종료");
	printf("\n\n입력란: ");
	
	int Mode;
	while(true)    // 제대로 입력헀는지 확인 및 입력값 출력 
	{
		scanf("%d", &Mode);
		
		switch(Mode)
		{
			case 1: case 2: case 3: case 4:
				return Mode;
			default:
				printf("1과 3사이의 자연수를 입력해주세요.\n");
		}
	}
}

int DecideRunningTimes(int Mode)    // 학습횟수 선택함수: 과정출력학습 또는 결과출력학습을 선택했을때 작동
{
	int Run = 1;
	
	printf("\n\n몇번 학습시키겠습니까? : ");
	
	while(true)    // 제대로 입력헀는지 확인 및 입력값 출력 
	{
		scanf("%d", &Run);
		if(Run >= 1)
			return Run;
		else
			printf("자연수를 입력해주세요.\n"); 
	} 
}

int LearningDataReset()    // 학습데이터초기화 함수: 학습데이터초기화를 선택했을때 작동
{
	int Check;
	
	printf("\n\n모든 가중치와 학습횟수가 초기화됩니다.");
	printf("\n정말 초기화하시겠습니까?");
	printf("\n1)예	2)아니오");
	printf("\n\n입력란: ");
	
	while(true)    // 제대로 입력헀는지 확인 및 초기화 실행 
	{
		scanf("%d", &Check);
		if(Check==1)
		{ 
		    FILE *LearningTimeData = fopen("LearningTimes.txt", "w");    // 학습횟수 초기화 
		    fprintf(LearningTimeData, "%d", 0);
		    fclose(LearningTimeData);
		    
		    FILE *WeightData = fopen("WeightData.txt", "w");    // 가중치초기화  
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
			printf("자연수를 입력해주세요.\n");
		}
	} 
}

void Learning(int Mode, int Seed)    //기계학습 함수: 프로그램의 핵심 
{
    FILE *Mnist = fopen("MNIST_test.txt", "r");    // MNIST데이터가  들어있는 파일을 읽기모드로 열기 
    float ImageData[28][28];    // MNIST데이터를 저장하기 위한 배열 
    int Target;    // MNIST데이터의 정답을 저장하기 위한 변수 
    for(int k=0; k<=Seed; k++)    // (랜덤함수에서 받은 값)번째에 위치한 데이터만 사용 
    {
        fscanf(Mnist, "%d ", &Target);
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(Mnist, "%f ", &ImageData[i][j]);
                if(k == Seed) 
                {
					if(Mode==1)   // 과정출력학습을 선택했을때 
                    	printf("%3d", (int)ImageData[i][j]);	
                    ImageData[i][j] /= 255;    // 0~255사이의 데이터를 0~1사이로 변환 
                }
            }
			if(k == Seed && Mode==1)
                printf("\n");
            
        }
    }
    fclose(Mnist);
    
    printf("\n\n");
    
    FILE *WeightData = fopen("WeightData.txt", "r");    // 가중치가 들어있는 파일을 읽기모드로 열기 
    FILE *MinusWeightData = fopen("MinusWeightData.txt", "r");    // 가중치가 음수인지 양수인지에 대한 정보가 담긴 파일을 읽기모드로 열기 
    float Weight[10][28][28];    // 가중치를 저장하기 위한 배열 
    int MinusWeight[10][28][28];    // 가중치가 음수인지 양수인지에 대한 정보를 저장하기 위한 배열 
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                fscanf(WeightData, "%f ", &Weight[k][i][j]);
                fscanf(MinusWeightData, "%d ", &MinusWeight[k][i][j]);
                
                Weight[k][i][j] /= 10000;    // 파일을 읽을때 소수를 읽을 수 없어서 정수로 저장해둔 값을 실수로 변경 (가중치를 저장할 때 10000배하여 정수로 저장했기 때문에) 
                if(MinusWeight[k][i][j] == 1)    // 파일을 읽을때 음수를 읽을 수 없어서 양수로 저장해둔 값을 음수로 변경 (MinusWeight가 1이라면 가중치가 음수, 0이라면 가중치가 양수)
                {
                    Weight[k][i][j] = -Weight[k][i][j];
                }
            }
        }
    }
    fclose(WeightData);
    fclose(MinusWeightData);
    
    float OutputLayer[10];    // 출력층의 결과값을 저장할 배열 
    float Sum;    // 입력층*가중치의 총합을 저장할 배열 
    float DeltaOutputOverSum[10];    // 결과값을  총합에 대해 미분한 값(즉, 활성함수를 미분한 값)을 저장할 배열 
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
        OutputLayer[k] = Sigmoid(Sum);    // 총합을 활성함수에 넣어 결과값 저장 
        DeltaOutputOverSum[k] = DeltaSigmoid(Sum);    // 결과값을  총합에 대해 미분한 값 저장 
    }
    
    float Result[2] = {0, 0};    // 출력층에서 최댓값을 선별하기 위한 배열 (Result[0]: 결과값의 최댓값 / Result[1]: 최댓값을 내보낸 출력층의 노드의 인덱스) 
    for(int k=0; k<10; k++)
    {
        printf("%f  ", OutputLayer[k]);
        if(Result[0] < OutputLayer[k])
        {
            Result[0] = OutputLayer[k];
            Result[1] = k;
        }
    }
    printf("\n\n예측된 값은 %d입니다.", (int)Result[1]);
    
    float ErrorLevel=0;    // 오차율을 저장하기 위한 변수 (MSE 이용) 
	float DeltaErrorOverOutput[10];    // 오차율을 결과값에 미분한 값을 저장할 배열 
    for(int k=0; k<10; k++)
    {
        if(k==Target)
        {
            DeltaErrorOverOutput[k] = (OutputLayer[k]-1)/5;    // 오차율을 결과값에 미분한 값을 저장 
            ErrorLevel += pow(1-OutputLayer[k], 2);
        }
        else
        {
            DeltaErrorOverOutput[k] = OutputLayer[k]/5;    // 오차율을 결과값에 미분한 값을 저장
            ErrorLevel += pow(OutputLayer[k], 2);
        }
    }
    ErrorLevel /= 10;
    
    printf("\n\n[예측%s] (정답: %d) (오차수준: %f)", (int)Result[1]==Target ? "성공":"실패", Target, ErrorLevel); 
    
    FILE *NewWeightData = fopen("WeightData.txt", "w");    // 가중치가 들어있는 파일을 쓰기모드로 열기 
    FILE *NewMinusWeightData = fopen("MinusWeightData.txt", "w");    // 가중치가 음수인지 양수인지에 대한 정보가 담긴 파일을 쓰기모드로 열기
    for(int k=0; k<10; k++)
    {
        for(int i=0; i<28; i++)
        {
            for(int j=0; j<28; j++)
            {
                Weight[k][i][j] -= DeltaErrorOverOutput[k]*DeltaOutputOverSum[k]*ImageData[i][j]*3;    // 가중치 업데이트 (경사하강법 이용) 
                
                if(Weight[k][i][j] < 0)
                {
                    Weight[k][i][j] = -Weight[k][i][j];    // 파일을 읽을때 음수를 읽을 수 없어서 가중치가 음수라면 양수로 변환 
                    MinusWeight[k][i][j] = 1;    // 가중치가 음수라면 MinusWeight에 1을 저장 
                }
                else
                {
                    MinusWeight[k][i][j] = 0;    // 가중치가 음수라면 MinusWeight에 1을 저장 
                }
                
                fprintf(NewWeightData, "%d ", (int)(Weight[k][i][j]*10000));    // 파일을 읽을때 소수를 읽을 수 없어서 10000배하여 정수로 저장
                fprintf(NewMinusWeightData, "%d ", MinusWeight[k][i][j]);    // 가중치가 음수인지 양수인지에 대한 정보 저장 
            }
        }
    }
    fclose(NewWeightData);
    fclose(NewMinusWeightData);
    
    int LearningTimes;    // 학습횟수를 저장할 변수 
    FILE *LearningTimeData = fopen("LearningTimes.txt", "r");   // 학습횟수가 들어있는 파일을 읽기모드로 열기 
    fscanf(LearningTimeData, "%d", &LearningTimes);
    fclose(LearningTimeData);
    LearningTimes++;    // 학습횟수+1 
    FILE *NewLearningTimeData = fopen("LearningTimes.txt", "w");    // 학습횟수가 들어있는 파일을 쓰기모드로 열기
    fprintf(NewLearningTimeData, "%d", LearningTimes);
    fclose(NewLearningTimeData);
    
    printf("\n\n가중치 업데이트 완료  <%d회 학습>\n", LearningTimes);
    printf("\n---------------------------------------------------------------------------------------------\n\n");
}

int main()
{
    printf("<Mnist 머신러닝을 시작합니다>\n");
    
    int Mode, Run, Seed;    // (Mode: 프로그램으로 할 작업) (Run: 학습할 횟수) (Seed: 랜덤함수의 결과값) 
    while(true)
    {
        Mode = DecideMode();    // 프로그램으로 할 작업 선택 
        
        switch (Mode)
        {
        	case 1: case 2:    // (Mode=1: 과정출력학습)  (Mode=2: 결과출력학습) 
        		Run = DecideRunningTimes(Mode);    // 학습횟수 선택 
        		for(int i=0; i<Run; i++)
				{
					Seed = Random();
					Learning(Mode, Seed);
				}
				break;
			case 3:    // 학습데이터초기화 
				LearningDataReset();
				break;
			case 4:    // 프로그램 종료 
				return 0;
		}	
    }
}
