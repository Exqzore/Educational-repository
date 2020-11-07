#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QFile file("IP.txt");
    file.open(QIODevice::ReadOnly);
    this->IP = file.readLine();
    file.close();
    ui->line_currentIP->setText(this->IP);
    ui->line_currentIP->setEnabled(false);
    ui->btn_apply->setVisible(false);
    ui->textEdit->setVisible(false);
    ui->btn_searchMenu->setEnabled(false);
    ui->progressBar->reset();
    ui->btn_toDisconnect->setVisible(false);
    this->socket = new QTcpSocket(this);
    connect(this->socket, SIGNAL(connected()), SLOT(sockConnected()));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(sockReadyRead()));
    connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sockErrorConnect(QAbstractSocket::SocketError)));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
}

MainWindow::~MainWindow() {
    delete ui;
}

QString MainWindow::getIP() {
    return this->IP;
}

void MainWindow::setIP(QString ip) {
    this->IP = ip;
}

void MainWindow::on_btn_changeIP_clicked() {
    ui->line_currentIP->setEnabled(true);
    ui->btn_changeIP->setVisible(false);
    ui->btn_apply->setVisible(true);
}

void MainWindow::on_btn_apply_clicked() {
    QFile file("IP.txt");
    file.open(QIODevice::WriteOnly);
    file.write(ui->line_currentIP->text().toUtf8());
    file.close();
    this->IP = ui->line_currentIP->text();
    ui->line_currentIP->setEnabled(false);
    ui->btn_changeIP->setVisible(true);
    ui->btn_apply->setVisible(false);
}

void MainWindow::sockConnected() {
    QMessageBox::about(this, "Message", "We connected");
}

void MainWindow::sockDisconnected() {
}

