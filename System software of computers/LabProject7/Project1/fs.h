#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#define BLOCK_SIZE 128

typedef unsigned long ADRESS;

using namespace std;

struct DirDesriptor {
	char name[64];
	ADRESS parentAdr;
	ADRESS dirsBlockAdr;
	ADRESS filesBlockAdr;
	char unused[52];
};

struct FileDescriptor {
	char name[64];
	ADRESS dataBlockAdr;
	char unused[60];
};

struct DirsBlock {
	ADRESS dirs[31];
	ADRESS nextDirsBlock;
};

struct FilesBlock {
	ADRESS files[31];
	ADRESS nextFilesBlock;
};

struct DataBlock {
	char data[124];
	ADRESS nextDataBlock;
};

class VirtualFS {
private:
	fstream file;
	vector<string> currentPath;
	ADRESS curDirAdr;
	DirDesriptor curDirDesc;
	bitset<1024> memoryBlock;
public:
	VirtualFS();
	VirtualFS(string filename);
	~VirtualFS();
	vector<string> getCurrentDirNames();
	vector<string> getCurrentFileNames();
	void createDir(string);
	void deleteDir(string);
	void renameDir(string, string);
	void enterDir(string);
	ADRESS findDirBlock(string);
	ADRESS findDirDesc(string);
	ADRESS findFileBlock(string);
	ADRESS findFileDesc(string);
	ADRESS allocateBlock();
	void freeBlock(ADRESS);
	DirDesriptor getDirDescriptor(ADRESS);
	static void createVirtualFs(string);
	void readBlock(ADRESS, char*);
	void writeBlock(ADRESS, char*);
	vector<string> getCurrentPath();
	unsigned long getUsedMemory();
	void createFile(string);
	void deleteFile(string);
	void renameFile(string, string);
	void editFile(string, string);
	string readFile(string);
	void updateMemory();
};