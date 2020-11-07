#include "friend.h"

Friend::Friend() :
    AboutHuman(), Date()
{
    this->ChildhoodFriend = false;
    this->SchoolFriend = false;
    this->UniversityFriend = false;
    this->WorkFriend = false;
}

Friend::Friend(QString Who_is_it, QString AdressFlat, QString AdressHome, QString AdressStreet, QString AdressCity,
               QString NumberOfPhone, QString Name, QString SecondName, QString FatherName,
               int ch, QString _data) :
    AboutHuman(Who_is_it, AdressFlat, AdressHome, AdressStreet, AdressCity,
               NumberOfPhone, Name, SecondName, FatherName),
    Date(_data)

{
    if(ch == 1)
        this->ChildhoodFriend = true;
    else this->ChildhoodFriend = false;
    if(ch == 2)
         this->SchoolFriend = true;
    else this->SchoolFriend = false;
    if(ch == 3)
         this->UniversityFriend = true;
    else this->UniversityFriend = false;
    if(ch == 4)
         this->WorkFriend = true;
    else this->WorkFriend = false;
}

Friend::~Friend(){}

void Friend::SetTypeOfFriend(int ch)
{
    if(ch == 2)
        this->ChildhoodFriend = true;
    else this->ChildhoodFriend = false;
    if(ch == 1)
         this->SchoolFriend = true;
    else this->SchoolFriend = false;
    if(ch == 3)
         this->UniversityFriend = true;
    else this->UniversityFriend = false;
    if(ch == 4)
         this->WorkFriend = true;
    else this->WorkFriend = false;
}

QString Friend::GetTyoeOfFriend()
{
    if(this->ChildhoodFriend)
        return "Childhood Friend";
    else if(this->SchoolFriend)
        return "School Friend";
    else if(this->UniversityFriend)
        return "University Friend";
    else if(this->WorkFriend)
        return "Work Friend";
    else return "Not a friend";
}

Friend& Friend::operator =(const Friend &temp)
{
    *dynamic_cast<AboutHuman*>(this) = dynamic_cast<const AboutHuman&>(temp);
    *dynamic_cast<Date*>(this) = dynamic_cast<const Date&>(temp);
    this->WorkFriend = temp.WorkFriend;
    this->SchoolFriend = temp.SchoolFriend;
    this->ChildhoodFriend = temp.ChildhoodFriend;
    this->UniversityFriend = temp.UniversityFriend;
    return *this;
}

bool Friend::operator ==(const Friend &temp)
{
    if(*dynamic_cast<AboutHuman*>(this) == dynamic_cast<const AboutHuman&>(temp))
       if(*dynamic_cast<Date*>(this) == dynamic_cast<const Date&>(temp))
           if(this->WorkFriend == temp.WorkFriend)
               if(this->SchoolFriend == temp.SchoolFriend)
                   if(this->ChildhoodFriend == temp.ChildhoodFriend)
                       if(this->UniversityFriend == temp.UniversityFriend)
                           return true;
    return false;
}
