#include <iostream>
#include <Windows.h>
#include <ntddscsi.h>
#include "DiskDrive.h"
#include "Ata.h"

using namespace std;

#define MEGABYTE 1048576

void show(DiskDrive diskDrive, string diskName)
{
	cout << "||||||||||||||||||||||||||||||||||-" << diskName << "-||||||||||||||||||||||||||||||||||" << endl;
	cout << "Модель устройства: Unavailable" << endl;
	cout << "Модель устройства: " << diskDrive.ProductId << endl << "Серийный номер устройства: " << diskDrive.SerialNumber << endl << "Версия прошивки: " << diskDrive.Revision << endl << "Тип интерфейса: " << diskDrive.BusType << endl;
	cout << "Емкость устройства: " << diskDrive.TotalMemory << " GB" << endl << "Используемая память: " << diskDrive.usedMemory << " GB" << endl << "Свободная память: " << diskDrive.unusedMemory << " GB" << endl;
	cout << "Поддерживаемые режимы ATA: ";
	for (int i = 8; i >= 4; i--) if ((diskDrive.ATA >> i) & 1) cout << "ATA" << i << " ";
	cout << endl << "Поддерживаемые режимы UDMA: ";
	for (int i = 8; i >= 0; i--) if ((diskDrive.DMA >> i) & 1) cout << "UDMA" << i << " ";
	cout << endl << "Поддерживаемые режимы PIO: ";
	for (int i = 0; i < 8; i++) if ((diskDrive.Pio >> i) & 1) cout << "PIO" << i + 3 << " ";
	cout << endl << "Поддерживаемые режимы MWDMA: ";
	for (int i = 0; i < 8; i++) if ((diskDrive.MWDMA >> i) & 1) cout << "MWDMA" << i << " ";
	cout << endl << "Поддерживаемый набор команд: " << endl;
	for (int i = 0, j = 1; i < 15; i++) if ((diskDrive.commandSet >> i) & 1) cout << j++ << "." << commands[i] << endl;
	cout << endl << endl;	
}

DiskDrive getDiskDriveInfo(string physicalDrivInd) 
{
	HANDLE handle = CreateFileA(physicalDrivInd.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	STORAGE_PROPERTY_QUERY storagePropertyQuery{};
	STORAGE_DEVICE_DESCRIPTOR* storageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)calloc(BUFSIZ, sizeof(BYTE));
	DeviceIoControl(handle, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), storageDeviceDescriptor, BUFSIZ, NULL, NULL);
	DiskDrive diskDrive;
	diskDrive.ProductId = (char*)(storageDeviceDescriptor) + storageDeviceDescriptor->ProductIdOffset;
	diskDrive.SerialNumber = (char*)(storageDeviceDescriptor) + storageDeviceDescriptor->SerialNumberOffset;
	diskDrive.Revision = (char*)(storageDeviceDescriptor) + storageDeviceDescriptor->ProductRevisionOffset;
	diskDrive.VendorId = (char*)(storageDeviceDescriptor)+storageDeviceDescriptor->VendorIdOffset;
	diskDrive.BusType = bType[storageDeviceDescriptor->BusType];

	DISK_GEOMETRY_EX diskGeometry;
	DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &diskGeometry, sizeof(diskGeometry), NULL, NULL);
	diskDrive.TotalMemory = (double)diskGeometry.DiskSize.QuadPart / (double)(MEGABYTE * 1024);
	DWORD logicalDrivesMask = GetLogicalDrives();
	for (int i = 0; i < 26; i++)
	{
		if ((logicalDrivesMask >> i) & 1)
		{
			char* localBuf = new char[BUFSIZ];
			char symbl = char(65 + i);
			string pathHandle = "\\\\.\\", pathName = ":\\";
			HANDLE logicalDriveHandle = CreateFileA((pathHandle + symbl + ":").c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			DeviceIoControl(logicalDriveHandle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, sizeof(STORAGE_DEVICE_NUMBER), localBuf, BUFSIZ, NULL, (LPOVERLAPPED)NULL);
			STORAGE_DEVICE_NUMBER* deviceNumber = (STORAGE_DEVICE_NUMBER*)localBuf;
			if (physicalDrivInd[17] - 48 == deviceNumber->DeviceNumber)
			{
				ULARGE_INTEGER freeSpace;
				GetDiskFreeSpaceEx((symbl + pathName).c_str(), 0, 0, &freeSpace);
				diskDrive.unusedMemory += (double)freeSpace.QuadPart;
			}
			CloseHandle(logicalDriveHandle);
		}
	}
	diskDrive.unusedMemory /= (double)MEGABYTE * 1024;
	diskDrive.usedMemory = diskDrive.TotalMemory - diskDrive.unusedMemory;

	UCHAR identifyDataBuffer[BUFSIZ + sizeof(ATA_PASS_THROUGH_EX)] = { 0 };
	ATA_PASS_THROUGH_EX& PTE = *(ATA_PASS_THROUGH_EX*)identifyDataBuffer;
	PTE.Length = sizeof(PTE);
	PTE.TimeOutValue = 10;
	PTE.DataTransferLength = 512;
	PTE.DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX);
	PTE.CurrentTaskFile[6] = 0xEC;
	PTE.CurrentTaskFile[1] = 1;
	PTE.AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_DRDY_REQUIRED;
	DeviceIoControl(handle, IOCTL_ATA_PASS_THROUGH, &PTE, sizeof(identifyDataBuffer), &PTE, sizeof(identifyDataBuffer), NULL, 0);
	WORD* data = (WORD*)(identifyDataBuffer + sizeof(ATA_PASS_THROUGH_EX));
	diskDrive.ATA = data[80];
	diskDrive.DMA = data[88];
	diskDrive.Pio = data[64];
	diskDrive.MWDMA = data[63];
	diskDrive.commandSet = data[82];

	return diskDrive;
}

int main()
{
	system("chcp 1251");
	system("cls");

	DiskDrive SSD = getDiskDriveInfo("\\\\.\\PhysicalDrive0");
	DiskDrive HDD = getDiskDriveInfo("\\\\.\\PhysicalDrive1"); 

	show(SSD, "SSD");
	show(HDD, "HDD");

	return 0;
}