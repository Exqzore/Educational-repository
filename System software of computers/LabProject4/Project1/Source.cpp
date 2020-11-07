#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

vector<HANDLE> _vecThread;
vector<LPCRITICAL_SECTION> _vecCS, _vecCheckCS;

int _count = 1;

struct Str
{
	string str;
	LPCRITICAL_SECTION CS;
	LPCRITICAL_SECTION checkCS;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
	Str _tempStr = *((Str*)lpParam);
	while (true) {
		EnterCriticalSection(_tempStr.CS);
		if (TryEnterCriticalSection(_tempStr.checkCS)) return 0;
		for (char a : _tempStr.str) {
			cout << a;
			Sleep(50);
		}
		cout << endl;
		LeaveCriticalSection(_tempStr.CS);
		Sleep(100);
	}
	return 0;
}

void fanc() {
	switch (_getch())
	{
	case '+':
	{
		Str* str = new Str;
		str->str = "String - " + to_string(_count);
		LPCRITICAL_SECTION _temp1 = new CRITICAL_SECTION;
		InitializeCriticalSection(_temp1);
		EnterCriticalSection(_temp1);
		_vecCS.push_back(_temp1);
		str->CS = _temp1;
		LPCRITICAL_SECTION _temp2 = new CRITICAL_SECTION;
		InitializeCriticalSection(_temp2);
		EnterCriticalSection(_temp2);
		_vecCheckCS.push_back(_temp2);
		str->checkCS = _temp2;
		_vecThread.push_back(CreateThread(NULL, NULL, ThreadProc, str, NULL, NULL));
		_count++;
		break;
	}
	case '-':
	{
		if (!_vecThread.empty()) {
			_count--;
			LeaveCriticalSection(_vecCheckCS.back());
			delete _vecCheckCS.back();
			_vecCheckCS.pop_back();
			LeaveCriticalSection(_vecCS.back());
			delete _vecCS.back();
			_vecCS.pop_back();
			CloseHandle(_vecThread.back());
			_vecThread.pop_back();
		}
		break;
	}
	case 'q':
	{
		while (!_vecThread.empty()) {
			_count--;
			LeaveCriticalSection(_vecCheckCS.back());
			delete _vecCheckCS.back();
			_vecCheckCS.pop_back();
			LeaveCriticalSection(_vecCS.back());
			delete _vecCS.back();
			_vecCS.pop_back();
			CloseHandle(_vecThread.back());
			_vecThread.pop_back();
		}
		exit(0);
	}
	}
}

int main() {
	while (true) {
		//cout << "Count: " << _vecThread.size() << endl;
		for (int i = 0; i < _vecThread.size(); i++){
			LeaveCriticalSection(_vecCS[i]);
			Sleep(50);
			EnterCriticalSection(_vecCS[i]);
			while (_kbhit()) {
				fanc();
			}
		}
		while (_kbhit()) {
			fanc();
		}
		//system("cls");
	}
	return 0;
}