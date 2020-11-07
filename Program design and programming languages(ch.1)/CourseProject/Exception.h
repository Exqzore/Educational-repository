#ifndef EXCEPTION_H
#define EXCEPTION_H

#pragma once

#include <QString>

class Exception
{
protected:
    QString error;
public:
    Exception()//Конструктор по умолчанию
    {
        this->error = "";
    }

    Exception(QString str)//Конструктор с параметрами
    {
        this->error = str;
    }

    QString what()//Возвращает описание ошибки
    {
        return this->error;
    }
};

#endif // EXCEPTION_H
