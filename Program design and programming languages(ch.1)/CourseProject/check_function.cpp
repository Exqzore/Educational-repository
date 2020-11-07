#include "check_function.h"

void _check_names(QString str,  int N)
{
    if(str == "")
        throw ExceptionInput("Область имен не может быть пустой", "FE0001");
    else if(str.size() > N)
        throw ExceptionInput("Область имен не может содежать больше" + QString::number(N) + "символов", "FE0005");
    else if(_check_letters(str))
        throw ExceptionInput("В поле имен могут быть записаны только \nбуквы латинского и русского алфавита", "FE0011");
}

void _check_date(QString str)
{
    if(str == "")
        throw ExceptionInput("Область даты дня рождения\nне может быть пустой", "FE0001");
    int count = 0;
    for(int i = 0; i<str.size(); i++)
        if(str[i] == '/')
            count++;
    if(str.mid(2,1)!='/' || str.mid(5,1)!='/' || str.size() != 10 || count > 2)
        throw ExceptionInput("Область даты дня рождения\nдожна быть введена в виде DD/MM/YYYY", "FE0225");
    else if(_check_int(str.mid(0,2)) || _check_int(str.mid(3,2)) || _check_int(str.mid(6,4)))
        throw ExceptionInput("В область даты дня рождения\nдожны быть введены только цифры и символы '/'", "FE0012");
    else if(str.mid(3,2).toInt() < 1 || str.mid(3,2).toInt() > 12 || str.mid(6,4).toInt() > QDate::currentDate().year())
        throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
    else if(str.mid(6,4).toInt() == QDate::currentDate().year())
    {
        if(str.mid(3,2).toInt() > QDate::currentDate().month())
            throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
        else if(str.mid(3,2).toInt() == QDate::currentDate().month())
            if(str.mid(0,2).toInt() > QDate::currentDate().day())
                throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
    }
    if(str.mid(3,2).toInt()%2 == 0)
    {
        if(str.mid(0,2).toInt() > 31)
            throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
        if(str.mid(3,2).toInt() == 2 && str.mid(6,4).toInt()%4==0)
        {
            if(str.mid(0,2).toInt() > 29)
                throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
        }
        else if(str.mid(3,2).toInt() == 2 && str.mid(6,4).toInt()%4!=0)
            if(str.mid(0,2).toInt() > 28)
                throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
    }
    else if(str.mid(3,2).toInt()%2 != 0)
    {
        if(str.mid(0,2).toInt() > 30)
            throw ExceptionInput("В область даты дня рождения\nбыла введена несуществующая дата", "FE4001");
    }
}

void _chech_number(QString str)
{
    if(str == "")
        throw ExceptionInput("Область номера телефона не может быть пустой", "FE0001");
    else if(str.mid(0,5) != "+375(" || str[7] != ')')
        throw ExceptionInput("Область номера телефона дожна\nбыть введена в виде +375(XX)XXXXXXX", "FE0001");
    AboutHuman temp;
    temp.SetNumberOfPhone(str);
    if(!temp._operatorDefinition())
        throw ExceptionInput("В область номера телефона введен несуществующий телефон", "FE5101");
}

void _check_adress(QString str, int N)
{
    if(str == "")
        throw ExceptionInput("Область названия улицы не может быть пустой", "FE0001");
    else if(str.size() > N)
        throw ExceptionInput("Область названия улицы не может содежать больше" + QString::number(N) + "символов", "FE0005");
    else if(_check_nameAdress(str))
        throw ExceptionInput("В поле названия улицы могут быть записаны только \nбуквы латинского и русского алфавита\nи символы '.', '/'", "FE0011");
}

bool _check_nameAdress(QString str)
{
    for (int i = 0; i < str.size(); i++)
        if (!(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= 'a' && str[i] <= 'z')
                && !(str[i] >= "А" && str[i] <= "Я") && !(str[i] >= "а" && str[i] <= "я")
                && str[i] != '/' && str[i] != '.' && !(str[i] >= '0' && str[i] <= '9') && str[i]!= " " && str[i]!= '\n')
            return true;
    return false;
}

bool _check_int(QString str)
{
    for (int i = 0; i < str.size(); i++)
        if (!(str[i] >= '0' && str[i] <= '9'))
            return true;
    return false;
}

bool _check_letters(QString str)
{
    for (int i = 0; i < str.size(); i++)
        if (!(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= 'a' && str[i] <= 'z')
                && !(str[i] >= "А" && str[i] <= "Я") && !(str[i] >= "а" && str[i] <= "я") && str[i]!= " " && str[i]!= '\n')
            return true;
    return false;
}
