#include <dos.h>

#define BUFF_WIDTH 80
#define CENTER_OFFSET 12
#define LEFT_OFFSET 25
#define REG_SCREEN_SIZE 9

struct VIDEO
{
    unsigned char symb;
    unsigned char attr;
};

int attribute = 0x6e; //color

void print(int offset, int value);
void getRegisterValue();

void interrupt(*oldHandle08) (...);
void interrupt(*oldHandle09) (...);
void interrupt(*oldHandle0A) (...);
void interrupt(*oldHandle0B) (...);
void interrupt(*oldHandle0C) (...);
void interrupt(*oldHandle0D) (...);
void interrupt(*oldHandle0E) (...);
void interrupt(*oldHandle0F) (...);

void interrupt(*oldHandle90) (...);
void interrupt(*oldHandle91) (...);
void interrupt(*oldHandle92) (...);
void interrupt(*oldHandle93) (...);
void interrupt(*oldHandle94) (...);
void interrupt(*oldHandle95) (...);
void interrupt(*oldHandle96) (...);
void interrupt(*oldHandle97) (...);

//Пользовательские обработчики ведущего контроллера
void interrupt newHandle08(...) { 
    getRegisterValue(); 
    oldHandle08(); 
}
void interrupt newHandle09(...) { 
    attribute++; 
    getRegisterValue(); 
    oldHandle09(); 
}
void interrupt newHandle0A(...) { 
    getRegisterValue(); 
    oldHandle0A(); 
}
void interrupt newHandle0B(...) { 
    getRegisterValue(); 
    oldHandle0B(); 
}
void interrupt newHandle0C(...) { 
    getRegisterValue(); 
    oldHandle0C(); 
}
void interrupt newHandle0D(...) { 
    getRegisterValue(); 
    oldHandle0D(); 
}
void interrupt newHandle0E(...) { 
    getRegisterValue(); 
    oldHandle0E(); 
}
void interrupt newHandle0F(...) { 
    getRegisterValue(); 
    oldHandle0F(); 
}

//Пользовательские обработчики ведомого контроллера
void interrupt newHandle90(...) { 
    getRegisterValue(); 
    oldHandle90(); 
}
void interrupt newHandle91(...) { 
    getRegisterValue(); 
    oldHandle91(); 
}
void interrupt newHandle92(...) { 
    getRegisterValue(); 
    oldHandle92(); 
}
void interrupt newHandle93(...) { 
    getRegisterValue(); 
    oldHandle93(); 
}
void interrupt newHandle94(...) { 
    getRegisterValue(); 
    oldHandle94(); 
}
void interrupt newHandle95(...) { 
    getRegisterValue(); 
    oldHandle95(); 
}
void interrupt newHandle96(...) { 
    getRegisterValue(); 
    oldHandle96(); 
}
void interrupt newHandle97(...) { 
    getRegisterValue(); 
    oldHandle97(); 
}

void print(int offset, int value)
{
    char temp;

    VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);
    screen += CENTER_OFFSET * BUFF_WIDTH + offset;

    for (int i = 7; i >= 0; i--) {
        temp = value % 2;
        value /= 2;
        screen->symb = temp + '0';
        screen->attr = attribute;
        screen++;
    }
}

void getRegisterValue()
{
    print(0 + LEFT_OFFSET, inp(0x21));

    outp(0x20, 0x0B);
    print(REG_SCREEN_SIZE + LEFT_OFFSET, inp(0x20));

    outp(0x20, 0x0A);
    print(REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0x20));

    print(BUFF_WIDTH + LEFT_OFFSET, inp(0xA1));

    outp(0xA0, 0x0B);
    print(BUFF_WIDTH + REG_SCREEN_SIZE + LEFT_OFFSET, inp(0xA0));

    outp(0xA0, 0x0A);
    print(BUFF_WIDTH + REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0xA0));
}

void initialization()
{      
    // Сохраняем старые значения векторов прерываний
    // IRQ 0-7
    oldHandle08 = getvect(0x08); // Timer
    oldHandle09 = getvect(0x09); // Keyboard
    oldHandle0A = getvect(0x0A); // Slave IRQ
    oldHandle0B = getvect(0x0B); // Random deviece
    oldHandle0C = getvect(0x0C); // Random deviece
    oldHandle0D = getvect(0x0D); // Random deviece
    oldHandle0E = getvect(0x0E); // Random deviece
    oldHandle0F = getvect(0x0F); // Random deviece

    // IRQ 8-15
    oldHandle90 = getvect(0x70); // Real time clock
    oldHandle91 = getvect(0x71); // Random deviece
    oldHandle92 = getvect(0x72); // Random deviece
    oldHandle93 = getvect(0x73); // Random deviece or timer
    oldHandle94 = getvect(0x74); // PS/2 mouse
    oldHandle95 = getvect(0x75); // FPU error
    oldHandle96 = getvect(0x76); // Random deviece or first ATA controller
    oldHandle97 = getvect(0x77); // Random deviece or second ATA controller

    // Устанавливаем новые значения векторов прерываний
    setvect(0x08, newHandle08);
    setvect(0x09, newHandle09);
    setvect(0x0A, newHandle0A);
    setvect(0x0B, newHandle0B);
    setvect(0x0C, newHandle0C);
    setvect(0x0D, newHandle0D);
    setvect(0x0E, newHandle0E);
    setvect(0x0F, newHandle0F);

    setvect(0x90, newHandle90);
    setvect(0x91, newHandle91);
    setvect(0x92, newHandle92);
    setvect(0x93, newHandle93);
    setvect(0x94, newHandle94);
    setvect(0x95, newHandle95);
    setvect(0x96, newHandle96);
    setvect(0x97, newHandle97);
   
    // Запрещаем прерывания (cli)
    _disable();

    // Отправляем значение в порт
    // Ведущий контроллер
    outp(0x20, 0x11); // ICW1
    outp(0x21, 0x08); // ICW2
    outp(0x21, 0x04); // ICW3
    outp(0x21, 0x01); // ICW4

    // Ведомый контроллер
    outp(0xA0, 0x11); // ICW1
    outp(0xA1, 0x90); // ICW2
    outp(0xA1, 0x02); // ICW3
    outp(0xA1, 0x01); // ICW4

    // Разрешаем прерывания (sti)
    _enable();
}

int main()
{
    unsigned far* fp;

    initialization();

    FP_SEG(fp) = _psp;
    FP_OFF(fp) = 0x2c;
    _dos_freemem(*fp);

    _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
    return 0;
}