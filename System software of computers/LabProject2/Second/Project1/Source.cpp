#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>

using namespace std;

int main()
{
	cout << "Action selection:" << endl << "1)New process: +" << endl << "2)Delete process: -" << endl << "3)Exit: q" << endl;

	HANDLE _event_1 = CreateEvent(NULL, true, FALSE, ("Ev_" + to_string(1)).c_str());
	HANDLE _event_2 = CreateEvent(NULL, true, FALSE, ("Ev_" + to_string(2)).c_str());
	HANDLE _event_3 = CreateEvent(NULL, true, FALSE, ("Ev_" + to_string(3)).c_str());

	HANDLE _event_Time = OpenEvent(EVENT_ALL_ACCESS, FALSE, ("Ev_" + to_string(20202)).c_str());

	while (1)
	{
		switch (_getch())
		{
		case '+':
			SetEvent(_event_1);
			SetEvent(_event_Time);
			break;
		case '-':
			SetEvent(_event_2);
			SetEvent(_event_Time);
			break;
		case 'q':
			SetEvent(_event_3);
			SetEvent(_event_Time);
			return 1;
		}
	}
	return 0;
}