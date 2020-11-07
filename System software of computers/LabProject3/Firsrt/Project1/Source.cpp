#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
#include <vector>

using namespace std;

int main()
{
    string _str;
    HANDLE _pipe = CreateNamedPipeA("\\\\.\\pipe\\Channel", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_UNLIMITED_INSTANCES, 80, 80, NULL, NULL);
    HANDLE _semaphore = CreateSemaphoreA(NULL, 0, 1, "MySemaphore");
    STARTUPINFOA _cif;
    ZeroMemory(&_cif, sizeof(STARTUPINFOA));
    PROCESS_INFORMATION _pi;
    CreateProcessA("D:/À‡·˚ —œŒ¬Ã/Child_Laba3/Project1/Debug/Project1.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &_cif, &_pi);
    WaitForSingleObject(_semaphore, INFINITE);
    while (true) {
        do {
            getline(cin, _str);
        } while (_str.size() == 0);
        WriteFile(_pipe, _str.c_str(), _str.size(), NULL, NULL);
        ReleaseSemaphore(_semaphore, 1, NULL);
        if (_str == "exit") break;
        WaitForSingleObject(_semaphore, INFINITE);
    }
	return 0;
}