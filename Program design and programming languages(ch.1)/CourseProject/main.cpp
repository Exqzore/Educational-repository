#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    MainWindow w;
    QFile styleFile(":/style.css");
    styleFile.open(QFile::ReadOnly);
    a.setStyleSheet(styleFile.readAll());
    w.show();
    return a.exec();
}
