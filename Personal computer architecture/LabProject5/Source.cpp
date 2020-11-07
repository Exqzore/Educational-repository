#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int old_value_A1, old_value_71, waitingTime, chAlarm = 1;

void interrupt(*old_alarm_handler) (...);

void interrupt(*temp_alarm_handler) (...);

void interrupt new_alarm_handler(...) {
	puts("Time to learn!!!");
	_disable();
	setvect(0x4A, old_alarm_handler);
	_enable();
	outp(0xA1, inp(0xA1) | 0x01);//00000001
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0xDF);//11011111
}

void interrupt(*old_RTC_handler) (...);

void interrupt new_RTC_handler(...) {
	waitingTime--;
    //old_RTC_handler();
	outp(0x70, 0x0C);
	inp(0x71);
	outp(0x20, 0x20);
	outp(0xA0, 0x20);
}

void read_RTC() {
	int s, m, h, dw, dm, mo, y;
	outp(0x70, 0xA);
	while (inp(0x71) & 0x80);//10000000
	outp(0x70, 00);
	s = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 02);
	m = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 04);
	h = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 06);
	dw = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 07);
	dm = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 0x08);
	mo = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	outp(0x70, 0x09);
	y = (inp(0x71) % 0x10) + (inp(0x71) / 0x10) * 10;
	printf("\n\nCurrent date: %02d.%02d.%02d.%02d\n", dw, dm, mo, y);
	printf("Current time: %02d:%02d:%02d\n\n", h, m, s);
}

void write_RTC() {
	int s, m, h;
	printf("Current hours: ");
	scanf("%d", &h);
	printf("Current minutes: ");
	scanf("%d", &m);
	printf("Current seconds: ");
	scanf("%d", &s);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x80);//10000000
	outp(0x70, 00);
	outp(0x71, (s % 10) + (s / 10) * 0x10);
	outp(0x70, 02);
	outp(0x71, (m % 10) + (m / 10) * 0x10);
	outp(0x70, 04);
	outp(0x71, (h % 10) + (h / 10) * 0x10);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0x7F);//01111111
}

void set_Date() {
	int dw, dm, mo, y;
	printf("Current day week: ");
	scanf("%d", &dw);
	printf("Current day month: ");
	scanf("%d", &dm);
	printf("Current month: ");
	scanf("%d", &mo);
	printf("Current year: ");
	scanf("%d", &y);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x80);//10000000
	outp(0x70, 06);
	outp(0x71, (dw % 10) + (dw / 10) * 0x10);
	outp(0x70, 07);
	outp(0x71, (dm % 10) + (dm / 10) * 0x10);
	outp(0x70, 0x08);
	outp(0x71, (mo % 10) + (mo / 10) * 0x10);
	outp(0x70, 0x09);
	outp(0x71, (y % 10) + (y / 10) * 0x10);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0x7F);//01111111
}

void delay_Time() {
	printf("Input delay time: ");
	scanf("%d", &waitingTime);
	_disable();
	old_RTC_handler = getvect(0x70);
	setvect(0x70, new_RTC_handler);
	_enable();
	read_RTC();
	outp(0xA1, inp(0xA1) & 0xFE);//11111110
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x40);//01000000
	while (waitingTime > 0);
	read_RTC();
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0xBF);//10111111
	outp(0xA1, inp(0xA1) | 0x01);//00000001
	_disable();
	setvect(0x70, old_RTC_handler);
	_enable();
}

void set_Alarm() {
	int h, m, s;
	printf("Input alarm hours: ");
	scanf("%d", &h);
	printf("Input alarm minutes: ");
	scanf("%d", &m);
	printf("Input alarm seconds: ");
	scanf("%d", &s);
	outp(0x70, 0xA);
	while (inp(0x71) & 0x80);
	outp(0x70, 01);
	outp(0x71, (s % 10) + (s / 10) * 0x10);
	outp(0x70, 03);
	outp(0x71, (m % 10) + (m / 10) * 0x10);
	outp(0x70, 05);
	outp(0x71, (h % 10) + (h / 10) * 0x10);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x20);//11011111
	_disable();
	old_alarm_handler = getvect(0x4A);
	setvect(0x4A, new_alarm_handler);
	outp(0xA1, inp(0xA1) & 0xFE);//11111110
	_enable();
}

void main() {
	do {
		clrscr();
		printf("\n\n1. Read RTC time.\n2. Write RTC time.\n3. Set date.\n4. Delay time.\n5. Set alarm.\n0. Exit.\n");
		fflush(stdin);
		char choice = getch();
		switch (choice) {
		case '1': read_RTC(); system("pause"); break;
		case '2': write_RTC(); system("pause"); break;
		case '3': set_Date(); system("pause"); break;
		case '4': delay_Time(); system("pause"); break;
		case '5': set_Alarm(); system("pause"); break;
		case '0': return;
		}
	} while (1);
}