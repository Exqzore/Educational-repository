#ifndef ABOUTHUMAN_H
#define ABOUTHUMAN_H

#pragma once

#include "human.h"
#include "Adress.h"

class AboutHuman : public Human
{
private:
    Adress _Adress;
    char Who_is_it[25];
    char path_to_photo[200];
    char NumberOfPhone[25];
    char Operator[25];
public:
    AboutHuman();//Конструктор по умолчанию
    //Конструктор с параметрами
    AboutHuman(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet, QString AdressCity,
               QString NumberOfPhone, QString Name, QString SecondName, QString FatherName);
    void SetAdressFlat(QString AdressFlat);//Устанавливаем номер квартиры
    void SetAdressHome(QString AdressHome);//Устанавливаем номер дома
    void SetAdressStreet(QString AdressStreet);//Устанавливаем название улицы
    void SetAdressCity(QString AdressCity);//Устанавливаем название города
    void SetNumberOfPhone(QString NumberOfPhone);//Устанавливаем номер телефона
    void SetPath_to_photo(QString Path_to_photo);//Устанавливаем путь к фотографии
    void SetWho_is_it(QString Who_is_it);//Устанавливаем тип человека
    QString GetAdressFlat();//Получаем номер квартиры
    QString GetAdressHome();//Получаем номер дома
    QString GetAdressStreet();//Получаем название улицы
    QString GetAdressCity();//Получаем название города
    QString GetNumberOfPhone();//Получаем номер телефона
    QString GetOperator();//Получаем оператор мобильного телефона
    QString GetPath_to_photo();//Получаем путь к фотографии
    QString GetWho_is_it();//Получаем тип человека
    bool _operatorDefinition();//Определяем оператор мобильного телефона
    //Компоратор для сравнения по определенному полю
    static bool _compare(AboutHuman a1, AboutHuman a2, int pole);
    AboutHuman& operator =(const AboutHuman& temp);//Оператор присваивания
    bool operator ==(const AboutHuman& temp);//Оператор сравнения
    ~AboutHuman() override;
};

#endif // ABOUTHUMAN_H
