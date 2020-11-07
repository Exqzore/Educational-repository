#pragma hdrstop

#include <Windows.h>
#include <setupapi.h>
#include <iostream>
#include <wdmguid.h>
#include <devguid.h>
#include <ctime>
#include <comdef.h> 
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#include <sstream>

#pragma comment(lib, "setupapi.lib")

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

#define FRAME_RATE 10
#define FRAME_TIME 1000 / FRAME_RATE

unsigned char commLen = 0;
char commBuffer[500] = { 0 };
static string SAVE_PATH = "frames/";

VideoCapture webcam;
Mat frame;

HHOOK keyboardHook;

string makeName()
{
	time_t now = time(0);
	tm* ltm = new tm;
	localtime_s(ltm, &now);
	stringstream sstream;
	sstream << ltm->tm_year + 1900 << '_' << ltm->tm_mon + 1 << '_' << ltm->tm_mday << '_'
		<< ltm->tm_hour << '_' << ltm->tm_min << '_' << ltm->tm_sec;
	return sstream.str();
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		commBuffer[commLen++] = (char)key->vkCode;
		//cout << commBuffer << endl;
		if (strncmp("EXIT", commBuffer, (commLen <= 4 ? commLen : 4)) &&
			strncmp("HIDE", commBuffer, (commLen <= 4 ? commLen : 4)) &&
			strncmp("SHOW", commBuffer, (commLen <= 4 ? commLen : 4)) &&
			strncmp("PHOTO", commBuffer, (commLen <= 5 ? commLen : 5)) &&
			strncmp("VIDEO", commBuffer, (commLen <= 5 ? commLen : 5)))
		{
			commLen = 0;
		}
		else
		{
			if (commLen == 4 && !strncmp("EXIT", commBuffer, 4))
			{
				webcam.release();
				PostQuitMessage(1);
			}
			else if (commLen == 4 && !strncmp("HIDE", commBuffer, 4))
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
				memset(commBuffer, 0, commLen);
				commLen = 0;
			}
			else if (commLen == 4 && !strncmp("SHOW", commBuffer, 4))
			{
				ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
				memset(commBuffer, 0, commLen);
				commLen = 0;
			}
			else if (commLen == 5 && !strncmp("PHOTO", commBuffer, 5))
			{				
				if (!webcam.open(1 + CAP_DSHOW)) return 1;
				webcam.set(CAP_PROP_FRAME_WIDTH, 1280);
				webcam.set(CAP_PROP_FRAME_HEIGHT, 720);
				webcam >> frame;
				imwrite(SAVE_PATH + "photo_" + makeName() + ".png", frame);
				webcam.release();
				memset(commBuffer, 0, commLen);
				commLen = 0;
			}
			else if (commLen == 8 && !strncmp("VIDEO", commBuffer, 5))
			{
				string numder = "";
				for (int i = 5; i < 8; i++)
				{
					numder += commBuffer[i];
					if (commBuffer[i] < '0' || commBuffer[i] > '9')
					{
						memset(commBuffer, 0, 8);
						commLen = 0;
						return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
					}
				}
				int dl = stoi(numder);
				if (dl == 0)
				{
					memset(commBuffer, 0, 8);
					commLen = 0;
					return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
				}
				if (!webcam.open(1 + CAP_DSHOW)) return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
				webcam.set(CAP_PROP_FRAME_WIDTH, 1280);
				webcam.set(CAP_PROP_FRAME_HEIGHT, 720);
				VideoWriter videoWriter(SAVE_PATH + "video_" + makeName() + ".avi", VideoWriter::fourcc('X', 'V', 'I', 'D'), FRAME_RATE, Size(1280, 720), true);
				long long curFrame = 0;
				auto prevClock = clock();
				while (curFrame < FRAME_RATE * dl)
				{
					while (clock() - prevClock < FRAME_TIME);
					prevClock = clock();
					curFrame++;
					webcam >> frame;
					videoWriter.write(frame);
				}
				webcam.release();
				memset(commBuffer, 0, commLen);
				commLen = 0;
			}
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void printCameraInfo()
{
	system("chcp 1251");
	system("cls");
	SP_DEVINFO_DATA DeviceInfoData = { 0 };
	HDEVINFO DeviceInfoSet = SetupDiGetClassDevsA(&GUID_DEVCLASS_CAMERA, "USB", NULL, DIGCF_PRESENT);
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	SetupDiEnumDeviceInfo(DeviceInfoSet, 0, &DeviceInfoData);
	char deviceName[256], deviceMfg[256];
	SetupDiGetDeviceRegistryPropertyA(DeviceInfoSet, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)deviceName, sizeof(deviceName), 0);
	SetupDiGetDeviceRegistryPropertyA(DeviceInfoSet, &DeviceInfoData, SPDRP_MFG, NULL, (PBYTE)deviceMfg, sizeof(deviceMfg), 0);
	cout << "Vendor: " << deviceMfg << endl << "Name: " << deviceName << endl;
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
}

int main()
{
	printCameraInfo();
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	MSG msg{ 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0);
	UnhookWindowsHookEx(keyboardHook);
	PostMessage(GetConsoleWindow(), WM_QUIT, 0, 0);
}