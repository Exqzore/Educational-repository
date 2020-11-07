#ifndef MITREE_H
#define MITREE_H

#pragma once

template<typename Type>
class mitree
{
private:
    Type* value;
    mitree* _previos;
    mitree* _left;
    mitree* _right;

    //Получение элемента по индексу
    mitree<Type>* get_by_index(int index, mitree<Type>* a = nullptr)
    {
        static int count = -1;
        static mitree<Type>* temp = nullptr;
        if (!a)
        {
            a = this;
            count = -1;
            temp = nullptr;
        }
        if (temp) return temp;
        if (a->_left) get_by_index(index, a->_left);
        count++;
        if (count == index) temp = a;
        if (a->_right) get_by_index(index, a->_right);
        return temp;
    }
    //Функция удаления заданного узла
    void delete_fanction(mitree<Type>* removable)
    {
        if (!removable) removable = this;
        mitree<Type>* temp = removable;
        if (temp->_left)
        {
            temp = temp->_left;
            while (temp->_right)
                temp = temp->_right;
            if (temp->_previos != removable)
                temp->_previos->_right = temp->_left;
            else
            {
                removable->_left = temp->_left;
                if (temp->_left)
                    temp->_left->_previos = removable;
            }
            *(removable->value) = *(temp->value);
            if (temp->_previos->_left == temp)
                temp->_previos->_left = nullptr;
            if (temp->_previos->_right == temp)
                temp->_previos->_right = nullptr;
            temp->_left = nullptr;
            temp->_right = nullptr;
            temp->_previos = nullptr;
            delete temp;
        }
        else
            if (temp->_right)
            {
                temp = temp->_right;
                removable->_left = temp->_left;
                if (removable->_left)
                    removable->_left->_previos = removable;
                removable->_right = temp->_right;
                if (removable->_right)
                    removable->_right->_previos = removable;
                *(removable->value) = *(temp->value);
                temp->_left = nullptr;
                temp->_right = nullptr;
                temp->_previos = nullptr;
                delete temp;
            }
            else
            {
                if (removable != this)
                {
                    if (temp->_previos->_left == temp)
                        temp->_previos->_left = nullptr;
                    if (temp->_previos->_right == temp)
                        temp->_previos->_right = nullptr;
                    temp->_previos = nullptr;
                    delete temp;
                }
                else
                {
                    delete value;
                    value = nullptr;
                }
                return;
            }
    }
    //Проверка на наличие элемента в дереве
    bool check_element(Type element, mitree<Type>* a = nullptr)
    {
        static bool check = false;
        if (!a)
        {
            a = this;
            check = 0;
        }
        if (check) return check;
        if (a->_left) check_element(element, a->_left);
        if (a->value)
            if (element == *(a->value)) check = true;
        if (a->_right) check_element(element, a->_right);
        return check;
    }

public:
    mitree()//Конструктор по умолчанию
    {
        value = nullptr;
        _previos = nullptr;
        _left = nullptr;
        _right = nullptr;
    }

    void push(Type value)//Функция добавления элемента
    {
        if (!this->value)
        {
            this->value = new Type;
            *(this->value) = value;
        }
        else
            if (*(this->value) >= value)
            {
                if (!this->_left)
                    this->_left = new mitree<Type>;
                this->_left->_previos = this;
                this->_left->push(value);
            }
            else
            {
                if (!this->_right)
                    this->_right = new mitree<Type>;
                this->_right->_previos = this;
                this->_right->push(value);
            }
    }
    //Функция добавления ссылки на элемент
    void push(const Type &value)
    {
        if (!this->value)
        {
            this->value = new Type;
            *(this->value) = value;
        }
        else
            if (*(this->value) >= value)
            {
                if (!this->_left)
                    this->_left = new mitree<Type>;
                this->_left->_previos = this;
                this->_left->push(value);
            }
            else
            {
                if (!this->_right)
                    this->_right = new mitree<Type>;
                this->_right->_previos = this;
                this->_right->push(value);
            }
    }
    //Функция добавления элемента с возможностью выбрата поля для сравнения
    void push(Type value, int pole, bool (*comporate)(Type , Type, int))
    {
        if (!this->value)
        {
            this->value = new Type;
            *(this->value) = value;
        }
        else
            if (comporate(*(this->value), value ,pole))
            {
                if (!this->_left)
                    this->_left = new mitree<Type>;
                this->_left->_previos = this;
                this->_left->push(value, pole, comporate);
            }
            else
            {
                if (!this->_right)
                    this->_right = new mitree<Type>;
                this->_right->_previos = this;
                this->_right->push(value, pole, comporate);
            }
    }
    //Функция добавления ссылки на элемент с возможность выбора поля для сравнения
    template<typename Type_obj>
    void push(const Type & value, int pole, bool (*comporate)(Type_obj , Type_obj, int))
    {
        if (!this->value)
        {
            this->value = new Type;
            *(this->value) = value;
        }
        else
            if (comporate(*(*(this->value)), *value ,pole))
            {
                if (!this->_left)
                    this->_left = new mitree<Type>;
                this->_left->_previos = this;
                this->_left->push(value, pole, comporate);
            }
            else
            {
                if (!this->_right)
                    this->_right = new mitree<Type>;
                this->_right->_previos = this;
                this->_right->push(value, pole, comporate);
            }
    }

