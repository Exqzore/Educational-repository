#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>

struct FileInfo {
    int sizeFilePath = NULL;
    QString filePath = NULL;
    int sizeFileName = NULL;
    QString fileName = NULL;
    int fileAtribute = NULL;
    bool typeOfFile = NULL;
    qint64 sizeFile = NULL;
    int dayDate = NULL;
    int monthDate = NULL;
    int yearDate = NULL;
    int dayLastModifiedDate = NULL;
    int monthLastModifiedDate = NULL;
    int yearLastModifiedDate = NULL;
};

#endif // FILEINFO_H
