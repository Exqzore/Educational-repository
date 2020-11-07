#include "human.h"

Human::Human()
{
    strcpy_s(this->Name, "");
    strcpy_s(this->SecondName, "");
    strcpy_s(this->FatherName, "");
}

Human::Human(QString Name, QString SecondName, QString FatherName)
{
    strcpy_s(this->Name, Name.toLocal8Bit());
    strcpy_s(this->SecondName, SecondName.toLocal8Bit());
    strcpy_s(this->FatherName, FatherName.toLocal8Bit());
}

Human::~Human(){}

void Human::SetName(QString Name)
{
    strcpy_s(this->Name, Name.toLocal8Bit());
}

void Human::SetSecondName(QString SecondName)
{
    strcpy_s(this->SecondName, SecondName.toLocal8Bit());
}

void Human::SetFatherName(QString FatherName)
{
    strcpy_s(this->FatherName, FatherName.toLocal8Bit());
}

QString Human::GetName()
{
    return this->Name;
}

QString Human::GetSecondName()
{
    return this->SecondName;
}

QString Human::GetFatherName()
{
    return this->FatherName;
}

Human& Human::operator =(const Human &temp)
{
    strcpy_s(this->Name, temp.Name);
    strcpy_s(this->SecondName, temp.SecondName);
    strcpy_s(this->FatherName, temp.FatherName);
    return *this;
}

bool Human::operator ==(const Human &temp)
{
    if(this->Name == temp.Name)
        if(this->SecondName == temp.SecondName)
            if(this->FatherName == temp.FatherName)
                return true;
    return false;
}
