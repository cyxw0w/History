#include<iostream>
#include<Windows.h>
#include<psapi.h>
using namespace std;

void init() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	cout << "Number of CPUs : "
		<< sysInfo.dwNumberOfProcessors
		<< endl;

	cout << "Index\t"
		<< "Time\t\t"
		<< "     "
		<< "Processes   "
		<< "   MemoryLoad   "
		<< "CPULoad"
		<< endl;
}
void time() {
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	cout << sysTime.wYear << "_"
		<< sysTime.wMonth << "_"
		<< sysTime.wDay << " "
		<< sysTime.wHour << "_"
		<< sysTime.wMinute << "_"
		<< sysTime.wSecond << "  \t";
}
void CountProcesses() {
	int Count;
	DWORD Proc[1024], Byte;
	EnumProcesses(Proc, sizeof(Proc), &Byte);
	Count = Byte / sizeof(DWORD);

	cout << Count << "\t\t";
}
void MemLoad() {
	MEMORYSTATUSEX memInfo;

	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	cout << memInfo.dwMemoryLoad << "%\t";
}
double cpu() {
	FILETIME idle, kernel, user;
	int i = 0;
	double r = 0;

	GetSystemTimes(&idle, &kernel, &user);
	DWORD lkHigh = kernel.dwHighDateTime; //late kernel HighDateTime
	DWORD lkLow = kernel.dwLowDateTime;
	DWORD luHigh = user.dwHighDateTime;
	DWORD luLow = user.dwLowDateTime;
	DWORD liHigh = idle.dwHighDateTime;
	DWORD liLow = idle.dwLowDateTime;

	Sleep(500);

	GetSystemTimes(&idle, &kernel, &user);
	DWORD nkHigh = kernel.dwHighDateTime; //current kernel HighDateTime
	DWORD nkLow = kernel.dwLowDateTime;
	DWORD nuHigh = user.dwHighDateTime;
	DWORD nuLow = user.dwLowDateTime;
	DWORD niHigh = idle.dwHighDateTime;
	DWORD niLow = idle.dwLowDateTime;
	while (i <= 5) {
		double ke = (nkHigh + nkLow) - (lkHigh + lkLow); //current - late;
		double us = (nuHigh + nuLow) - (luHigh + luLow);
		double id = (niHigh + niLow) - (liHigh + liLow);
		double res = (us + ke - id) * 100 / (us + ke); 
		// os + user *100 / os + user + idle  => ke - id + us *100 / ke + us 
		Sleep(100);
		r = r + res;
		i++;
		
	}
	return r / 5;
}
void cpuAvg(double * arr) {
	
}
void main() {	
	init();
	int index = 0;
	int i = 0;
	static double avgArr[1024]; // cpu 평균값 구하기 용 (총 1024초 분량 저장)
		
	while (true) {
		double val = cpu();
		double avg;
		double sum = 0.;
		cout << index << "\t";
		time();
		CountProcesses();
		MemLoad();						
		
		avgArr[index] = val;

		cout << fixed;
		cout.precision(2);
		cout << "[CPU Load:" << val << "\t]";
		
		if (i >= 5) {
			for (int j = index - 5; j < index; j++) {				
				sum += avgArr[j];				
			}
			avg = sum / 5.;
			sum = 0.;
			cout << "[5s avg:" << avg << "\t]";
			if (i >= 10) {
				for (int x = index - 10; x <= index; x++) {
					sum += avgArr[x];
				}
				avg = sum / 10.;
				sum = 0.;
				cout << "[10s avg:" << avg << "\t]";
				if (i >= 15) {
					for (int x = index - 15; x <= index; x++) {
						sum += avgArr[x];
					}
					avg = sum / 15.;
					sum = 0.;
					cout << "[15s avg:" << avg << "\t]"<<endl;
				}
				else
					cout << endl;
			}
			else
				cout << endl;
		}
		else
			cout << endl;
		
		index++;
		i++;	
	}
	return;
}