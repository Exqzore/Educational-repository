#include "date.h"

Date::Date()
{
    strcpy_s(this->_data, "");
}

Date::Date(QString _data)
{
    strcpy_s(this->_data, _data.toLocal8Bit());
}

Date::~Date(){}

void Date::Set_Data(QString _data)
{
    strcpy_s(this->_data, _data.toLocal8Bit());
}

QString Date::Get_Data_s()
{
    return this->_data;
}

QString Date::Get_Data()
{
    return QDate::fromString(this->_data,"dd/MM/yyyy").toString();
}

qint64 Date::DeterminationOfDaysBirthday()
{
    QDate _temp = QDate::currentDate(), _date = QDate::fromString(this->_data,"dd/MM/yyyy");
    if(_temp.month() > _date.month())
        _temp.setDate(_temp.year() + 1, _date.month(), _date.day());
    else if(_temp.month() == _date.month())
    {
        if(_temp.day() > _date.day())
            _temp.setDate(_temp.year() + 1, _date.month(), _date.day());
        else _temp.setDate(_temp.year(), _date.month(), _date.day());
    }
    else _temp.setDate(_temp.year() + 1, _date.month(), _date.day());
    return QDate::currentDate().daysTo(_temp);
}

Date& Date::operator =(const Date &temp)
{
    strcpy_s(this->_data, temp._data);
    return *this;
}

bool Date::operator ==(const Date &temp)
{
    if(this->_data == temp._data)
        return true;
    return false;
}
