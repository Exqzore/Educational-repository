#ifndef FRIEND_H
#define FRIEND_H

#pragma once

#include "abouthuman.h"
#include "date.h"

class Friend : public AboutHuman, public Date
{
private:
    bool SchoolFriend;
    bool ChildhoodFriend;
    bool UniversityFriend;
    bool WorkFriend;
public:
    Friend();//Конструктор по умолчанию
    Friend(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet,
           QString AdressCity, QString NumberOfPhone, QString Name, QString SecondName,
           QString FatherName, int ch, QString _data);//Конструктор с параметрами
    void SetTypeOfFriend(int ch);//Устанавливаем тип друга
    QString GetTyoeOfFriend();//получаем тип друга
    Friend& operator =(const Friend& temp);//Оператор присваивания
    bool operator ==(const Friend& temp);//Оператор сравнения
    ~Friend() override;
};

#endif // FRIEND_H
