#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
#include <vector>

using namespace std;

//int main()
//{
//    HANDLE _event_Time = CreateEvent(NULL, true, FALSE, ("Event_" + to_string(20202)).c_str());
//    HANDLE _event_Time2 = CreateEvent(NULL, true, FALSE, ("Event_1"));
//
//    STARTUPINFOA cif0;
//    ZeroMemory(&cif0, sizeof(STARTUPINFOA));
//    PROCESS_INFORMATION pi0;
//    CreateProcess("D:/À‡·˚ —œŒ¬Ã/Child_Laba2/Project1/Debug/Project1.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif0, &pi0);
//
//    Sleep(300);
//
//    int _count = 1;
//
//    vector<HANDLE> _vecEvent, _vecProcess;
//
//    HANDLE _event_11 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(1)).c_str());
//    HANDLE _event_22 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(2)).c_str());
//    HANDLE _event_33 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Event_" + to_string(3)).c_str());
//
//    const HANDLE _masHandle[3] = { _event_11 , _event_22 ,_event_33 };
//
//    STARTUPINFOA cif;
//    ZeroMemory(&cif, sizeof(STARTUPINFOA));
//    PROCESS_INFORMATION pi;
//
//    while (1)
//    {
//        cout << "Number of processes: " << _vecProcess.size() << endl;
//        switch (WaitForMultipleObjects(3, _masHandle, FALSE, 200))
//        {
//        case WAIT_FAILED:
//            cout << "Code error: " << GetLastError() << endl;
//            system("pause");
//            break;
//        case WAIT_TIMEOUT:
//            for (int i = 0; i < _vecEvent.size(); i++)
//            {
//                if (WAIT_OBJECT_0 == WaitForSingleObject(_event_Time, 2000)) {
//                    //ResetEvent(_event_Time);
//                    WaitForSingleObject(_event_Time2, 200);
//                    //ResetEvent(_event_Time2);
//                    //break;
//                }
//                Sleep(1);
//                PulseEvent(_vecEvent[i]);
//                WaitForSingleObject(_vecEvent[i], INFINITE); 
//            }
//            break;
//        case WAIT_OBJECT_0 + 0:
//            if (!CreateProcessA("D:/À‡·˚ —œŒ¬Ã/Second_Child_Laba2/Project1/Debug/Project1.exe",
//                const_cast<LPSTR>(("String - " + to_string(_count)).c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi))
//                cout << "Process creation error!!!";
//            else
//            {
//                _vecProcess.push_back(pi.hProcess);
//                _vecEvent.push_back(CreateEvent(NULL, true, false, ("Event_" + to_string(pi.dwProcessId)).c_str()));
//                _count++;
//                ResetEvent(_masHandle[0]);
//                PulseEvent(_event_Time2);
//            }
//            break;
//        case WAIT_OBJECT_0 + 1:
//            if (!_vecProcess.empty())
//            {
//                _count--;
//                TerminateProcess(_vecProcess.back(), 0);
//                CloseHandle(_vecEvent.back());
//                CloseHandle(_vecProcess.back());
//                _vecEvent.pop_back();
//                _vecProcess.pop_back();
//                ResetEvent(_masHandle[1]);
//            }
//            break;
//        case WAIT_OBJECT_0 + 2:
//            while (!_vecProcess.empty())
//            {
//                TerminateProcess(_vecProcess.back(), 0);
//                CloseHandle(_vecEvent.back());
//                CloseHandle(_vecProcess.back());
//                _vecEvent.pop_back();
//                _vecProcess.pop_back();
//                ResetEvent(_masHandle[3]);
//            }
//            return 1;
//        }
//        system("CLS");
//    }
//    return 0;
//}

