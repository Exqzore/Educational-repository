#ifndef PCH_H
#define PCH_H
#define EXPORT extern "C" __declspec(dllexport)

#include "framework.h"

struct tempPipe {
	HANDLE* event;
	HANDLE* pipe;
};

EXPORT DWORD WINAPI ReaderFileFunc(void*);
EXPORT DWORD WINAPI WriterFileFunc(void*);

#endif