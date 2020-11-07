#ifndef CHECK_FUNCTION_H
#define CHECK_FUNCTION_H

#pragma once

#include <QDate>
#include "abouthuman.h"
#include "ExceptionInput.h"

void _check_names(QString str,  int N);//Функция проверки правильности заполнения полей имен

void _check_date(QString str);//Функция проверики правильности заполнения даты рождения

void _chech_number(QString str);//Функция проверки правильности введения номера мобильного телефона

void _check_adress(QString str,  int N);//Функция проверки правильности заполнения полей адреса

bool _check_nameAdress(QString str);//Функция проверки правильности заполнения полей адреса

bool _check_letters(QString str);//Функция проверики на символ в строке

bool _check_int(QString str);//Функция проверки на число в строке

#endif // CHECK_FUNCTION_H
