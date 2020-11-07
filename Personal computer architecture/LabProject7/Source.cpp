#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <fstream.h>

#define COM1 0x3F8

union REGS reg;

void interrupt newInterrupt(...);
void interrupt(*oldInterrupt)(...);

void initComPort() {
	outp(COM1 + 3, 0x80);    
	outp(COM1 + 0, 0x0C);    
	outp(COM1 + 1, 0x00);    
	outp(COM1 + 3, 0x03);   
	outp(COM1 + 2, 0x07);   
	outp(COM1 + 4, 0x0B);    
	outp(COM1 + 1, 0x01);    
}

void writeToComPort(char* str) {
	char size = strlen(str);
	while (!(inp(COM1 + 5) & 0x20));
	outp(COM1 + 0, size);
	for (int i = 0; i < strlen(str); i++) {
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, str[i]);
	}
}

char readComPort() {
	while ((inp(COM1 + 5) & 1) == 0);
	return inp(COM1 + 0);
}

void loadfile(const char* str) {
	writeToComPort((char*)str);
	unsigned long filesize = 0;
	char tmp;
	ifstream file(str);
	while (file.get(tmp)) filesize++;
	file.close();
	unsigned char* tempPtr = (unsigned char*)&filesize;
	for (unsigned long i = 0; i < sizeof(unsigned long); i++) {
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, *(tempPtr + i));
	}
	cout << endl << "Size of file: " << filesize << endl;
	ifstream file1(str);
	for (unsigned long j = 0; j < filesize; j++) {
		char c;
		file1.get(c);
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, c);
	}
	file1.close();
}

int main() {
	_disable();
	initComPort();
	_enable();
	int choice;
	cout << "1 - uploads files\n2 - launch programs\nYour choice: ";
	cin >> choice;
	switch (choice) {
	case 1:
	{
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, '1');
		char tempNumber,
			list[] = "1 - cg.txt\n"
			"2 - name.txt\n"
			"3 - random.txt\n"
			"4 - IP.txt\n";
		do {
			writeToComPort(list);
			delay(1000);
			tempNumber = readComPort();
			if (tempNumber > '0') cout << "\nFile is uploading...\n";
			else {
				cout << "\nProgram completion\n";
				system("pause");
				return 0;
			}
			switch (tempNumber) {
			case '1':
				cout << "\nUpload file CG.TXT\n";
				loadfile("folder.png");
				break;
			case '2':
				cout << "\nUpload file NAME.TXT\n";
				loadfile("name.txt");
				break;
			case '3':
				cout << "\nUpload file RANDOM.TXT\n";
				loadfile("random.txt");
				break;
			case '4':
				cout << "\nUpload file IP.TXT\n";
				loadfile("IP.txt");
				break;
			}
		} while (1);
	}
	case 2:
	{
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, '2');
		_disable();
		outp(0x21, 0);
		oldInterrupt = getvect(0x0C);
		setvect(0x0C, newInterrupt);
		_enable();
		char tempNumber1,
			list1[] = "1 - program1.exe\n"
			"2 - program2.exe\n"
			"3 - program3.exe\n"
			"4 - program4.exe\n"
			"5 - program5.exe\n";
		writeToComPort(list1);
		delay(1000);
		_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
		return 0;
	}
	}
	return 0;
}

void f(char asciiCode, char scanCode) {
	reg.h.ah = 0x05;
	reg.h.cl = asciiCode;
	reg.h.ch = scanCode;
	int86(0x16, &reg, &reg);
}

void interrupt newInterrupt(...)
{
	char temp = readComPort();
	switch (temp)
	{
	case '1':
	{
		char asciiCodes[13] = { 0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x31, 0x2E, 0x45, 0x58, 0x45, 0x0D };
		char scanCodes[13] = { 0x19, 0x13, 0x18, 0x22, 0x13, 0x1E, 0x32, 0x02, 0x34, 0x12, 0x2D, 0x12, 0x1C };
		for (int i = 0; i < 13; i++) f(asciiCodes[i], scanCodes[i]);
		delay(100);
		break;
	}
	case '2':
	{
		char asciiCodes[13] = { 0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x32, 0x2E, 0x45, 0x58, 0x45, 0x0D };
		char scanCodes[13] = { 0x19, 0x13, 0x18, 0x22, 0x13, 0x1E, 0x32, 0x03, 0x34, 0x12, 0x2D, 0x12, 0x1C };
		for (int i = 0; i < 13; i++) f(asciiCodes[i], scanCodes[i]);
		delay(100);
		break;
	}
	case '3':
	{
		char asciiCodes[13] = { 0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x33, 0x2E, 0x45, 0x58, 0x45, 0x0D };
		char scanCodes[13] = { 0x19, 0x13, 0x18, 0x22, 0x13, 0x1E, 0x32, 0x04, 0x34, 0x12, 0x2D, 0x12, 0x1C };
		for (int i = 0; i < 13; i++) f(asciiCodes[i], scanCodes[i]);
		delay(100);
		break;
	}
	case '4':
	{
		char asciiCodes[13] = { 0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x34, 0x2E, 0x45, 0x58, 0x45, 0x0D };
		char scanCodes[13] = { 0x19, 0x13, 0x18, 0x22, 0x13, 0x1E, 0x32, 0x05, 0x34, 0x12, 0x2D, 0x12, 0x1C };
		for (int i = 0; i < 13; i++) f(asciiCodes[i], scanCodes[i]);
		delay(100);
		break;
	}
	case '5':
	{
		char asciiCodes[13] = { 0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x35, 0x2E, 0x45, 0x58, 0x45, 0x0D };
		char scanCodes[13] = { 0x19, 0x13, 0x18, 0x22, 0x13, 0x1E, 0x32, 0x06, 0x34, 0x12, 0x2D, 0x12, 0x1C };
		for (int i = 0; i < 13; i++) f(asciiCodes[i], scanCodes[i]);
		delay(100);
		break;
	}
	case '0':
	{
		outp(0x20, 0x20);
		exit(0);
	}
	}
	outp(0x20, 0x20);
}