void MainWindow::sockReadyRead() {
    int ch = 0;
    FileInfo tempFileInfo;
    this->socket = (QTcpSocket*)sender();
    QDataStream in(this->socket);
    in.setVersion(QDataStream::Qt_5_14);
    int tempAction;
    in >> tempAction;
    if (tempAction == CodeAction::InfoDirectory) {
        int size;
        in >> size;
        ui->progressBar->setRange(0, size);
        ui->progressBar->setValue(0);
        tempListInfo.clear();
        while (true) {
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.sizeFilePath;
            if (this->socket->bytesAvailable() < tempFileInfo.sizeFilePath) break;
            in >> tempFileInfo.filePath;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.sizeFileName;
            if(this->socket->bytesAvailable() < tempFileInfo.sizeFileName) break;
            in >> tempFileInfo.fileName;
            tempFileInfo.filePath += tempFileInfo.fileName + "/";
            tempFileInfo.sizeFilePath += tempFileInfo.sizeFileName + 1;
            if (this->socket->bytesAvailable() < sizeof(bool)) break;
            in >> tempFileInfo.typeOfFile;
            if (this->socket->bytesAvailable() < sizeof(qint64)) break;
            in >> tempFileInfo.sizeFile;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.dayDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.monthDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.yearDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.dayLastModifiedDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.monthLastModifiedDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.yearLastModifiedDate;
            if (tempFileInfo.fileName == ".") ch++;
            if (tempFileInfo.fileName.mid(1, 2) == ":/") ch--;
            tempListInfo.push_back(tempFileInfo);
            ui->progressBar->setValue(ui->progressBar->value() + sizeof(tempFileInfo));
        }
        ui->progressBar->setValue(size);
        if (!ch && tempListInfo.size()) {
            FileInfo temp1;
            temp1.filePath = tempListInfo[0].filePath.mid(0, 3) + "./";
            temp1.sizeFilePath = temp1.filePath.size();
            temp1.fileName = ".";
            temp1.sizeFile = 1;
            temp1.typeOfFile = true;
            tempListInfo.push_front(temp1);
        } else ch = 0;
        ui->listWidget->clear();
        if(tempListInfo.size()) ui->lb_path->setText(this->createPath(tempListInfo[0].filePath));
        for (int i = 0; i < tempListInfo.count(); i++) {
            QListWidgetItem* tempItem = new QListWidgetItem();
            if (tempListInfo[i].typeOfFile && tempListInfo[i].fileName.mid(1,2) == ":/") tempItem->setIcon(QIcon("D:/section.png"));
            else if (tempListInfo[i].typeOfFile) tempItem->setIcon(QIcon("D:/folder.png"));
            else if (!tempListInfo[i].typeOfFile) tempItem->setIcon(QIcon("D:/file.png"));
            tempItem->setText(tempListInfo[i].fileName);
            ui->listWidget->addItem(tempItem);
        }
    }
    else if (tempAction == CodeAction::ReadFileToSocket) {
        int fileSize;
        QByteArray fileBytes = NULL;
        QString tempNameFile = "D:/tempFile" + tempListInfo[indexOfFile].fileName;
        QFile file(tempNameFile);
        in >> fileSize;
        ui->progressBar->setRange(0, fileSize);
        ui->progressBar->setValue(0);
        while(true) {
            QByteArray temp = this->socket->read(2048);
            fileBytes += temp;
            temp.clear();
            ui->progressBar->setValue(fileBytes.size());
            if(fileBytes.size() == fileSize) break;
            this->socket->waitForReadyRead(100000);
        }
        file.open(QIODevice::WriteOnly);
        file.write(fileBytes);
        file.close();
        QDesktopServices::openUrl((QUrl(tempNameFile, QUrl::TolerantMode)));
    }
    else if (tempAction == CodeAction::ReadSearchFileToSocket) {
        int fileSize;
        QByteArray fileBytes = NULL;
        QString tempNameFile = "D:/tempFile" + tempListSearchInfo[indexOfSearchFile].fileName;
        QFile file(tempNameFile);
        in >> fileSize;
        ui->progressBar->setRange(0, fileSize);
        ui->progressBar->setValue(0);
        while(true) {
            QByteArray temp = this->socket->read(2048);
            fileBytes += temp;
            temp.clear();
            ui->progressBar->setValue(fileBytes.size());
            if(fileBytes.size() == fileSize) break;
            this->socket->waitForReadyRead(100000);
        }
        file.open(QIODevice::WriteOnly);
        file.write(fileBytes);
        file.close();
        QDesktopServices::openUrl((QUrl(tempNameFile, QUrl::TolerantMode)));
    }
    else if (tempAction == CodeAction::SearchAllFile) {
        ui->listWidget_2->clear();
        tempListSearchInfo.clear();
        while (true) {
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.sizeFilePath;
            if (this->socket->bytesAvailable() < tempFileInfo.sizeFilePath) break;
            in >> tempFileInfo.filePath;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.sizeFileName;
            if(this->socket->bytesAvailable() < tempFileInfo.sizeFileName) break;
            in >> tempFileInfo.fileName;
            tempFileInfo.filePath += tempFileInfo.fileName + "/";
            tempFileInfo.sizeFilePath += tempFileInfo.sizeFileName + 1;
            if (this->socket->bytesAvailable() < sizeof(bool)) break;
            in >> tempFileInfo.typeOfFile;
            if (this->socket->bytesAvailable() < sizeof(qint64)) break;
            in >> tempFileInfo.sizeFile;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.dayDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.monthDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.yearDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.dayLastModifiedDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.monthLastModifiedDate;
            if (this->socket->bytesAvailable() < sizeof(int)) break;
            in >> tempFileInfo.yearLastModifiedDate;
            tempListSearchInfo.push_back(tempFileInfo);
        }
        for (int i = 0; i < tempListSearchInfo.count(); i++) {
            QListWidgetItem* tempItem = new QListWidgetItem();
            if (tempListSearchInfo[i].typeOfFile && tempListSearchInfo[i].fileName.mid(1,2) == ":/") tempItem->setIcon(QIcon("D:/section.png"));
            else if (tempListSearchInfo[i].typeOfFile) tempItem->setIcon(QIcon("D:/folder.png"));
            else if (!tempListSearchInfo[i].typeOfFile) tempItem->setIcon(QIcon("D:/file.png"));
            tempItem->setText(tempListSearchInfo[i].fileName);
            ui->listWidget_2->addItem(tempItem);
        }
        ui->btn_search->setEnabled(true);
        ui->listWidget->setEnabled(true);
        ui->listWidget_2->setEnabled(true);
        ui->btn_toDisconnect->setEnabled(true);
    }
    else if (tempAction == CodeAction::NoFiles) {
        QMessageBox::about(this, "Message", "There are no such files on this device.");
        ui->btn_search->setEnabled(true);
        ui->listWidget->setEnabled(true);
        ui->listWidget_2->setEnabled(true);
        ui->btn_toDisconnect->setEnabled(true);
    }
}

