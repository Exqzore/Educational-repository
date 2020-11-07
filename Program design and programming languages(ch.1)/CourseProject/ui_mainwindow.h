/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QWidget *centralwidget;
    QListWidget *list_widget;
    QPushButton *btn_ADD;
    QComboBox *comboBox_sort_by;
    QComboBox *comboBox_menuPeople;
    QLineEdit *lineEdit;
    QGraphicsView *gv_Area;
    QPushButton *btn_Save;
    QPushButton *btn_Remove;
    QPushButton *btn_Edit;
    QPushButton *btn_Save_2;
    QLabel *label;
    QGraphicsView *graphicsView;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lab_HELLO;
    QMenuBar *menubar;
    QMenu *menu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1250, 880);
        MainWindow->setMinimumSize(QSize(1250, 880));
        MainWindow->setMaximumSize(QSize(1250, 880));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../\320\264\320\273\321\217 \321\204\320\276\320\275\320\260/xx.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(20, 46, 69);\n"
"background-color: rgb(15, 26, 42);"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMinimumSize(QSize(1250, 880));
        centralwidget->setMaximumSize(QSize(1250, 880));
        list_widget = new QListWidget(centralwidget);
        list_widget->setObjectName(QString::fromUtf8("list_widget"));
        list_widget->setGeometry(QRect(10, 140, 300, 641));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(14);
        font.setBold(false);
        font.setUnderline(false);
        font.setWeight(50);
        list_widget->setFont(font);
        list_widget->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"selection-background-color: rgb(34, 36, 49);\n"
"background-color: rgb(15, 26, 42);\n"
"border: none;\n"
"}\n"
"QListView{\n"
"color: rgb(0, 172, 229);\n"
"transition: 3s;\n"
"}\n"
"QListView::item:hover:!selected{\n"
"color: rgb(61, 145, 192);\n"
"border: 2px solid #828282;\n"
"border-color: rgb(2, 93, 151);\n"
"border-radius: 6px;\n"
"background-color: rgb(0,36,69);\n"
"}\n"
"QListView:item:selected{\n"
"color: rgb(61, 145, 192);\n"
"border: 2px solid #828282;\n"
"border-color: rgb(2, 93, 151);\n"
"border-radius: 6px;\n"
"background-color: rgb(0,36,69);\n"
"}\n"
"QScrollBar:vertical {   \n"
"        border: 1px;\n"
"	    border-color: rgb(11, 68, 112);\n"
"        background:white;\n"
"        width:10px;    \n"
"        margin: 0px 0px 0px 0px;\n"
"    }\n"
"    QScrollBar::handle:vertical {\n"
"        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop: 0  rgb(2, 93, 151), stop: 0.5 rgb(2, 93, 151), stop:1  rgb(2, 93, 151));\n"
"        min-height: 0px;\n"
"    }\n"
"    "
                        "QScrollBar::add-line:vertical {\n"
