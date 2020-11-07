#pragma once
#include <iostream>

using namespace std;

class DiskDrive
{
public:
	string ProductId = "";
	string VendorId = "";
	string SerialNumber = "";
	string Revision = "";
	string BusType = "";

	double TotalMemory = 0;
	double unusedMemory = 0;
	double usedMemory = 0;
		
	USHORT commandSet = 0;
	int MWDMA = 0;
	int DMA = 0;
	int ATA = 0;
	int Pio = 0;
};