#ifndef FILE_H
#define FILE_H

#pragma once
#include <QString>
#include <fstream>
#include <string>
#include <QMessageBox>

using namespace std;

class File
{
protected:
    QString name;
    ifstream in;
    ofstream out;
public:
    File(QString name)
    {
        this->name = name;
    }

    ~File()
    {
        if (this->in.is_open())
            this->in.close();
        if (this->out.is_open())
            this->out.close();
    }

    void out_close()//Закртытие файла для записи
    {
        if (this->out.is_open())
            this->out.close();
    }

    void in_close()//Закрытие файла для чтения
    {
        if (this->in.is_open())
            this->in.close();
    }
};

class BinFile : public File
{
public:
    enum Type { OUT, NEW_OUT, IN };//Типы открытия файла

    BinFile(QString name) : File(name) {}

    bool out_open(int type)//Открытие файла для записи определенного типа
    {
        if (type == 0)
        {
            this->out.open(this->name.toStdString(), ios::app | ios::out | ios::binary);
            if (!this->out.is_open())
                return false;
        }
        else if (type == 1)
        {
            this->out.open(this->name.toStdString(), ios::out | ios::binary);
            if (!this->out.is_open())
                return false;
        }
        return true;
    }

    bool in_open(int type)//Открытие файла для чтения определенного типа
    {
        if (type == 2)
        {
            this->in.open(this->name.toStdString(), ios::in | ios::binary);
            if (!this->in.is_open())
                return false;
        }
        else return false;
        return true;
    }

    template<typename T>
    void a_write(T obj)//Запись в файл
    {
        if (!this->out.is_open())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Невозможно открыть файл");
            msgBox.exec();
            return;
        }
        else this->out.write((char*)(&obj), sizeof(obj));
    }

    template<typename T>
    void a_read(T& obj)//чтение файла
    {
        if (!this->in.is_open())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("Невозможно открыть файл");
            msgBox.exec();
            return;
        }
        else this->in.read((char*)(&obj), sizeof(obj));
    }

    int size()//Возвращает размер файла
    {
        if (this->in.is_open())
        {
            this->in.seekg(0, ios::end);
            int end = this->in.tellg();
            this->in.seekg(0);
            return end;
        }
        else if (this->out.is_open())
        {
            this->out.seekp(0, ios::end);
            int end = this->out.tellp();
            this->out.seekp(0);
            return end;
        }
        return 0;
    }
};

#endif // FILE_H
