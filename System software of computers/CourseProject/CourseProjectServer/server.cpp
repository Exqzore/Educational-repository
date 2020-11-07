#include "server.h"

Server::Server(){}

Server::~Server(){}

void Server::startServer() {
    if (this->listen(QHostAddress::Any, 5555)) {
        qDebug() << "Server is running";
    }
    else {
        qDebug() << "Server is not running";
    }
}

void Server::incomingConnection(qintptr handle) {
    this->socket = new QTcpSocket(this);
    this->socket->setSocketDescriptor(handle);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(sockReadyRead()));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    QByteArray byteArray = NULL, tempByteArray = NULL;
    QDataStream tempout(&tempByteArray, QIODevice::WriteOnly);
    tempout.setVersion(QDataStream::Qt_5_14);
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    out << CodeAction::InfoDirectory;
    TCHAR currentDrive[] = L"A:/";
    DWORD disksMask = GetLogicalDrives();
    for(int i = 0; i < 26; i++) {
        if(disksMask & 1) {
            tempout << 0 << QString("") << QString::fromWCharArray(currentDrive).size()
                    << QString::fromWCharArray(currentDrive) << true;
            tempout << qint64(0) << 0 << 0 << 0 << 0 << 0 << 0;
        }
        disksMask >>= 1;
        currentDrive[0]++;
    }
    out << tempByteArray.size();
    byteArray += tempByteArray;
    this->socket->write(byteArray);
    qDebug() << "Send client connect status - YES" << "    " << byteArray.size() << endl;
}

