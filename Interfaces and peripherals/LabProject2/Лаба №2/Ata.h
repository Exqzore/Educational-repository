#pragma once
#include <Windows.h>

const char* commands[] = {
    "SMART",
    "Security",
    "Removable media",
    "Power managment",
    "ATAPI",
    "Write cache",
    "Look-Ahead",
    "Release Interrupt",
    "Service Interrupt",
    "Device reset",
    "Host protected area",
};

const char* bType[] = {
    "Unknown", 
    "Scsi", 
    "Atapi", 
    "Ata", 
    "1394", 
    "Ssa", 
    "Fibre", 
    "Usb", 
    "RAID", 
    "iScsi", 
    "Sas",
    "Sata", 
    "Sd", 
    "Mmc", 
    "Virtual", 
    "FileBackedVirtual", 
    "Spaces", 
    "Max", 
    "MaxReserved" 
};