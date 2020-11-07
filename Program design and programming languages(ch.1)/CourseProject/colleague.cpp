#include "colleague.h"

Colleague::Colleague() :
    AboutHuman(), Date()
{
    strcpy_s(this->Post, "");
}

Colleague::Colleague(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet, QString AdressCity,
                     QString NumberOfPhone, QString Name, QString SecondName, QString FatherName,
                     QString Post, QString _data) :
    AboutHuman(Who_is_it, AdressFlat, AdressHome, AdressStreet, AdressCity,
               NumberOfPhone, Name, SecondName, FatherName),
    Date(_data)

{
    strcpy_s(this->Post, Post.toLocal8Bit());
}

Colleague::~Colleague(){}

void Colleague::SetPost(QString Post)
{
    strcpy_s(this->Post, Post.toLocal8Bit());
}

QString Colleague::GetPost()
{
    return this->Post;
}

Colleague& Colleague::operator =(const Colleague &temp)
{
    *dynamic_cast<AboutHuman*>(this) = dynamic_cast<const AboutHuman&>(temp);
    *dynamic_cast<Date*>(this) = dynamic_cast<const Date&>(temp);
    strcpy_s(this->Post, temp.Post);
    return *this;
}

bool Colleague::operator ==(const Colleague &temp)
{
    if(*dynamic_cast<AboutHuman*>(this) == dynamic_cast<const AboutHuman&>(temp))
       if(*dynamic_cast<Date*>(this) == dynamic_cast<const Date&>(temp))
           if(this->GetPost() == temp.Post)
               return true;
    return false;
}
