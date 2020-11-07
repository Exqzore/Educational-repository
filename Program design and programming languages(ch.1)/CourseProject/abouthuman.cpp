#include "abouthuman.h"

AboutHuman::AboutHuman() :
    Human()
{
    strcpy_s(this->Who_is_it, "");
    strcpy_s(this->_Adress.AdressFlat, "");
    strcpy_s(this->_Adress.AdressHome, "");
    strcpy_s(this->_Adress.AdressStreet, "");
    strcpy_s(this->_Adress.AdressCity, "");
    strcpy_s(this->NumberOfPhone, "");
    strcpy_s(this->Operator, "");
    strcpy_s(this->path_to_photo, "");
    strcpy_s(this->Who_is_it, "");
}

AboutHuman::AboutHuman(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet, QString AdressCity,
                       QString NumberOfPhone, QString Name, QString SecondName, QString FatherName) :
    Human(Name, SecondName, FatherName)
{
    strcpy_s(this->Who_is_it, Who_is_it.toLocal8Bit());
    strcpy_s(this->_Adress.AdressFlat, AdressFlat.toLocal8Bit());
    strcpy_s(this->_Adress.AdressHome, AdressHome.toLocal8Bit());
    strcpy_s(this->_Adress.AdressStreet, AdressStreet.toLocal8Bit());
    strcpy_s(this->_Adress.AdressCity, AdressCity.toLocal8Bit());
    strcpy_s(this->NumberOfPhone, NumberOfPhone.toLocal8Bit());
    strcpy_s(this->Operator, "");
    strcpy_s(this->path_to_photo, "");
    strcpy_s(this->Who_is_it, "");
}

AboutHuman::~AboutHuman(){}

void AboutHuman::SetAdressFlat(QString AdressFlat)
{
    strcpy_s(this->_Adress.AdressFlat, AdressFlat.toLocal8Bit());
}

void AboutHuman::SetAdressHome(QString AdressHome)
{
    strcpy_s(this->_Adress.AdressHome, AdressHome.toLocal8Bit());
}

void AboutHuman::SetAdressStreet(QString AdressStreet)
{
    strcpy_s(this->_Adress.AdressStreet, AdressStreet.toLocal8Bit());
}

void AboutHuman::SetAdressCity(QString AdressCity)
{
    strcpy_s(this->_Adress.AdressCity, AdressCity.toLocal8Bit());
}

void AboutHuman::SetNumberOfPhone(QString NumberOfPhone)
{
    strcpy_s(this->NumberOfPhone, NumberOfPhone.toLocal8Bit());
}

void AboutHuman::SetPath_to_photo(QString Path_to_photo)
{
    strcpy_s(this->path_to_photo, Path_to_photo.toLocal8Bit());
}

void AboutHuman::SetWho_is_it(QString Who_is_it)
{
    strcpy_s(this->Who_is_it, Who_is_it.toLocal8Bit());
}

QString AboutHuman::GetAdressFlat()
{
    return this->_Adress.AdressFlat;
}

QString AboutHuman::GetAdressHome()
{
    return this->_Adress.AdressHome;
}

QString AboutHuman::GetAdressStreet()
{
    return this->_Adress.AdressStreet;
}

QString AboutHuman::GetAdressCity()
{
    return this->_Adress.AdressCity;
}

QString AboutHuman::GetNumberOfPhone()
{
    return this->NumberOfPhone;
}

QString AboutHuman::GetOperator()
{
    return this->Operator;
}

QString AboutHuman::GetPath_to_photo()
{
    return this->path_to_photo;
}

QString AboutHuman::GetWho_is_it()
{
    return this->Who_is_it;
}

bool AboutHuman::_operatorDefinition()
{
    bool check = false;
    QString NumberOfPhone = this->NumberOfPhone;
    if("29" == NumberOfPhone.mid(5, 2) && (NumberOfPhone[8] == "1" || NumberOfPhone[8] == "3" ||
             NumberOfPhone[8] == "6" || NumberOfPhone[8] == "9"))
    {
            strcpy_s(this->Operator, "A1");
            check = true;
            return check;
    }
    else if("44" == NumberOfPhone.mid(5, 2))
    {
        strcpy_s(this->Operator, "A1");
        check = true;
        return check;
    }
    else if("29" == NumberOfPhone.mid(5, 2) && (NumberOfPhone[8] == "2" || NumberOfPhone[8] == "5" ||
              NumberOfPhone[8] == "7" || NumberOfPhone[8] == "8"))
    {
        strcpy_s(this->Operator, "MTC");
        check = true;
        return check;
    }
    else if("33" == NumberOfPhone.mid(5, 2))
    {
        strcpy_s(this->Operator, "MTC");
        check = true;
        return check;
    }
    else if("25" == NumberOfPhone.mid(5, 2))
    {
        strcpy_s(this->Operator, "life:)");
        check = true;
        return check;
    }
    else if("29" == NumberOfPhone.mid(5, 2) && NumberOfPhone[8] == "4")
    {
        strcpy_s(this->Operator, "life:)");
        check = true;
        return check;
    }
    else
        return check;
}

bool AboutHuman::_compare(AboutHuman a1, AboutHuman a2, int pole)
{
    if(pole == 0)
    {
        QString str1(a1.GetName()), str2(a2.GetName());
        if(str1 >= str2)
            return true;
    }
    else if(pole == 1)
    {
        QString str1(a1.GetSecondName()), str2(a2.GetSecondName());
        if(str1 >= str2)
            return true;
    }
    else if(pole == 2)
    {
        QString str1(a1.GetNumberOfPhone()), str2(a2.GetNumberOfPhone());
        if(str1 >= str2)
            return true;
    }
    return false;
}

AboutHuman& AboutHuman::operator =(const AboutHuman &temp)
{
    *dynamic_cast<Human*>(this) = dynamic_cast<const Human&>(temp);
    strcpy_s(this->Who_is_it, temp.Who_is_it);
    strcpy_s(this->_Adress.AdressFlat, temp._Adress.AdressFlat);
    strcpy_s(this->_Adress.AdressHome, temp._Adress.AdressHome);
    strcpy_s(this->_Adress.AdressStreet, temp._Adress.AdressStreet);
    strcpy_s(this->_Adress.AdressCity, temp._Adress.AdressCity);
    strcpy_s(this->NumberOfPhone, temp.NumberOfPhone);
    strcpy_s(this->Operator, temp.Operator);
    strcpy_s(this->path_to_photo, temp.path_to_photo);
    strcpy_s(this->Who_is_it, temp.Who_is_it);
    return *this;
}

bool AboutHuman::operator ==(const AboutHuman &temp)
{  
    if(*dynamic_cast<Human*>(this) == dynamic_cast<const Human&>(temp))
       if(this->GetWho_is_it() == temp.Who_is_it)
           if(this->GetOperator() == temp.Operator)
               if(this->GetAdressCity() == temp._Adress.AdressCity)
                   if(this->GetAdressStreet() == temp._Adress.AdressStreet)
                       if(this->GetAdressHome() == temp._Adress.AdressHome)
                           if(this->GetAdressFlat() == temp._Adress.AdressFlat)
                               if(this->GetNumberOfPhone() == temp.NumberOfPhone)
                                   if(this->GetPath_to_photo() == temp.path_to_photo)
                                       return true;
    return false;
}