int main()
{
	HANDLE _event_Time = CreateEvent(NULL, true, FALSE, ("Ev_" + to_string(20202)).c_str());

	STARTUPINFOA cif0;
	ZeroMemory(&cif0, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION pi0;
	CreateProcess("D:/À‡·˚ —œŒ¬Ã/Child_Laba2/Project1/Debug/Project1.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &cif0, &pi0);

	Sleep(300);

	int _count = 1;

	vector<HANDLE> _vecEvent, _vecProcess;

	HANDLE _event_11 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Ev_" + to_string(1)).c_str());
	HANDLE _event_22 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Ev_" + to_string(2)).c_str());
	HANDLE _event_33 = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Ev_" + to_string(3)).c_str());

	const HANDLE _masHandle[3] = { _event_11 , _event_22 ,_event_33 };

	STARTUPINFOA cif;
	ZeroMemory(&cif, sizeof(STARTUPINFOA));
	PROCESS_INFORMATION pi;
	int i = 0;
	bool ch = true;

	while (ch) {
		switch (WaitForMultipleObjects(3, _masHandle, FALSE, 1000)) {
		case WAIT_FAILED:
			cout << "Code error: " << GetLastError() << endl;
			system("pause");
			break;
		case WAIT_OBJECT_0 + 0:
			if (!CreateProcessA("D:/À‡·˚ —œŒ¬Ã/Second_Child_Laba2/Project1/Debug/Project1.exe", const_cast<LPSTR>(("String - " + to_string(_count)).c_str()), NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi))
				cout << "Process creation error!!!";
			else {
				_vecProcess.push_back(pi.hProcess);
				_vecEvent.push_back(CreateEvent(NULL, true, false, ("Ev_" + to_string(pi.dwProcessId)).c_str()));
				_count++;
				ResetEvent(_masHandle[0]);
				Sleep(100);
			}
			break;
		case WAIT_OBJECT_0 + 1:
			if (!_vecProcess.empty()) {
				_count--;
				TerminateProcess(_vecProcess.back(), 0);
				CloseHandle(_vecEvent.back());
				CloseHandle(_vecProcess.back());
				_vecEvent.pop_back();
				_vecProcess.pop_back();
				ResetEvent(_masHandle[1]);
			}
			break;
		case WAIT_OBJECT_0 + 2:
			while (!_vecProcess.empty()) {
				TerminateProcess(_vecProcess.back(), 0);
				CloseHandle(_vecEvent.back());
				CloseHandle(_vecProcess.back());
				_vecEvent.pop_back();
				_vecProcess.pop_back();
				ResetEvent(_masHandle[2]);
			}
			return 1;
		}
		if (_vecProcess.size() > 0) {
			PulseEvent(_vecEvent[i]);
			WaitForSingleObject(_vecEvent[i], INFINITE);
			i++;
			if (_vecProcess.size() == i) i = 0;
		}
	}
	return 0;
}

//int main()
//{
//	int _count = 1;
//
//	vector<HANDLE> processVector, eventVector;
//	STARTUPINFO cif;
//	ZeroMemory(&cif, sizeof(STARTUPINFO));
//	PROCESS_INFORMATION pi;
//
//	while (true)
//	{
//		for (int i = 0; i < eventVector.size(); i++)
//		{
//			ReleaseSemaphore(eventVector[i], 1, NULL);
//			WaitForSingleObject(eventVector[i], INFINITE);
//		}
//		while (_kbhit())
//		{
//			switch (_getch())
//			{
//			case '+':
//				if (CreateProcess("D:/À‡·˚ —œŒ¬Ã/Second_Child_Laba2/Project1/Debug/Project1.exe", const_cast<LPSTR>(("String - " + to_string(_count)).c_str()), NULL, NULL, NULL, NULL, NULL, NULL, &cif, &pi))
//				{
//	
//					eventVector.push_back(CreateSemaphore(NULL, 0, 10, (to_string(pi.dwProcessId) + "event").c_str()));
//					processVector.push_back(pi.hProcess);
//					WaitForSingleObject(eventVector.back(), INFINITE);
//					_count++;
//				}
//				else {
//					cout << "Cant create process" << endl;
//					_getch();
//				}
//				break;
//			case '-':
//				if (!processVector.empty())
//				{
//					_count--;
//					TerminateProcess(processVector.back(), 0);
//					CloseHandle(eventVector.back());
//					CloseHandle(processVector.back());
//					eventVector.pop_back();
//					processVector.pop_back();
//				}
//				break;
//			case 'q':
//				while (!processVector.empty())
//				{
//					_count--;
//					TerminateProcess(processVector.back(), 0);
//					CloseHandle(eventVector.back());
//					CloseHandle(processVector.back());
//					eventVector.pop_back();
//					processVector.pop_back();
//				}
//				ExitProcess(0);
//				break;
//			}
//		}
//	}
//}

int main(){
	vector<HANDLE> processVector, eventVector;
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	int _count = 1;
	while (true){
		//cout << "Current processes count: " << processVector.size() << endl;
		for (int i = 0; i < eventVector.size(); i++){
			PulseEvent(eventVector[i]);
			WaitForSingleObject(eventVector[i], INFINITE);
		}
		while (_kbhit()){
			switch (_getch()){
			case '+':
				if (CreateProcess("D:/À‡·˚ —œŒ¬Ã/Second_Child_Laba2/Project1/Debug/Project1.exe", const_cast<LPSTR>(("String - " + to_string(_count)).c_str()), NULL, NULL, NULL, NULL, NULL, NULL, &cif, &pi)) {		
					eventVector.push_back(CreateEvent(NULL, true, false, (to_string(pi.dwProcessId) + "event").c_str()));
					processVector.push_back(pi.hProcess);
					WaitForSingleObject(eventVector.back(), INFINITE);
					_count++;
				}
				else cout << "Process creation error!!!";
				break;
			case '-':
				if (!processVector.empty()){
					_count--;
					TerminateProcess(processVector.back(), 0);
					CloseHandle(eventVector.back());
					CloseHandle(processVector.back());
					eventVector.pop_back();
					processVector.pop_back();
				}
				break;
			case 'q':
				while (!processVector.empty()){
					_count--;
					TerminateProcess(processVector.back(), 0);
					CloseHandle(eventVector.back());
					CloseHandle(processVector.back());
					eventVector.pop_back();
					processVector.pop_back();
				}
				ExitProcess(0);
				break;
			}
		}
	}
}