void Server::sockReadyRead() {
    this->socket = (QTcpSocket*)sender();
    QDataStream in(this->socket);
    in.setVersion(QDataStream::Qt_5_14);
    int tempAction;
    QString tempStr;
    in >> tempAction >> tempStr;
    if(tempAction == CodeAction::GoToDirectory) {
        if(tempStr.mid(tempStr.size() - 3, 2) == "..") {
            tempStr.remove(tempStr.size() - 3, 3);
            int i;
            for(i = tempStr.size() - 2; tempStr[i] != '/'; i--){
                if(i <= 0) break;
            }
            i++;
            tempStr.remove(i, tempStr.size() - i);
            QByteArray byteArray = NULL, tempByteArray = NULL;
            QDataStream tempout(&tempByteArray, QIODevice::WriteOnly);
            tempout.setVersion(QDataStream::Qt_5_14);
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out << CodeAction::InfoDirectory;
            wchar_t* temp = NULL;
            TCHAR filePath[MAX_PATH * 2];
            lstrcpy(filePath, qstrToWChar(temp, tempStr));
            WIN32_FIND_DATA fileInfo;
            HANDLE hFind;
            TCHAR * const lpLastChar = filePath + lstrlen(filePath);
            lstrcat(filePath, L"*");
            hFind = FindFirstFile(filePath, &fileInfo);
            *lpLastChar = '\0';
            if(INVALID_HANDLE_VALUE == hFind) return;
            do {
                QFileInfo temp(QString::fromWCharArray(filePath) + QString::fromWCharArray(fileInfo.cFileName));
                tempout << QString::fromWCharArray(filePath).size() << QString::fromWCharArray(filePath) <<
                           QString::fromWCharArray(fileInfo.cFileName).size() << QString::fromWCharArray(fileInfo.cFileName);
                if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) tempout << true;
                else tempout << false;
                tempout << temp.size() << temp.birthTime().date().day() << temp.birthTime().date().month() << temp.birthTime().date().year() <<
                           temp.lastModified().date().day() << temp.lastModified().date().month() << temp.lastModified().date().year();
                *lpLastChar = '\0';
            } while(FindNextFile(hFind, &fileInfo));
            free(temp);
            FindClose(hFind);
            out << tempByteArray.size();
            byteArray += tempByteArray;
            this->socket->write(byteArray);
            qDebug() << "Send client connect status - YES" << "    " << byteArray.size() << endl;
            return;
        }
        else if(tempStr[tempStr.size() - 2] == ".") {
            QByteArray byteArray = NULL, tempByteArray = NULL;
            QDataStream tempout(&tempByteArray, QIODevice::WriteOnly);
            tempout.setVersion(QDataStream::Qt_5_14);
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out << CodeAction::InfoDirectory;
            TCHAR currentDrive[] = L"A:/";
            DWORD disksMask = GetLogicalDrives();
            for(int i = 0; i < 26; i++) {
                if(disksMask & 1) {
                    tempout << 0 << QString("") << QString::fromWCharArray(currentDrive).size()
                            << QString::fromWCharArray(currentDrive) << true;
                    tempout << qint64(0) << 0 << 0 << 0 << 0 << 0 << 0;
                }
                disksMask >>= 1;
                currentDrive[0]++;
            }
            out << tempByteArray.size();
            byteArray += tempByteArray;
            this->socket->write(byteArray);
            qDebug() << "Send client connect status - YES" << "    " << byteArray.size() << endl;
            return;
        }
        else {
            QByteArray byteArray = NULL, tempByteArray = NULL;
            QDataStream tempout(&tempByteArray, QIODevice::WriteOnly);
            tempout.setVersion(QDataStream::Qt_5_14);
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out << CodeAction::InfoDirectory;
            wchar_t* temp = NULL;
            TCHAR filePath[MAX_PATH * 2];
            lstrcpy(filePath, qstrToWChar(temp, tempStr));
            WIN32_FIND_DATA fileInfo;
            HANDLE hFind;
            TCHAR * const lpLastChar = filePath + lstrlen(filePath);
            lstrcat(filePath, L"*");
            hFind = FindFirstFile(filePath, &fileInfo);
            *lpLastChar = '\0';
            if(INVALID_HANDLE_VALUE == hFind) return;
            do {
                QFileInfo temp(QString::fromWCharArray(filePath) + QString::fromWCharArray(fileInfo.cFileName));
                tempout << QString::fromWCharArray(filePath).size() << QString::fromWCharArray(filePath) <<
                           QString::fromWCharArray(fileInfo.cFileName).size() << QString::fromWCharArray(fileInfo.cFileName);
                if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) tempout << true;
                else tempout << false;
                tempout << temp.size() << temp.birthTime().date().day() << temp.birthTime().date().month() << temp.birthTime().date().year() <<
                           temp.lastModified().date().day() << temp.lastModified().date().month() << temp.lastModified().date().year();
                *lpLastChar = '\0';
            } while(FindNextFile(hFind, &fileInfo));
            free(temp);
            FindClose(hFind);
            out << tempByteArray.size();
            byteArray += tempByteArray;
            this->socket->write(byteArray);
            qDebug() << "Send client connect status - YES" << "    " << byteArray.size() << endl;
            return;
        }
    }
    else if(tempAction == CodeAction::UploadFileToClient) {
        tempStr.remove(tempStr.size() - 1, 1);
        QFile file(tempStr);
        if (file.open(QFile::ReadOnly)) {
            QByteArray byteArrayFile = file.readAll(), byteArray = NULL;
            file.close();
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out << CodeAction::ReadFileToSocket << byteArrayFile.size();
            while(true){
                qDebug() << "File sent, file size - " << byteArrayFile.size() << endl;
                byteArray += byteArrayFile.mid(0, 2048);
                byteArrayFile = byteArrayFile.remove(0, 2048);
                this->socket->write(byteArray);
                byteArray.clear();
                if(!byteArrayFile.size()) break;
                this->socket->waitForBytesWritten(100000);
            }
            qDebug() << "File sent, file size - " << byteArrayFile.size() << endl;
        }
        else {
            qDebug() << "File open error";
        }
    }
    else if (tempAction == CodeAction::UploadSearchFileToClient) {
        tempStr.remove(tempStr.size() - 1, 1);
        QFile file(tempStr);
        if (file.open(QFile::ReadOnly)) {
            QByteArray byteArrayFile = file.readAll(), byteArray = NULL;
            file.close();
            QDataStream out(&byteArray, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out << CodeAction::ReadSearchFileToSocket << byteArrayFile.size();
            while(true){
                qDebug() << "File sent, file size - " << byteArrayFile.size() << endl;
                byteArray += byteArrayFile.mid(0, 2048);
                byteArrayFile = byteArrayFile.remove(0, 2048);
                this->socket->write(byteArray);
                byteArray.clear();
                if(!byteArrayFile.size()) break;
                this->socket->waitForBytesWritten(100000);
            }
            qDebug() << "File sent, file size - " << byteArrayFile.size() << endl;
        }
        else {
            qDebug() << "File open error";
        }
    }
    else if (tempAction == CodeAction::SearchFile) {
        this->socket->readAll();
        this->tempListSearchInfo.clear();
        searchOnAllDrives(tempStr);
        QByteArray byteArray = NULL;;
        QDataStream out(&byteArray, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_14);
        if(this->tempListSearchInfo.size()) {
            out << CodeAction::SearchAllFile;
            for(int i = 0; i < this->tempListSearchInfo.size(); i++) {
                QFileInfo temp(tempListSearchInfo[i].filePath + tempListSearchInfo[i].fileName);
                out << tempListSearchInfo[i].sizeFilePath << tempListSearchInfo[i].filePath <<
                       tempListSearchInfo[i].sizeFileName << tempListSearchInfo[i].fileName <<
                       tempListSearchInfo[i].typeOfFile;
                out << temp.size() << temp.birthTime().date().day() << temp.birthTime().date().month() << temp.birthTime().date().year() <<
                       temp.lastModified().date().day() << temp.lastModified().date().month() << temp.lastModified().date().year();
            }
        }
        else out << CodeAction::NoFiles;
        this->socket->write(byteArray);
    }
}

