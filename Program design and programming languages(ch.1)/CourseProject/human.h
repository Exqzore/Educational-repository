#ifndef HUMAN_H
#define HUMAN_H

#pragma once

#include <QString>

class Human
{
private:
    char Name[25];
    char SecondName[25];
    char FatherName[25];
public:
    Human();//Конструктор по умолчанию
    //Конструктор с параметрами
    Human(QString Name, QString SecondName, QString FatherName);
    void SetName(QString Name);//Устанавливаем имя
    void SetSecondName(QString SecondName);//Устанавливаем фамилию
    void SetFatherName(QString FatherName);//Устанавливаем отчество
    QString GetName();//Получаем имя
    QString GetSecondName();//Получаем фамилию
    QString GetFatherName();//Получаем отчество
    Human& operator =(const Human& temp);//Оператор присваивания
    bool operator ==(const Human& temp);//Оператор сравнения
    virtual ~Human();
};

#endif // HUMAN_H
