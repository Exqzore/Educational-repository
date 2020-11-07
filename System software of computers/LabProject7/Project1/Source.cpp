#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "fs.h"

string editString(string str) {
	DWORD readRecords;
	HANDLE stdHundle = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD* inputData = new INPUT_RECORD[str.size()];
	for (int i = 0; i < str.size(); i++) {
		inputData[i].EventType = KEY_EVENT;
		inputData[i].Event.KeyEvent.uChar.UnicodeChar = str[i];
	}
	WriteConsoleInput(stdHundle, inputData, str.size(), &readRecords);
	string a;
	getline(cin, a);
	return a;
}

int main()
{
	VirtualFS fs("virtualFs.fs");
	unsigned short currrentChoice = 0;
	unsigned short maxChoice = 0;
	while (true)  {
		vector<string> dirNames = fs.getCurrentDirNames();
		vector<string> fileNames = fs.getCurrentFileNames();
		maxChoice = dirNames.size() + fileNames.size() - 1;
		cout << "N - create dir | F - create file | D - delete | R - rename | E - exit\n";
		cout << "Total memory usage: " << fs.getUsedMemory() * BLOCK_SIZE << " (" << fs.getUsedMemory() << " blocks)\n\n";
		for (auto it : fs.getCurrentPath()) cout << it << '/';
		cout << '\n';
		for (unsigned short i = 0; i < dirNames.size(); i++)
			cout << (i == currrentChoice ? " > " : "   ") << dirNames[i] << '/' << (i == currrentChoice ? " < " : "   ") << '\n';
		for (unsigned short i = 0; i < fileNames.size(); i++)
			cout << (i + dirNames.size() == currrentChoice ? " > " : "   ") << fileNames[i] << (i + dirNames.size() == currrentChoice ? " < " : "   ") << '\n';
		switch (_getch())
		{
		case 0:case 224:
		{
			switch (_getch())
			{
			case 72:
				if (currrentChoice) currrentChoice--;
				break;
			case 80:
				if (currrentChoice < maxChoice) currrentChoice++;
				break;
			}
			break;
		}
		case 'n':
		{
			system("cls");
			cout << "Enter name for new folder: ";
			string name;
			cin >> name;
			fs.createDir(name);
			break;
		}
		case 'f':
		{
			system("cls");
			cout << "Enter name for new file: ";
			string name;
			cin >> name;
			fs.createFile(name);
			break;
		}
		case 'd':
		{
			if (currrentChoice < dirNames.size()) fs.deleteDir(dirNames[currrentChoice]);
			else fs.deleteFile(fileNames[currrentChoice - dirNames.size()]);
			currrentChoice = 0;
			break;
		}
		case 'r':
		{
			system("cls");
			cout << "Enter new name: ";
			string newName;
			cin >> newName;
			if (currrentChoice < dirNames.size()) fs.renameDir(dirNames[currrentChoice], newName);
			else fs.renameFile(fileNames[currrentChoice - dirNames.size()], newName);
			break;
		}
		case 'e':
		{
			return 0;
		}
		case 13:
		{
			if (currrentChoice < dirNames.size()) {
				fs.enterDir(dirNames[currrentChoice]);
				currrentChoice = 0;
			}
			else fs.editFile(fileNames[currrentChoice - dirNames.size()], editString(fs.readFile(fileNames[currrentChoice - dirNames.size()])));
			break;
		}
		}
		system("cls");
	}
	return 0;
}