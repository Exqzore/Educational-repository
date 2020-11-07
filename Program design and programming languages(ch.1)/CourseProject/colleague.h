#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#pragma once

#include "abouthuman.h"
#include "date.h"

class Colleague : public AboutHuman, public Date
{
private:
    char Post[25];
public:
    Colleague();//Конструктор по умолчанию
    Colleague(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet,
              QString AdressCity, QString NumberOfPhone, QString Name, QString SecondName,
              QString FatherName, QString Post, QString _data);//Конструктор с параметрами
    void SetPost(QString Post);//Устанавливаем пост
    QString GetPost();//получаем пост
    Colleague& operator =(const Colleague& temp);//Оператор присваивания
    bool operator ==(const Colleague& temp);//Оператор сравнения
    ~Colleague() override;
};

#endif // COLLEAGUE_H
