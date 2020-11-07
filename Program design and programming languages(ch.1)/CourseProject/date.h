#ifndef DATE_H
#define DATE_H

#pragma once

#include <QDate>
#include <QString>

class Date
{
private:
    char _data[11];
public:
    Date();//Конструктор по умолчанию
    Date(QString _data);//Конструктор с параметрами
    void Set_Data(QString _data);//Устанавливаем дату рождения
    QString Get_Data_s();//Подучаем дату в виде ДД/ММ/ГГГГ
    QString Get_Data();//Получаем дату в виде строки
    qint64 DeterminationOfDaysBirthday();//Определение количества дней до дня рождения
    Date& operator =(const Date& temp);//Оператор присваивания
    bool operator ==(const Date& temp);//Оператор сравнения
    ~Date();
};

#endif // DATE_H
