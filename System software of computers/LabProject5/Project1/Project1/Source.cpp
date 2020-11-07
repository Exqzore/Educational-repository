#include <windows.h>
#include <iostream>
#include <filesystem>

#define SIZEBUF 1024

using namespace std;

struct tempPipe {
	HANDLE* event;
	HANDLE* pipe;
};

typedef DWORD(WINAPI* dllFunction)(void*);
dllFunction ReaderFileFunc;
dllFunction WriterFileFunc;

int main()
{
	HINSTANCE _dllLib = LoadLibrary("WaRDLL");
	ReaderFileFunc = (dllFunction)GetProcAddress(_dllLib, "_ReaderFileFunc@4");
	WriterFileFunc = (dllFunction)GetProcAddress(_dllLib, "_WriterFileFunc@4");	
	HANDLE _readPipe, _writePipe, _eventPipe = CreateEventA(NULL, TRUE, FALSE, NULL);
	tempPipe _pipe1;
	_pipe1.event = &_eventPipe;
	_pipe1.pipe = &_readPipe;
	tempPipe _pipe2;
	_pipe2.event = &_eventPipe;
	_pipe2.pipe = &_writePipe;
	CreatePipe(&_readPipe, &_writePipe, NULL, SIZEBUF);
	HANDLE _writerThread = CreateThread(NULL, NULL, WriterFileFunc, &_pipe1, NULL, NULL);
	HANDLE _readerThread = CreateThread(NULL, NULL, ReaderFileFunc, &_pipe2, NULL, NULL);
	WaitForSingleObject(_readerThread, INFINITE);
	FreeLibrary(_dllLib);
	return 0;
}