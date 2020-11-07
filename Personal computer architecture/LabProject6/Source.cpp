#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

char buffer[80] = { 0 }, scanCodes[128] = { 0 };
int bufLength = 0, chDelay = 0, chSpeed = 30;

void f() {
	scanCodes[16] = 'q';
	scanCodes[17] = 'w';
	scanCodes[18] = 'e';
	scanCodes[19] = 'r';
	scanCodes[20] = 't';
	scanCodes[21] = 'y';
	scanCodes[22] = 'u';
	scanCodes[23] = 'i';
	scanCodes[24] = 'o';
	scanCodes[25] = 'p';
	scanCodes[30] = 'a';
	scanCodes[31] = 's';
	scanCodes[32] = 'd';
	scanCodes[33] = 'f';
	scanCodes[34] = 'g';
	scanCodes[35] = 'h';
	scanCodes[36] = 'j';
	scanCodes[37] = 'k';
	scanCodes[38] = 'l';
	scanCodes[44] = 'z';
	scanCodes[45] = 'x';
	scanCodes[46] = 'c';
	scanCodes[47] = 'v';
	scanCodes[48] = 'b';
	scanCodes[49] = 'n';
	scanCodes[50] = 'm';
	scanCodes[2] = '1';
	scanCodes[3] = '2';
	scanCodes[4] = '3';
	scanCodes[5] = '4';
	scanCodes[6] = '5';
	scanCodes[7] = '6';
	scanCodes[8] = '7';
	scanCodes[9] = '8';
	scanCodes[10] = '9';
	scanCodes[11] = '0';
}

void interrupt(*old_ch_int)(...);

void interrupt (new_ch_int)(...) {
	old_ch_int();
	unsigned char scanCode = inp(0x60);
	if (scanCode < 128) {
		buffer[bufLength++] = scanCodes[scanCode];
		int tempLength = bufLength;
		if (!(tempLength <= 7)) tempLength = 7;
		if (!strncmp("chdelay", buffer, tempLength) || !strncmp("chspeed", buffer, tempLength)) {
			if (bufLength == 8 && !strncmp("chdelay", buffer, 7)) {
				if (buffer[7] - '0' >= 0 && buffer[7] - '0' <= 3) {
					while (inp(0x64) & 2);
					do {
						outp(0x60, 0xF3);
					} while (inp(0x60) == 0xFE);
					do {
						outp(0x60, (buffer[7] - '0') * 0x20 + chSpeed);
					} while (inp(0x60) == 0xFE);
					chDelay = buffer[7] - '0';
				}
				bufLength = 0;
			}
			else if (bufLength == 9 && !strncmp("chspeed", buffer, 7)) {
				if ((buffer[7] - '0') * 10 + (buffer[8] - '0') >= 0 && (buffer[7] - '0') * 10 + (buffer[8] - '0') <= 31) {
					while (inp(0x64) & 2);
					do {
						outp(0x60, 0xF3);
					} while (inp(0x60) == 0xFE);
					do {
						outp(0x60, chDelay * 0x20 + (buffer[7] - '0') * 10 + (buffer[8] - '0'));
					} while (inp(0x60) == 0xFE);
					chSpeed = (buffer[7] - '0') * 10 + (buffer[8] - '0');
				}
				bufLength = 0;
			}
		}
		else {
			bufLength = 0;
			return;
		}
	}
}

void main() {
	f();
	unsigned far* fp;
	_disable();
	old_ch_int = getvect(0x09);
	setvect(0x09, new_ch_int);
	_enable();
    FP_SEG(fp) = _psp;
    FP_OFF(fp) = 0x2c;
    _dos_freemem(*fp);
    _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
	return;
}