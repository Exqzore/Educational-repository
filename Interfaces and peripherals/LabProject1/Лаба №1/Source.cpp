#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <regex>

#pragma comment( lib, "setupapi.lib" )

using namespace std;

string searchID(string tmpStr, string reg)
{
    regex regID = regex(string(reg));
    smatch match;
    regex_search(tmpStr, match, regID);
    if(match.size()) return string(match[0]).substr(4, 4);
    return "";
}

int main()
{
    system("chcp 1251");
    system("cls");

    /////////////Registry/////////////
    HKEY hKey;
    wstring pciPath(L"SYSTEM\\CurrentControlSet\\Enum\\PCI");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, pciPath.c_str(), NULL, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
    {
        wchar_t lpName[MAX_PATH];
        DWORD dwIndex = 0, lpcchName = MAX_PATH;
        while (RegEnumKeyEx(hKey, dwIndex++, lpName, &lpcchName, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
        {
            wstring wstr(lpName);
            HKEY hKeyDevice;
            string str(wstr.begin(), wstr.end());            
            cout << dwIndex << ") VendorID - " << searchID(str, "VEN_.{4}") << ", DevicedID - "<< searchID(str, "DEV_.{4}") << endl;
            lpcchName = MAX_PATH;
        }
    }
    else cout << endl << "Error!!!" << endl;

    RegCloseKey(hKey);
    //////////endRegistry////////////

    cout << endl << endl << endl;

    //////////SetupDevice////////////
    HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, L"PCI", NULL, DIGCF_ALLCLASSES );

    if (hDevInfo == INVALID_HANDLE_VALUE) cout << "Error: " << GetLastError() << endl;
    else
    {
        SP_DEVINFO_DATA hDevInfoData;
        hDevInfoData.cbSize = sizeof(hDevInfoData);
        DWORD hMemberIndex = 0, size = 0;
        BYTE buffer[MAX_SIZE_SECURITY_ID], bufferName[MAX_SIZE_SECURITY_ID], buf[MAX_SIZE_SECURITY_ID];

        while (SetupDiEnumDeviceInfo(hDevInfo, hMemberIndex++, &hDevInfoData))
        {
            SetupDiGetDeviceRegistryPropertyA(hDevInfo, &hDevInfoData, SPDRP_HARDWAREID, NULL, buffer, MAX_SIZE_SECURITY_ID, &size);
            string tmpStr = (char*)buffer;
            SetupDiGetDeviceRegistryPropertyA(hDevInfo, &hDevInfoData, SPDRP_DEVICEDESC, NULL, bufferName, MAX_SIZE_SECURITY_ID, &size);
            string tmpStrName = (char*)bufferName;
            SetupDiGetDeviceRegistryPropertyA(hDevInfo, &hDevInfoData, SPDRP_MFG, NULL, buf, MAX_SIZE_SECURITY_ID, &size);
            string tm = (char*)buf;
            cout << hMemberIndex << ") VendorID - " << searchID(tmpStr, "VEN_.{4}") << ", DevicedID - " << searchID(tmpStr, "DEV_.{4}") << "  " << buf << "   |   " << tmpStrName << endl;
        }
        if (!SetupDiDestroyDeviceInfoList(hDevInfo)) cout << GetLastError() << endl;
    }
    //////////endSetupDevice////////////

	return 0;
}