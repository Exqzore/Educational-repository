#ifndef FAMILY_H
#define FAMILY_H

#pragma once

#include "abouthuman.h"
#include "date.h"

class Family : public AboutHuman, public Date
{
private:
    char FamilyPosition[25];
public:
    Family();//Конструктор по умолчанию
    //Конструктор с параметрами
    Family(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet,
           QString AdressCity, QString NumberOfPhone, QString Name, QString SecondName,
           QString FatherName, QString FamilyPosition, QString _data);
    void SetFamilyPosition(QString FamilyPosition);//Устанавливаем позицию в семье
    QString GetFamilyPosition();//Получаем позицию в семье
    Family& operator =(const Family& temp);//Оператор присваивания
    bool operator ==(const Family& temp);//Оператор сравнения
    ~Family() override;
};


#endif // FAMILY_H