void MainWindow::sockErrorConnect(QAbstractSocket::SocketError err) {
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                             "The host was not found." :
                             err == QAbstractSocket::RemoteHostClosedError ?
                                 "The remote host is closed." :
                                 err == QAbstractSocket::ConnectionRefusedError ?
                                     "The connection was refused." :
                                     QString(socket->errorString())
                                     );
    QMessageBox::critical(this, "Error", strError);
}

void MainWindow::on_btn_toConnect_clicked() {
    ui->btn_toDisconnect->setVisible(true);
    ui->btn_toConnect->setVisible(false);
    ui->btn_info->setEnabled(false);
    ui->btn_apply->setEnabled(false);
    ui->btn_changeIP->setEnabled(false);
    ui->listWidget->setEnabled(true);
    ui->listWidget_2->setEnabled(true);
    ui->textEdit->setEnabled(true);
    ui->btn_search->setEnabled(true);
    ui->btn_searchMenu->setEnabled(true);
    ui->line_search->setEnabled(true);
    this->socket->connectToHost(this->IP, 5555);
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    for (int i = 0; i < tempListInfo.size(); i++) {
        if (item->text() == tempListInfo[i].fileName) {
            if (!tempListInfo[i].typeOfFile) {
                out << CodeAction::UploadFileToClient;
                out << tempListInfo[i].filePath;
                indexOfFile = i;
            }
            else out << CodeAction::GoToDirectory;
            out << tempListInfo[i].filePath;
            break;
        }
    }
    this->socket->write(byteArray);
}

QString MainWindow::createPath(QString path) {
    if(path.size() == 4) return path = "Drives";
    else {
        int i;
        for(i = path.size() - 2; path[i] != '/'; i--) if(i <= 0) break;
        i++;
        path.remove(i, path.size() - i);
    }
    return path;
}

void MainWindow::on_btn_searchMenu_clicked() {
    ui->btn_searchMenu->setEnabled(false);
    ui->btn_info->setEnabled(true);
    ui->textEdit->setVisible(false);
    ui->btn_search->setVisible(true);
    ui->line_search->setVisible(true);
    ui->listWidget_2->setVisible(true);
}

void MainWindow::on_btn_info_clicked() {
    ui->btn_searchMenu->setEnabled(true);
    ui->btn_info->setEnabled(false);
    ui->btn_search->setVisible(false);
    ui->line_search->setVisible(false);
    ui->listWidget_2->setVisible(false);
    ui->textEdit->setVisible(true);
}

void MainWindow::on_btn_search_clicked() {
    if(ui->line_search->text() == "") {
        QMessageBox::about(this, "Message", "Search bar is empty");
    }
    else {
        ui->btn_search->setEnabled(false);
        ui->listWidget->setEnabled(false);
        ui->listWidget_2->setEnabled(false);
        ui->btn_toDisconnect->setEnabled(false);
        QByteArray byteArray;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_14);
        out << CodeAction::SearchFile << ui->line_search->text();
        this->socket->write(byteArray);
    }
}

