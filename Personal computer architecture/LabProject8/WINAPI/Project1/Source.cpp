#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <fstream>

using namespace std;

int main() {
	HANDLE com2 = CreateFileA("COM2", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (!com2) {
		cout << errno << '\n';
		exit(0);
	}
	DCB dcb;
	GetCommState(com2, &dcb);
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	if (!SetCommState(com2, &dcb)) {
		printf("Попытка вызвать изменить состояние com2 провалилась!\n");
		return 1;
	}
	char choice;
	DWORD count = 0;
	ReadFile(com2, &choice, 1, &count, NULL);
	switch (choice) {
	case '1':
	{
		while (true) {
			system("cls");
			char temp, temp2;
			DWORD readed = 0;
			ReadFile(com2, &temp, 1, &readed, NULL);
			for (; temp > 0; temp--) {
				ReadFile(com2, &temp2, 1, &readed, NULL);
				cout << temp2;
			}
			cout << "0 - exit" << endl << "Your choice: ";
			char signal;
			cin >> signal;
			WriteFile(com2, &signal, 1, &readed, NULL);
			if (signal == '0')
				exit(0);
			else {
				unsigned char sizeFileName = 0;
				char fileName[BUFSIZ] = { 0 };
				char dirName[] = "loadFiles/";
				ReadFile(com2, &sizeFileName, 1, &readed, NULL);
				ReadFile(com2, &fileName, sizeFileName, &readed, NULL);
				fileName[readed] = NULL;
				unsigned char tempSize[sizeof(unsigned long)] = { 0 };
				ReadFile(com2, &tempSize, sizeof(unsigned long), &readed, NULL);
				unsigned long size4 = *((unsigned long*)tempSize);
				cout << endl << "Size of file: " << size4 << endl;
				char tempChar = 0;
				ofstream file(strcat(dirName, fileName));
				for (unsigned long i = 0; i < size4; i++) {
					ReadFile(com2, &tempChar, 1, &readed, NULL);
					file << tempChar;
				}
				file.close();
				system("pause");
			}
		}
		CloseHandle(com2);
	}
	case '2':
	{
		char temp, temp2;
		DWORD readed = 0;
		ReadFile(com2, &temp, 1, &readed, NULL);
		for (; temp > 0; temp--) {
			ReadFile(com2, &temp2, 1, &readed, NULL);
			cout << temp2;
		}
		cout << "0 - exit";
		while (true) {
			cout << endl << "Your choice: ";
			char signal;
			cin >> signal;
			WriteFile(com2, &signal, 1, &readed, NULL);
			if (signal == '0') exit(0);
			system("pause");
		}
		CloseHandle(com2);
	}
	}
}