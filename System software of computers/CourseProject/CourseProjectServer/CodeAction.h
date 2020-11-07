#ifndef CODEACTION_H
#define CODEACTION_H

enum CodeAction {
    UploadFileToClient = 1000,
    GoToDirectory = 1001,
    FileOpenError = 1002,
    ReadFileToSocket = 1003,
    InfoDirectory = 1004,
    SearchFile = 1005,
    SearchAllFile = 1006,
    UploadSearchFileToClient = 1007,
    ReadSearchFileToSocket = 1008,
    NoFiles = 1009
};

#endif // CODEACTION_H