void MainWindow::on_btn_toDisconnect_clicked() {
    ui->btn_toConnect->setVisible(true);
    ui->btn_toDisconnect->setVisible(false);
    ui->listWidget->setEnabled(false);
    ui->listWidget_2->setEnabled(false);
    ui->btn_info->setEnabled(false);
    ui->btn_apply->setEnabled(true);
    ui->btn_changeIP->setEnabled(true);
    ui->textEdit->setEnabled(false);
    ui->btn_search->setEnabled(false);
    ui->btn_searchMenu->setEnabled(false);
    ui->line_search->setEnabled(false);
    this->socket->disconnectFromHost();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item) {
    for (int i = 0; i < tempListInfo.size(); i++) {
        if (item->text() == tempListInfo[i].fileName) {
            ui->btn_searchMenu->setEnabled(true);
            ui->btn_info->setEnabled(false);
            ui->btn_search->setVisible(false);
            ui->line_search->setVisible(false);
            ui->listWidget_2->setVisible(false);
            ui->textEdit->setVisible(true);
            ui->textEdit->clear();
            QString temp;
            if (tempListInfo[i].typeOfFile) temp = "directory";
            else temp = "file";
            ui->textEdit->setText("Name of file: " + tempListInfo[i].fileName + "\n"
                                  "Full path to file: " + tempListInfo[i].filePath + "\n"
                                  "Type: " + temp + "\n"
                                  "Size of file: " + QString::number(tempListInfo[i].sizeFile) + " bytes\n"
                                  "Date of creation: " + QString::number(tempListInfo[i].dayDate)
                                  + "." + QString::number(tempListInfo[i].monthDate)
                                  + "." + QString::number(tempListInfo[i].yearDate) + "\n"
                                  "Date of last modification: " + QString::number(tempListInfo[i].dayLastModifiedDate)
                                  + "." + QString::number(tempListInfo[i].monthLastModifiedDate)
                                  + "." + QString::number(tempListInfo[i].yearLastModifiedDate));
            break;
        }
    }
}

void MainWindow::on_listWidget_2_itemClicked(QListWidgetItem *item) {
    for (int i = 0; i < tempListSearchInfo.size(); i++) {
        if (item->text() == tempListSearchInfo[i].fileName) {
            ui->btn_searchMenu->setEnabled(true);
            ui->btn_info->setEnabled(true);
            ui->btn_search->setVisible(true);
            ui->line_search->setVisible(true);
            ui->listWidget_2->setVisible(true);
            ui->textEdit->setVisible(true);
            ui->textEdit->clear();
            QString temp;
            if (tempListSearchInfo[i].typeOfFile) temp = "directory";
            else temp = "file";
            ui->textEdit->setText("Name of file: " + tempListSearchInfo[i].fileName + "\n"
                                  "Full path to file: " + tempListSearchInfo[i].filePath + "\n"
                                  "Type: " + temp + "\n"
                                  "Size of file: " + QString::number(tempListSearchInfo[i].sizeFile) + " bytes\n"
                                  "Date of creation: " + QString::number(tempListSearchInfo[i].dayDate)
                                  + "." + QString::number(tempListSearchInfo[i].monthDate)
                                  + "." + QString::number(tempListSearchInfo[i].yearDate) + "\n"
                                  "Date of last modification: " + QString::number(tempListSearchInfo[i].dayLastModifiedDate)
                                  + "." + QString::number(tempListSearchInfo[i].monthLastModifiedDate)
                                  + "." + QString::number(tempListSearchInfo[i].yearLastModifiedDate));
            break;
        }
    }
}

void MainWindow::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item) {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    for (int i = 0; i < tempListSearchInfo.size(); i++) {
        if (item->text() == tempListSearchInfo[i].fileName) {
            if (!tempListSearchInfo[i].typeOfFile) {
                out << CodeAction::UploadSearchFileToClient;
                out << tempListSearchInfo[i].filePath;
                indexOfSearchFile = i;
            }
            break;
        }
    }
    this->socket->write(byteArray);
}
