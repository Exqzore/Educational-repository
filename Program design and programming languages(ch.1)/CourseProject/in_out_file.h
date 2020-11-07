#ifndef IN_OUT_FILE_H
#define IN_OUT_FILE_H

#include "mitree.h"
#include "File.h"

template<typename T>
void cout_file(QList<T> &_tree, QString name)//Чтение всего файла в дерево
{
    BinFile file(name);
    file.in_open(BinFile::Type::IN);
    T temp;
    int n = file.size() / sizeof(temp);
    for (int i = 0; i < n; i++)
    {
        file.a_read(temp);
        _tree.push_back(temp);
    }
    file.in_close();
}

template<typename OBJ>
void write_to_file(OBJ obj, QString str)//Запись одного объекта в файл
{
    BinFile file(str);
    file.out_open(BinFile::OUT);
    file.a_write(obj);
    file.out_close();
}

template<typename It>
void write_to_new_file(It _begin, It _end, QString str)//Запись всего дерева в новый файл
{
    BinFile file(str);
    file.out_open(BinFile::NEW_OUT);
    for(; _begin < _end; _begin++)
        file.a_write(*_begin);
    file.out_close();
}

#endif // IN_OUT_FILE_H
