#ifndef EXCEPTIONINPUT_H
#define EXCEPTIONINPUT_H

#pragma once

#include "Exception.h"

class ExceptionInput : public Exception
{
private:
    QString dataState;
public:
    ExceptionInput(): //Конструктор по умолчанию
        Exception()
    {
        this->dataState = "";
    }

    ExceptionInput(QString str, QString code): //Конструктор с параметрами
        Exception(str)
    {
        this->dataState = code;
    }

    ExceptionInput(QString str): //Конструктор с параметрами
        Exception(str)
    {
        this->dataState = "";
    }

    QString Code()//Возвращает код ошибки
    {
        return this->dataState;
    }
};

#endif // EXCEPTIONINPUT_H