    void pop_element(Type element)//Функция удаления элемента
    {
        mitree<Type>* removable = get_by_index(mitree<Type>::get_index_element(element));
        mitree<Type>::delete_fanction(removable);
    }

    void pop(int index = 0)//функция удаления элемента по индексу
    {
        mitree<Type>* removable = get_by_index(index);
        mitree<Type>::delete_fanction(removable);
    }

    void pop_all_tree()//Функция удаления всего дерева
    {
        while (this->value)
            this->pop();
    }

    int size()//Возвращает размер дерева
    {
        if (this->value)
        {
            int _left_count = 0, _right_count = 0;
            if (this->_left)
                _left_count = this->_left->size();
            if (this->_right)
                _right_count = this->_right->size();
            return _left_count + _right_count + 1;
        }
        else return 0;
    }
    //Функция получения индекса элемента
    int get_index_element(Type element, mitree<Type>* a = nullptr)
    {
        static int count1 = -1;
        static int index = 0;
        if (!a)
        {
            a = this;
            count1 = -1;
            index = 0;
        }
        if (index) return index;
        if (a->_left) get_index_element(element, a->_left);
        count1++;
        if (element == *(a->value)) index = count1;
        if (a->_right) get_index_element(element, a->_right);
    }
    //Перегрузка оператора индексации
    Type operator[](int index)
    {
        return *(get_by_index(index)->value);
    }

    ~mitree()//Деструктор
    {
        delete value;
        delete _left;
        delete _right;
        value = nullptr;
        _left = nullptr;
        _right = nullptr;
    }

