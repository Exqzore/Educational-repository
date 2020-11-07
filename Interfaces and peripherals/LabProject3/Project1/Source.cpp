#include <iostream>
#include <Windows.h>
#include <Setupapi.h>
#include <Poclass.h>
#include <batclass.h>
#include <devguid.h>
#include <string>

#pragma comment (lib, "setupapi.lib")

using namespace std;

bool BatteryHim(string* type)
{
	HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = { 0 };
	DeviceInterfaceData.cbSize = sizeof(SP_DEVINFO_DATA);
	SetupDiEnumDeviceInterfaces(DeviceInfoSet, NULL, &GUID_DEVCLASS_BATTERY, 0, &DeviceInterfaceData);
	DWORD cbRequired = 0;
	SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, NULL, NULL, &cbRequired, NULL);
	PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
	pdidd->cbSize = sizeof(*pdidd);
	SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, pdidd, cbRequired, &cbRequired, NULL);
	HANDLE hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BATTERY_QUERY_INFORMATION BatteryQueryInformation = { 0 };
	DWORD bytesWait = 0;
	DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, &bytesWait, sizeof(bytesWait), &BatteryQueryInformation.BatteryTag, sizeof(BatteryQueryInformation.BatteryTag), NULL, NULL);
	BATTERY_INFORMATION BatteryInfo = { 0 };
	BatteryQueryInformation.InformationLevel = BatteryInformation;
	DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION, &BatteryQueryInformation, sizeof(BatteryQueryInformation), &BatteryInfo, sizeof(BatteryInfo), NULL, NULL);
	*type = (char*)BatteryInfo.Chemistry;
	*type = (*type).substr(0, 4);
	CloseHandle(hBattery);
	return true;
}

int main()
{
	string type = "";
	BatteryHim(&type);
	while (true)
	{
		system("cls");

		SYSTEM_POWER_STATUS batteryDescriptor;
		GetSystemPowerStatus(&batteryDescriptor);

		if (batteryDescriptor.ACLineStatus == 0) cout << "ACLineStatus: Offline" << endl;
		else if (batteryDescriptor.ACLineStatus == 1) cout << "ACLineStatus: Online" << endl;
		else cout << "ACLineStatus: Unknown status" << endl;

		if (batteryDescriptor.BatteryFlag == 0) cout << "BatteryFlag: Battery is not being charged and the battery capacity is between low and high" << endl;
		else if (batteryDescriptor.BatteryFlag == 1) cout << "BatteryFlag: High-the battery capacity is at more than 66 percent" << endl;
		else if (batteryDescriptor.BatteryFlag == 2) cout << "BatteryFlag: Low-the battery capacity is at less than 33 percent" << endl;
		else if (batteryDescriptor.BatteryFlag == 4) cout << "BatteryFlag: Critical—the battery capacity is at less than five percent" << endl;
		else if (batteryDescriptor.BatteryFlag == 8) cout << "BatteryFlag: Charging" << endl;
		else if (batteryDescriptor.BatteryFlag == 128) cout << "BatteryFlag: No system battery" << endl;
		else cout << "BatteryFlag: Unknown status—unable to read the battery flag information" << endl;

		cout << "BatteryLifePercent: " << (batteryDescriptor.BatteryLifePercent == 255 ? "Status is unknown" : to_string(batteryDescriptor.BatteryLifePercent) + " %") << endl;

		if (batteryDescriptor.SystemStatusFlag == 0) cout << "SystemStatusFlag: Battery saver is off" << endl;
		else cout << "SystemStatusFlag: Battery saver on. Save energy where possible" << endl;

		if (batteryDescriptor.BatteryLifeTime == -1) cout << "BatteryLifeTime: Remaining seconds are unknown or if the device is connected to AC power" << endl;
		else cout << "BatteryLifeTime: The number of minutes of battery life remaining: " << batteryDescriptor.BatteryLifeTime / 60 << endl;

		if (batteryDescriptor.BatteryFullLifeTime == -1) cout << "BatteryFullLifeTime: Full battery lifetime is unknown or if the device is connected to AC power" << endl;
		else cout << "BatteryFullLifeTime: The number of minutes of battery life when at full charge: " << batteryDescriptor.BatteryFullLifeTime / 60 << endl;

		cout << "Battery type by chemical composition: " << (type == "" ? "Unknown" : type) << endl;

		system("pause");
	}
	return 0;
}