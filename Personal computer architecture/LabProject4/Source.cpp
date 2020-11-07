#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#define COUNT 8
#define DELAY 10
#define TIME_RUN 65536

void interrupt(*oldvect)(...);

void soundGeneration();
void divisionCoefficient();
void statusWord();
void randomNumber();
void tempfunc();

int fftime = 0;
int count = 1;

void interrupt reboot(...) {
	if (count >= 18 * fftime) {
		int kd = 1193180 / 784;
		outp(0x43, 0xB6);
		outp(0x42, kd % 256);
		outp(0x42, kd /= 256);
		outp(0x61, inp(0x61) | 3);
		delay(400);
		outp(0x61, inp(0x61) & 0xFC);
		setvect(0x08, oldvect);
	}
	count++;
	oldvect();
}

void main() {
	int choice;
	do {
		clrscr();
		printf("\n\n1. Sound generation.\n2. Coefficient of division.\n3. Status word.\n4. Random number.\n5. Temp.\n0. Exit.\n");
		printf("\nYour choice: ");
		fflush(stdin);
		scanf("%d", &choice);
		switch (choice) {
		case 1: soundGeneration(); system("pause");  break;
		case 2: divisionCoefficient(); system("pause"); break;
		case 3: statusWord(); system("pause"); break;
		case 4: randomNumber(); system("pause"); break;
		case 5: tempfunc();
		}
	} while(choice != 0);
}

void tempfunc() {
	unsigned far* fp;
	printf("\nYour time (s): ");
	scanf("%d", &fftime);

	oldvect = getvect(0x08);
	setvect(0x08, reboot);

	outp(0x43, 0x36);
	outp(0x40, 0xff);
	outp(0x40, 0xff);
	
	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
}

void soundGeneration() {
	int frequency[COUNT] = { 784, 392, 440, 440, 784, 440, 493, 493 };
	int duration[COUNT] = { 200, 200, 200, 200, 200, 200, 200, 200 };
	long unsigned timerFrequency = 1193180;

	for (int i = 0, kd = 0; i < COUNT; i++) {					
		outp(0x43, 0xB6);							// 10110110
		//  [0] - 2ss, [1-3] - режим с прямоугольным импульсом, [4-5] - чтение/запись младшего, затем старшего байта, [7-6]- выбор 3 канала
		kd = timerFrequency / frequency[i];
		outp(0x42, kd % 256);
		outp(0x42, kd /= 256);
		outp(0x61, inp(0x61) | 3);
		delay(duration[i]);
		outp(0x61, inp(0x61) & 0xFC);
	}
}

void divisionCoefficient() {
	int ports[3] = { 0x40, 0x41, 0x42 };
	int controlBytes[3] = { 0, 64, 128 };			// для выбора канала  xx000000
	unsigned int iValue, iHigh, iLow, temp;

	printf("\n\nCoefficient of division: \n");

	for (int i = 0; i < 3; i++) {
		if (i == 2)	outp(0x61, inp(0x61) | 1);		//  Динамик - ON
		iValue = 0;
		for (int j = 0; j < 30000; j++) {
			outp(0x43, controlBytes[i]);
			iLow = inp(ports[i]);					// считываем младшее слово
			iHigh = inp(ports[i]);					// считываем старшее слово
			temp = iHigh * 256 + iLow;				// записываем все знаение 
			if (temp > iValue) iValue = temp;		// если оно максимальное, запоминаем	
		}
		if (i == 2) outp(0x61, inp(0x61) & 0xFC);	//  Динамик - OFF
		printf("\nChannel %4d: %4X\n", i, iValue);
	}
}

void statusWord() {
	unsigned char controlBytes[3] = { 0xE2, 0xE4, 0xE8 };	// 11100010, 11100100, 11101000
	//[0] - всегда 0, [1-3] - выбор канала(нужный в 1), [4] - 0 позволяет получить состояние канала, [5] - 0 фиксирует значение счетчика, [7-6] - считывание значений из регистров каналов
	unsigned char state;
	unsigned char ports[3] = { 0x40, 0x41, 0x42 };

	for (int i = 0; i < 3; i++) {
		outp(0x43, controlBytes[i]);
		state = inp(ports[i]);
		printf("Channel %d: ", i);
		for (int j = 7; j >= 0; j--)
			printf("%d", (state >> j) & 1);
		printf("\n");
	}
}

void randomNumber() {
	int choice;
	unsigned int limit = TIME_RUN - 1, numberLow, numberHigh, number;

	do {
		printf("\n\n1. Set a limit.\n2. Get a number.\n0. Exit\n");
		printf("\nYour choise: ");
		fflush(stdin);
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			do {
				printf("\nEnter a limit in range [1...65535].\n");
				fflush(stdin);
			} while (!scanf("%d", &limit) || limit < 1);
			outp(0x43, 0xB4);						//   43 - регистр команд,   В4 - 10110100
			outp(0x42, limit % 256);
			outp(0x42, limit /= 256);
			outp(0x61, inp(0x61) | 1);
			break;
		case 2:
			outp(0x43, 128);
			numberLow = inp(0x42);
			numberHigh = inp(0x42);
			number = numberHigh * 256 + numberLow;
			printf("\nRandom number: %u\n", number);
			break;
		}
	} while (choice != 0);
	outp(0x61, inp(0x61) & 0xFC);
}