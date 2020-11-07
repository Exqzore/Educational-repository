#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	time_t t1;
	t1 = time(0);
	cout << ctime(&t1) << endl;
	system("pause");
	return 0;
}			