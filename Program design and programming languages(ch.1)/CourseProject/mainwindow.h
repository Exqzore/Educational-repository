#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QFile>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QIcon>
#include <QFont>
#include <QDate>
#include <QComboBox>
#include <QRadioButton>
#include <QListView>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include "check_function.h"
#include "abouthuman.h"
#include "family.h"
#include "colleague.h"
#include "friend.h"
#include "mitree.h"
#include "Ui.h"
#include "File.h"

#include <iterator>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    mitree<AboutHuman*> _all;//Дерево хранящее ссылки на все объекты

    QList<AboutHuman> _unknows;//Хранит незнакомых
    QList<Family> _family;//Хранит семью
    QList<Friend> _friend;//Хранит друзей
    QList<Colleague> _colleague;//Хранит коллег

    QList<int> _index;//Хранит индексы элементов

    QList<int> _actions;//Хранит последние выполняемые действия
    QList<int> _type;//Хранит тип с которым было проведено последнее действие
    QList<int> _type_edit;//Хранит последний изменяемый тип

    QList<Family> _data_Deleted_Family;//Хранит удаленные объекты семьи
    QList<Friend> _data_Deleted_Friend;//Хранит удаленные объекты друзей
    QList<Colleague> _data_Deleted_Colleague;//Хранит удаленные объекты коллег
    QList<AboutHuman> _data_Deleted_AboutHuman;//Хранит удаленные объекты незнакомых

    QString _Path_of_File;//Хранит путь к фотографии

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Заполнение дерева учитывая поисковое окно
    void under_Filling_ListWidget(mitree<AboutHuman*>&, QString);

    void Filling_ListWidget();//Заполнение панели объектов

    void Filling_Tree();//Заполнение дерева

    QString save_image(QString, QString, QString, QString);//Сохранение картинки в отдельный файл

    void data_entry_verification();//Функция проверки правильности заполнения всех полей

private slots:
    void on_btn_ADD_clicked();//Открывает меню для заполнения данных

    void changed_Enabled_AdressFlat();//Позволяет выборать тип дома

    void slot_Family();//Дорисовывает поле для заполнение данных семьи

    void slot_Friend();//Дорисовывает поле для заполнения данных друзей

    void slot_Colleague();//Дорисовывает поле для заполнения данных коллег

    void slot_Unknown();//Дорисовывает поле для заполнения данных незнакомых

    void on_btn_Image_clicked();//Открывает диалоговое окно выбора фотографий

    void on_btn_Save_clicked();//Сохраняет записанный объект в контейнеры и файл

    void on_list_widget_itemClicked(QListWidgetItem *item);//Показывает информацию выбранного объекта

    void on_comboBox_sort_by_currentIndexChanged(int index);//Переписывает дерево сортируя по выбранному полю

    void on_comboBox_menuPeople_currentIndexChanged(int index);//Показывает из дерева людей определенного типа

    void on_lineEdit_textChanged(const QString &arg1);//Функция поиска при изменении строки в поисковом поле

    void on_btn_Remove_clicked();//Удаление выбранного объекта

    void on_btn_Edit_clicked();//Редактирование выбранного объекта

    void on_btn_Save_2_clicked();//Сохранение отредактированного объекта в файл и контейнеры

    void on_action_triggered();//Отменяет последнее действие

    void Timer();//Запускает таймер(Часы)

    void on_action_2_triggered();//Выходит в главное меню

private:
    //begin fill window===================================
    QPushButton* btn_Image = nullptr;
    QRadioButton* Rb_Family = nullptr;
    QRadioButton* Rb_friend = nullptr;
    QRadioButton* Rb_colleague = nullptr;
    QRadioButton* Rb_Unknown = nullptr;
    QLineEdit* line_Name = nullptr;
    QLineEdit* line_SecondName = nullptr;
    QLineEdit* line_FatherName = nullptr;
    QLineEdit* line_Number = nullptr;
    QLineEdit* line_Date = nullptr;
    QLineEdit* line_FamilyPosition = nullptr;
    QLineEdit* line_Post = nullptr;
    QLineEdit* line_AdressCity = nullptr;
    QLineEdit* line_AdressStreet = nullptr;
    QLineEdit* line_AdressHome = nullptr;
    QLineEdit* line_AdressFlat = nullptr;
    QComboBox* CBox_CheckHome = nullptr;
    QComboBox* CBox_Friends = nullptr;
    QLabel* lab_Number = nullptr;
    QLabel* lab_Date = nullptr;
    QLabel* lab_Adress = nullptr;
    QLabel* lab_Check_Person = nullptr;
    QListView* view_CheckHome = nullptr;
    QListView* view_TypeOfFriends = nullptr;
    //end of fill window=================================

    //begin display window===============================
    QPushButton* _btn_Image = nullptr;
    QLabel* _lab_Name = nullptr;
    QLabel* _lab_Name_content = nullptr;
    QLabel* _lab_SecondName = nullptr;
    QLabel* _lab_SecondName_content = nullptr;
    QLabel* _lab_FatherName = nullptr;
    QLabel* _lab_FatherName_content = nullptr;
    QLabel* _lab_Number = nullptr;
    QLabel* _lab_Number_content = nullptr;
    QLabel* _lab_Adress = nullptr;
    QLabel* _lab_Adress_content = nullptr;
    QLabel* _lab_Date = nullptr;
    QLabel* _lab_Date_content = nullptr;
    QLabel* _lab_DateWaiting = nullptr;
    QLabel* _lab_DateWaiting_content = nullptr;
    QLabel* _lab_Operator_content = nullptr;
    QLabel* _lab_Who_is_it = nullptr;
    //end display window=================================

    QTimer* _timer;//Таймер

    template<typename Widget>
    void delete_element(Widget*&);//Функция удаления виджета
    void deleteElementsDynamic();//Удаление всех виджетов 1
    void deleteElementsStatic();//Удаление всех виджетов 2
    void setting_parameters_in_the_output_field();//Установление параметров интерфейса 1
    void setting_parameters_in_the_output_field_2();//Установление параметров интерфейса 2
    void display_interface_for_data_entry();//Отрисовывает интерфейс для заполнения данных
    void qpushbutton_display(QPushButton*&, QString, int, int, int, int, QIcon, QSize);//Отображение кнопок
    void qlineedit_display(QLineEdit*&, QString, int, int, int, int, QFont);//Отображение полей для записи
    void qlabel_display(QLabel*&, QString, int, int, int, int, QFont);//Отображение заполненных полей
    void qcombobox_display(QComboBox*&, QListView*&, int, int, int, int, QFont);//Отображение меню выбора
    void qradiobutton_dispaly(QRadioButton*&, QString, int, int, int, int, QFont);//Отображение радиокнопок

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
