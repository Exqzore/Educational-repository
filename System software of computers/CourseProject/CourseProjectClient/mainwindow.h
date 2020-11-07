#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QDir>
#include <QFileSystemModel>
#include <QList>
#include <QDesktopServices>

#include <windows.h>
#include <shlwapi.h>

#include "FileInfo.h"
#include "CodeAction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getIP();
    void setIP(QString);

public slots:
    void sockConnected();
    void sockReadyRead();
    void sockErrorConnect(QAbstractSocket::SocketError);
    void sockDisconnected();

private slots:
    void on_btn_changeIP_clicked();
    void on_btn_apply_clicked();
    void on_btn_toConnect_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_btn_info_clicked();
    void on_btn_searchMenu_clicked();
    void on_btn_search_clicked();
    void on_btn_toDisconnect_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_2_itemClicked(QListWidgetItem *item);
    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);

private:
    int indexOfFile;
    int indexOfSearchFile;
    QString IP;
    QTcpSocket* socket;
    QList<FileInfo> tempListInfo;
    QList<FileInfo> tempListSearchInfo;
    QString createPath(QString);

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
