#include "family.h"

Family::Family() :
    AboutHuman(), Date()
{
    strcpy_s(this->FamilyPosition, "");
}

Family::Family(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet, QString AdressCity,
               QString NumberOfPhone, QString Name, QString SecondName, QString FatherName,
               QString FamilyPosition, QString _data) :
    AboutHuman(Who_is_it, AdressFlat, AdressHome, AdressStreet, AdressCity,
               NumberOfPhone, Name, SecondName, FatherName),
    Date(_data)

{
    strcpy_s(this->FamilyPosition, FamilyPosition.toLocal8Bit());
}

Family::~Family(){}

void Family::SetFamilyPosition(QString FamilyPosition)
{
    strcpy_s(this->FamilyPosition, FamilyPosition.toLocal8Bit());
}

QString Family::GetFamilyPosition()
{
    return this->FamilyPosition;
}

Family& Family::operator =(const Family &temp)
{
    *dynamic_cast<AboutHuman*>(this) = dynamic_cast<const AboutHuman&>(temp);
    *dynamic_cast<Date*>(this) = dynamic_cast<const Date&>(temp);
    strcpy_s(this->FamilyPosition, temp.FamilyPosition);
    return *this;
}

bool Family::operator ==(const Family &temp)
{
    if(*dynamic_cast<AboutHuman*>(this) == dynamic_cast<const AboutHuman&>(temp))
       if(*dynamic_cast<Date*>(this) == dynamic_cast<const Date&>(temp))
           if(this->FamilyPosition == temp.FamilyPosition)
               return true;
    return false;
}