void Server::sockDisconnected() {
    qDebug() << "Disconnect";
    this->socket->deleteLater();
}

void Server::sockConnected() {
    qDebug() << " Client connected";
}

void Server::processFoundFile(wchar_t* filePath, WIN32_FIND_DATA const * const fileInfo, wchar_t* fileName) {
    if(NULL != StrStrI(fileInfo->cFileName, fileName)) {
        QString tempFilePath = QString::fromWCharArray(filePath);
        int i;
        for (i = tempFilePath.size() - 1; tempFilePath[i] != '/'; i--) if(i <= 0) break;
        i++;
        tempFilePath.remove(i, tempFilePath.size() - i);
        FileInfo temp;
        temp.sizeFileName = QString::fromStdWString(fileInfo->cFileName).size();
        temp.fileName = QString::fromStdWString(fileInfo->cFileName);
        temp.sizeFilePath = tempFilePath.size();
        temp.filePath = tempFilePath;
        if(fileInfo->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) temp.typeOfFile = true;
        else temp.typeOfFile = false;
        this->tempListSearchInfo.push_back(temp);
        qDebug() << tempFilePath << endl << QString::fromStdWString(fileInfo->cFileName) << endl;
    }
}

void Server::recursiveSearch(wchar_t* filePath, wchar_t* fileName) {
    WIN32_FIND_DATA fileInfo;
    HANDLE hFind;
    TCHAR * const lpLastChar = filePath + lstrlen(filePath);
    lstrcat(filePath, L"*");
    hFind = FindFirstFile(filePath, &fileInfo);
    *lpLastChar = '\0';
    if(INVALID_HANDLE_VALUE == hFind) return;
    do {
        if((0 == lstrcmp(fileInfo.cFileName, L".")) || (0 == lstrcmp(fileInfo.cFileName, L".."))) continue;
        lstrcat(filePath, fileInfo.cFileName);
        if(fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            lstrcat(filePath, L"/");
            recursiveSearch(filePath, fileName);
        } else processFoundFile(filePath, &fileInfo, fileName);
        *lpLastChar = '\0';
    } while(FindNextFile(hFind, &fileInfo));
    FindClose(hFind);
}

void Server::searchOnAllDrives(QString qfileName) {
    wchar_t* fileName = NULL;
    TCHAR currentDrive[] = L"A:/";
    TCHAR filePath[MAX_PATH * 2];
    DWORD disksMask = GetLogicalDrives();
    for(int i = 0; i < 26; i++) {
        if(disksMask & 1) {
            lstrcpy(filePath, currentDrive);
            recursiveSearch(filePath, qstrToWChar(fileName, qfileName));
        }
        disksMask >>= 1;
        currentDrive[0]++;
    }
}

LPWSTR Server::qstrToWChar(wchar_t* fileName, QString qfileName) {
    fileName = new wchar_t[qfileName.size() + 1];
    qfileName.toWCharArray(fileName);
    fileName[qfileName.size()] = NULL;
    return fileName;
}
