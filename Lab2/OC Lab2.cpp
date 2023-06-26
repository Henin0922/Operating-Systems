#include<iostream>
#include<Windows.h>
using namespace std;

int num = 0;
int min_n;
int max_n;
UINT StopFlag = 0;


DWORD WINAPI min_max(LPVOID lpParam)//线程函数1
{

	float minValue = FLT_MAX;
	float maxValue = FLT_MIN;

	for (int i = 0; i < num; i++) {
		if (minValue > ((float*)lpParam)[i]) {
			minValue = ((float*)lpParam)[i];
			min_n = i;
		}
		Sleep(7);//休息几毫秒
		if (maxValue < ((float*)lpParam)[i]) {
			maxValue = ((float*)lpParam)[i];
			max_n = i;
		}
		Sleep(7);
	}
	printf("\nMin:\n");
	printf("%f", minValue);
	printf("\nMax:\n");
	printf("%f", maxValue);

	return 0;
}
float av_val;

DWORD WINAPI average(LPVOID lpParam)//线程函数2
{
	av_val = 0;

	for (int i = 0; i < num; i++) {
		av_val += ((float*)lpParam)[i];
		Sleep(12);
	}
	av_val /= num;
	printf("\nAverage: \n");
	printf("%f", av_val);

	return 0;
}
int main()
{
	cin >> num;
	float* arr = new float[num];
	for (int i = 0; i < num; i++)
	{
		cin >> arr[i];
	}

	HANDLE hThread1, hThread2;

	hThread1 = ::CreateThread(0, 0, min_max, arr, 0, NULL);
	hThread2 = ::CreateThread(0, 0, average, arr, 0, NULL);
	while (StopFlag < 2)//两个线程都结束后StopFlag的值会增加到2，此时主线程停止等待，
	{
		Sleep(1);//休息几毫秒
	}
	system("pause");
	return 0;
}