"        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop: 0  rgb(2, 93, 151), stop: 0.5  rgb(2, 93, 151),  stop:1  rgb(2, 93, 151));\n"
"        height: 0px;\n"
"        subcontrol-position: bottom;\n"
"        subcontrol-origin: margin;\n"
"    }\n"
"    QScrollBar::sub-line:vertical {\n"
"        background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop: 0  rgb(2, 93, 151), stop: 0.5  rgb(2, 93, 151),  stop:1 rgb(2, 93, 151));\n"
"        height: 0 px;\n"
"        subcontrol-position: top;\n"
"        subcontrol-origin: margin;\n"
"    }"));
        btn_ADD = new QPushButton(centralwidget);
        btn_ADD->setObjectName(QString::fromUtf8("btn_ADD"));
        btn_ADD->setGeometry(QRect(160, 100, 150, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(10);
        btn_ADD->setFont(font1);
        btn_ADD->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"border-radius: 1px;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        comboBox_sort_by = new QComboBox(centralwidget);
        comboBox_sort_by->addItem(QString());
        comboBox_sort_by->addItem(QString());
        comboBox_sort_by->addItem(QString());
        comboBox_sort_by->setObjectName(QString::fromUtf8("comboBox_sort_by"));
        comboBox_sort_by->setGeometry(QRect(10, 100, 150, 40));
        comboBox_sort_by->setFont(font1);
        comboBox_sort_by->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"border-radius: 1px;\n"
"}\n"
"QComboBox:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"QComboBox::drop-down{\n"
"    border:none;\n"
"}\n"
"QListView{\n"
"    border: none;\n"
"    color: rgb(0, 172, 229);\n"
"    selection-background-color: rgb(0, 172, 229);\n"
"    background-color: rgb(11, 68, 112);\n"
"}\n"
""));
        comboBox_menuPeople = new QComboBox(centralwidget);
        comboBox_menuPeople->addItem(QString());
        comboBox_menuPeople->addItem(QString());
        comboBox_menuPeople->addItem(QString());
        comboBox_menuPeople->addItem(QString());
        comboBox_menuPeople->addItem(QString());
        comboBox_menuPeople->setObjectName(QString::fromUtf8("comboBox_menuPeople"));
        comboBox_menuPeople->setGeometry(QRect(10, 50, 300, 40));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Agency FB"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        comboBox_menuPeople->setFont(font2);
        comboBox_menuPeople->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"border-radius: 1px;\n"
"}\n"
"QComboBox:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"QComboBox::drop-down{\n"
"    border:none;\n"
"}\n"
"QListView{\n"
"    border: none;\n"
"    color: rgb(0, 172, 229);\n"
"    selection-background-color: rgb(0, 172, 229);\n"
"    background-color: rgb(11, 68, 112);\n"
"}"));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 10, 280, 30));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Agency FB"));
        font3.setPointSize(14);
        font3.setBold(true);
        font3.setWeight(75);
        lineEdit->setFont(font3);
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"color: rgb(81, 81, 81);\n"
"	background-color: rgb(223, 223, 223);\n"
"border: 2px solid #828282;\n"
"border-radius: 14px;\n"
"border-color: rgb(0, 166, 255);\n"
"}\n"
""));
        gv_Area = new QGraphicsView(centralwidget);
        gv_Area->setObjectName(QString::fromUtf8("gv_Area"));
        gv_Area->setGeometry(QRect(320, 90, 920, 691));
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        gv_Area->setFont(font4);
        gv_Area->setStyleSheet(QString::fromUtf8("QGraphicsView{\n"
"border: none;\n"
"	background-color: rgb(15, 26, 42);\n"
"}"));
        btn_Save = new QPushButton(centralwidget);
        btn_Save->setObjectName(QString::fromUtf8("btn_Save"));
        btn_Save->setGeometry(QRect(1050, 790, 191, 61));
        btn_Save->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color:rgb(0, 172, 229);\n"
"border: 2px solid #828282;;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        btn_Remove = new QPushButton(centralwidget);
        btn_Remove->setObjectName(QString::fromUtf8("btn_Remove"));
        btn_Remove->setGeometry(QRect(850, 790, 191, 61));
        btn_Remove->setFont(font1);
        btn_Remove->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        btn_Edit = new QPushButton(centralwidget);
        btn_Edit->setObjectName(QString::fromUtf8("btn_Edit"));
        btn_Edit->setGeometry(QRect(1050, 790, 191, 61));
        btn_Edit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        btn_Save_2 = new QPushButton(centralwidget);
        btn_Save_2->setObjectName(QString::fromUtf8("btn_Save_2"));
        btn_Save_2->setGeometry(QRect(1050, 790, 191, 61));
        btn_Save_2->setFont(font1);
        btn_Save_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"color: rgb(0, 172, 229);\n"
