#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

#define COM1 0x3F8

void initComPort() {
	outp(COM1 + 1, 0x00);    // Disable all interrupts
	outp(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outp(COM1 + 0, 0x0C);    // Set divisor to 3 (lo byte) 38400 baud
	outp(COM1 + 1, 0x00);    // (hi byte)
	outp(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outp(COM1 + 2, 0x87);    // Enable FIFO, clear them, with 14-byte threshold
	outp(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

//void interrupt NewInterrupt(...);
//void interrupt(*oldInterrupt)(...);
//
//void interrupt(*oldINT)(...) = getvect(0x14);
//
//void interrupt(newINT)(...) {
//	cout << (char)readCom1() << "ASDASDAS\n";
//	oldINT();
//}

void writeToComPort(char* str) {	
	for (int i = 0; i < strlen(str); i++) {
		while (!(inp(COM1 + 5) & 0x20));
		outp(COM1 + 0, str[i]);
	}
}

char readComPort() {
	while (inp(COM1 + 5) & 1 == 0);
	return inp(COM1 + 0);
}

int main() {
	initComPort();
	char tempNumber,
		list[] = "1 - program1.exe\n"
		"2 - program2.exe\n"
		"3 - program3.exe\n"
		"4 - program4.exe\n"
		"5 - program5.exe\n";
	//_disable();
	//setvect(0x14, newINT);
	//_enable();

	
	do {
		writeToComPort(list);
		//delay(500);
		tempNumber = readComPort();
		if (tempNumber > '0') cout << "\nStart program №" << tempNumber << "\n";
		else {
			cout << "\nProgram completion\n";
			break;
		}
		switch (tempNumber) {
		case '1': 
			system("program1.exe");
			break;
		case '2':
			system("program2.exe");
			break;
		case '3':
			system("program3.exe");
			break;
		case '4':
			system("program4.exe");
			break;
		case '5':
			system("program5.exe");
			break;
		}
	} while (true);

	system("pause");

	//writeCom1('1');
	//writeCom1(' ');
	//writeCom1('-');
	//writeCom1(' ');
	//writeCom1('A');
	//writeCom1('C');
	//writeCom1('E');
	//writeCom1('\n');
	//writeCom1('2');
	//writeCom1(' ');
	//writeCom1('-');
	//writeCom1(' ');
	//writeCom1('m');
	//writeCom1('p');
	//writeCom1('3');
	//delay(1000);


	//temp = readCom1();
	//cout << temp << endl;

	//_disable();
	//oldInterrupt = getvect(0x09);
	//setvect(0x09, NewInterrupt);
	//_enable();

	//delay(10000);
	//_asm {

	//}
	//delay(10000);
	//_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

	return 0;
}

//int value;
//char alt_flag;
//union REGS reg;
//
//void call_int16(char asciiCodesArr, char scanCodesArr) {
//	reg.h.ah = 0x05;
//	reg.h.cl = asciiCodesArr;
//	reg.h.ch = scanCodesArr;
//	int86(0x16, &reg, &reg);
//}
//
//void interrupt NewInterrupt(...)
//{
//	if (inp(0x64) & 1)                    // проверка наличия данных в выходном буфере(OBS)
//	{
//		//cout << "121" << endl;
//		if (inp(0x60) == 0x38)            // Если нажат alt
//			alt_flag = ALT_TRUE;          // устанавливаем флаг в единицу
//		else
//		{
//			//cout << "122" << endl;
//			if (alt_flag == ALT_TRUE)
//			{
//
//				value = inp(0x60);            // получаем код нажатой клавиши
//				if (value == 0x1E) //A
//				{
//					cout << temp << endl;
//					switch (temp)
//					{
//					case '1': // A
//					{
//						//system("lab4.exe");
//						//cout << "2" << endl;
//						//char asciiCodesArr[7] = { 0x41, 0x3A, 0x0D, 0x41, 0x43, 0x45, 0x20 }; // A: enter ACE  
//					 //   char scanCodesArr[7] = { 0x1E, 0x27, 0x1C, 0x1E, 0x2E, 0x12, 0x39 };
//
//						//for (int i = 0; i < 7; i++)
//						//{
//						//	call_int16(asciiCodesArr[i], scanCodesArr[i]);
//						//}
//						delay(100);
//						break;
//					}
//					case '2': // S
//					{
//						cout << "3" << endl;
//						//char asciiCodesArr2[9] = { 0x43, 0x3A, 0x5C, 0x54, 0x2E ,0x54, 0x58 ,0x54 , 0x0D }; // C:\t.txt enter
//						//char scanCodesArr2[9] = { 0x2E, 0x27, 0x2B, 0x14, 0x34, 0x14, 0x2D, 0x14, 0x1C };
//
//						//for (int i = 0; i < 9; i++)
//						//{
//						//	call_int16(asciiCodesArr2[i], scanCodesArr2[i]);
//						//}
//						//delay(100);
//						break;
//					}
//					default: {break; }
//					}
//				}
//			}
//			alt_flag = 0;
//		}
//	}
//	oldInterrupt();
//}