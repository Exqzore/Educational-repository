#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
#include <vector>

using namespace std;

int main()
{
	HANDLE _semaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, "MySemaphore");
	ReleaseSemaphore(_semaphore, 1, NULL);
	HANDLE _pipe = CreateFileA("\\\\.\\pipe\\Channel", PIPE_ACCESS_DUPLEX, 0, NULL, OPEN_EXISTING, 0, NULL);
	char _str[80];
	int _size;
	string _tempStr;
	while (true) {
		WaitForSingleObject(_semaphore, INFINITE);
		ReadFile(_pipe, _str, 80, (LPDWORD)&_size, NULL);
		_str[_size] = 0;
		_tempStr = _str;
		if (_tempStr == "exit") exit(0);
		cout << _tempStr << endl;
		ReleaseSemaphore(_semaphore, 1, NULL);
	}
	return 0;
}