    class Iterator//Класс Iterator для премещения по дереву
    {
    private:
        mitree* MyTree;//Указатель на узел дерева
    public:
        //Конструктор по умолчанию
        Iterator()
        {
            this->MyTree = nullptr;
        }
        //Конструктор с параметрами
        Iterator(mitree* Temp)
        {
            this->MyTree = Temp;
        }
        //Конструктор копирования
        Iterator(const Iterator& Temp)
        {
            this->MyTree = Temp.MyTree;
        }
        //Оператор присваивания
        Iterator& operator =(const Iterator& Temp)
        {
            this->MyTree = Temp.MyTree;
            return *this;
        }
        //Оператор разадресации
        Type operator*()
        {
            return *(this->MyTree->value);
        }
        //Перегрузка оператора постфиксного ++
        Iterator operator ++(int)
        {
            Iterator TempTwo = *this;
            mitree* Temp = this->MyTree;
            mitree* TempOne = this->MyTree;
            while (TempOne->_previos)
                TempOne = TempOne->_previos;
            while (TempOne->_right)
                TempOne = TempOne->_right;
            if (Temp->_right)
            {
                Temp = Temp->_right;
                while (Temp->_left)
                    Temp = Temp->_left;
            }
            else
            {
                if (Temp == TempOne)
                    Temp = TempOne->_right;
                else
                {
                    if (Temp->_previos->_right == Temp)
                        Temp = Temp->_previos->_previos;
                    else
                    {
                        if (Temp->_previos)
                            Temp = Temp->_previos;
                    }
                }
            }
            this->MyTree = Temp;
            return TempTwo;
        }
        //Перегрузка оператора префиксного ++
        Iterator operator ++()
        {
            mitree* Temp = this->MyTree;
            mitree* TempOne = this->MyTree;
            while (TempOne->_previos)
                TempOne = TempOne->_previos;
            while (TempOne->_right)
                TempOne = TempOne->_right;
            if (Temp->_right)
            {
                Temp = Temp->_right;
                while (Temp->_left)
                    Temp = Temp->_left;
            }
            else
            {
                if (Temp == TempOne)
                    Temp = TempOne->_right;
                else
                {
                    if (Temp->_previos->_right == Temp)
                        Temp = (Temp->_previos)->_previos;
                    else
                    {
                        if (Temp->_previos)
                            Temp = Temp->_previos;
                    }
                }
            }
            this->MyTree = Temp;
            return *this;
        }
        //Перегрузка оператора префиксного --
        Iterator operator --()
        {
            mitree* Temp = this->MyTree;
            mitree* TempOne = this->MyTree;
            while (TempOne->_previos)
                TempOne = TempOne->_previos;
            while (TempOne->_left)
                TempOne = TempOne->_left;
            if (Temp->_left)
            {
                Temp = Temp->_left;
                while (Temp->_right)
                    Temp = Temp->_right;
            }
            else
            {
                if (Temp == TempOne)
                    Temp = TempOne->_left;
                else
                {
                    if (Temp->_previos->_left == Temp)
                        Temp = Temp->_previos->_previos;
                    else
                    {
                        if (Temp->_previos)
                            Temp = Temp->_previos;
                    }
                }
            }
            this->MyTree = Temp;
            return *this;
        }
        //Перегрузка оператора постфиксного --
        Iterator operator --(int)
        {
            Iterator TempTwo = *this;
            mitree* Temp = this->MyTree;
            mitree* TempOne = this->MyTree;
            while (TempOne->_previos)
                TempOne = TempOne->_previos;
            while (TempOne->_left)
                TempOne = TempOne->_left;
            if (Temp->_left)
            {
                Temp = Temp->_left;
                while (Temp->_right)
                    Temp = Temp->_right;
            }
            else
            {
                if (Temp == TempOne)
                    Temp = TempOne->_left;
                else
                {
                    if (Temp->_previos->_left == Temp)
                        Temp = Temp->_previos->_previos;
                    else
                    {
                        if (Temp->_previos)
                            Temp = Temp->_previos;
                    }
                }
            }
            this->MyTree = Temp;
            return TempTwo;
        }
        //Оператор сравнения
        bool operator ==(Iterator& Temp)
        {
            if (Temp.MyTree == this->MyTree)
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator !=(Iterator& Temp)
        {
            if (Temp.MyTree != this->MyTree)
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator !=(const Iterator& Temp)
        {
            if (Temp.MyTree != this->MyTree)
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator <(Iterator& Temp)
        {
            if (*(this->MyTree->value) < *(Temp.MyTree->value))
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator >(Iterator& Temp)
        {
            if (*(this->MyTree->value) > *(Temp.MyTree->value))
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator <=(Iterator& Temp)
        {
            if (*(this->MyTree->value) <= *(Temp.MyTree->value))
                return true;
            else
                return false;
        }
        //Оператор сравнения
        bool operator >=(Iterator& Temp)
        {
            if (*(this->MyTree->value) >= *(Temp.MyTree->value))
                return true;
            else
                return false;
        }
        //Оператор сложения +=
        Iterator& operator +=(int count)
        {
            for (int i = 0; i < count; i++)
            {
                (*this)++;
            }
            return *this;
        }
        //Оператор вычитания -=
        Iterator& operator -=(int count)
        {
            for (int i = count; i > 0; i--)
            {
                (*this)--;
            }
            return *this;
        }

        ~Iterator() {}
    };
    //Функци установления итератора на начало
    Iterator begin()
    {
        mitree* Temp = this;
        while(Temp->_left)
            Temp = Temp->_left;
        return Iterator(Temp);
    }
    //Функция установления итератора на конец
    Iterator end()
    {
        mitree* Temp = this;
        while(Temp)
            Temp = Temp->_right;
        return Iterator(Temp);
    }
    //Обратное начало
    Iterator rbegin()
    {
        mitree* Temp = this;
        while (Temp->_right)
            Temp = Temp->_right;
        return Iterator(Temp);
    }
    //Обратный конец
    Iterator rend()
    {
        mitree* Temp = this;
        while (Temp)
            Temp = Temp->_left;
        return Iterator(Temp);
    }
};

#endif // MITREE_H
