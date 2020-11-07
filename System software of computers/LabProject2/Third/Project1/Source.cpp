//#include <iostream>
//#include <Windows.h>
//#include <string>
//
//using namespace std;
//
//int main()
//{
//	HANDLE _event = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Ev_" + to_string(GetCurrentProcessId())).c_str());
//	string str = to_string(GetCurrentProcessId()), message = GetCommandLineA();
//	while (1)
//	{
//		WaitForSingleObject(_event, INFINITE);
//		for (char a : message + "   id " + str)
//		{
//			cout << a;
//			Sleep(50);
//		}
//		PulseEvent(_event);
//		cout << endl;
//	}
//	return 0;
//}

#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//int main()
//{
//	HANDLE _event = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, (to_string(GetCurrentProcessId()) + "event").c_str());
//	ReleaseSemaphore(_event, 1, NULL);
//	string str = to_string(GetCurrentProcessId()),
//		message = GetCommandLine();
//	while (true)
//	{
//		WaitForSingleObject(_event, INFINITE);
//		for (char a : message + " " + str)
//		{
//			cout << a;
//			Sleep(30);
//		}
//		ReleaseSemaphore(_event, 1, NULL);
//		cout << endl;
//	}
//	return 0;
//}

int main()
{
	HANDLE _event = OpenEvent(EVENT_ALL_ACCESS, FALSE, (to_string(GetCurrentProcessId())+"event").c_str());
	//HANDLE _event = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, (to_string(GetCurrentProcessId()) + "event").c_str());
	PulseEvent(_event);
	//WaitForSingleObject(_event, 0);
	ReleaseSemaphore(_event, 1, NULL);
	string str = to_string(GetCurrentProcessId()),
		message = GetCommandLine();
	while (true)
	{
		WaitForSingleObject(_event, INFINITE);
		for (char a : message + " " + str)
		{
			cout << a;
			Sleep(30);
		}
		PulseEvent(_event);
		ReleaseSemaphore(_event, 1, NULL);
		cout << endl;
	}
	return 0;
}