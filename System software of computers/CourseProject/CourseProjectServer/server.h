#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDataStream>

#include <windows.h>
#include <shlwapi.h>

#include "CodeAction.h"
#include "FileInfo.h"

class Server: public QTcpServer {
    Q_OBJECT
public:
    Server();
    ~Server();
    void startServer();
    QTcpSocket* socket;

public slots:    
    void incomingConnection(qintptr handle) override;
    void sockReadyRead();
    void sockDisconnected();
    void sockConnected();

private:
    void searchOnAllDrives(QString);
    void processFoundFile(wchar_t*, WIN32_FIND_DATA const * const, wchar_t*);
    void recursiveSearch(wchar_t*, wchar_t*);
    LPWSTR qstrToWChar(wchar_t*, QString);
    QList<FileInfo> tempListSearchInfo;
};

#endif // SERVER_H