"border: 2px solid #828282;;\n"
"border-color: rgb(11, 68, 112);\n"
"background-color: rgb(11, 68, 112);\n"
"border-bottom-color: rgb(0, 172, 229);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 790, 291, 61));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Proxy 1"));
        font5.setPointSize(29);
        label->setFont(font5);
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(20, 46, 69);"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 1250, 880));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: rgb(20, 46, 69);\n"
"border: none;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(600, 10, 361, 71));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Candara Light"));
        font6.setPointSize(44);
        label_2->setFont(font6);
        label_2->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(20, 46, 69);"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(960, 31, 261, 41));
        QFont font7;
        font7.setPointSize(20);
        label_3->setFont(font7);
        label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(350, 31, 251, 41));
        label_4->setFont(font7);
        label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));
        lab_HELLO = new QLabel(centralwidget);
        lab_HELLO->setObjectName(QString::fromUtf8("lab_HELLO"));
        lab_HELLO->setGeometry(QRect(490, 340, 591, 151));
        QFont font8;
        font8.setFamily(QString::fromUtf8("Proxy 9"));
        font8.setPointSize(101);
        lab_HELLO->setFont(font8);
        lab_HELLO->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));
        MainWindow->setCentralWidget(centralwidget);
        graphicsView->raise();
        list_widget->raise();
        btn_ADD->raise();
        comboBox_sort_by->raise();
        comboBox_menuPeople->raise();
        lineEdit->raise();
        gv_Area->raise();
        btn_Save->raise();
        btn_Remove->raise();
        btn_Edit->raise();
        btn_Save_2->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        lab_HELLO->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1250, 21));
        menubar->setStyleSheet(QString::fromUtf8("QMenuBar{\n"
"border: none;\n"
"color:rgb(0, 172, 229);\n"
"	background-color: rgb(15, 26, 42);\n"
"}\n"
"\n"
"QMenuBar:hover{\n"
"	background-color: rgb(15, 26, 42);\n"
"}\n"
"\n"
"QMenuBar:pressed{\n"
"	background-color: rgb(15, 26, 42);\n"
"}"));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setStyleSheet(QString::fromUtf8("QMenu{\n"
"color:rgb(0, 172, 229);\n"
"background-color:  rgb(11, 68, 112);\n"
"}\n"
"\n"
"QMenu:hover{\n"
"background-color: rgb(15, 85, 139);\n"
"}\n"
"\n"
"QMenu:pressed{\n"
"background-color: rgb(15, 85, 139);\n"
"}"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu->menuAction());
        menu->addSeparator();
        menu->addAction(action);
        menu->addAction(action_2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "ADRESS BOOK", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        action_2->setText(QCoreApplication::translate("MainWindow", "\320\223\320\273\320\260\320\262\320\275\320\276\320\265 \320\274\320\265\320\275\321\216", nullptr));
        btn_ADD->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        comboBox_sort_by->setItemText(0, QCoreApplication::translate("MainWindow", "\320\237\320\276 \320\270\320\274\320\265\320\275\320\270", nullptr));
        comboBox_sort_by->setItemText(1, QCoreApplication::translate("MainWindow", "\320\237\320\276 \321\204\320\260\320\274\320\270\320\273\320\270\320\270", nullptr));
        comboBox_sort_by->setItemText(2, QCoreApplication::translate("MainWindow", "\320\237\320\276 \321\202\320\265\320\273\320\265\321\204\320\276\320\275\321\203", nullptr));

        comboBox_menuPeople->setItemText(0, QCoreApplication::translate("MainWindow", "\320\222\321\201\320\265", nullptr));
        comboBox_menuPeople->setItemText(1, QCoreApplication::translate("MainWindow", "\320\240\320\276\320\264\321\201\321\202\320\262\320\265\320\275\320\275\320\270\320\272\320\270", nullptr));
        comboBox_menuPeople->setItemText(2, QCoreApplication::translate("MainWindow", "\320\224\321\200\321\203\320\267\321\214\321\217", nullptr));
        comboBox_menuPeople->setItemText(3, QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\273\320\265\320\263\320\270", nullptr));
        comboBox_menuPeople->setItemText(4, QCoreApplication::translate("MainWindow", "\320\235\320\265\320\267\320\275\320\260\320\272\320\276\320\274\321\213\320\265", nullptr));

        btn_Save->setText(QCoreApplication::translate("MainWindow", "\320\241\320\236\320\245\320\240\320\220\320\235\320\230\320\242\320\254", nullptr));
        btn_Remove->setText(QCoreApplication::translate("MainWindow", "\320\243\320\224\320\220\320\233\320\230\320\242\320\254", nullptr));
        btn_Edit->setText(QCoreApplication::translate("MainWindow", "\320\240\320\225\320\224\320\220\320\232\320\242\320\230\320\240\320\236\320\222\320\220\320\242\320\254", nullptr));
        btn_Save_2->setText(QCoreApplication::translate("MainWindow", "\320\241\320\236\320\245\320\240\320\220\320\235\320\230\320\242\320\254", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "ADRESS BOOK", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "________________________________________________________", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "________________________________________________________", nullptr));
        lab_HELLO->setText(QCoreApplication::translate("MainWindow", "HELLO!!!", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
