#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <Windows.h>

using namespace std;

int main()
{
	system("chcp 1251");
	system("CLS");

	STARTUPINFOA cif;
	ZeroMemory(&cif, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION pi;

	time_t _time;
	_time = time(0);
	cout << ctime(&_time) << endl;

	Sleep(5000);

	CreateProcess("D:/Ëàáû ÑÏÎÂÌ/Time/Project1/Debug/Project1.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif, &pi);

	return 0;
}