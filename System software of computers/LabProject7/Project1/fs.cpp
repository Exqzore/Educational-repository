#include "fs.h"

VirtualFS::VirtualFS() {}

VirtualFS::VirtualFS(string filename) {
	file.open(filename, ios_base::in | ios_base::out | ios_base::binary);
	char tempMemoryBlock[BLOCK_SIZE];
	file.read((char*)&tempMemoryBlock, BLOCK_SIZE);
	for (int j = 0, i = 0; j < BLOCK_SIZE; j++) {
		bitset<8> temp = tempMemoryBlock[j];
		for (char k = 0; k < 8; k++)
			memoryBlock[i++] = temp[k];
	}
	currentPath.push_back("root");
	curDirAdr = 1;
	curDirDesc = getDirDescriptor(curDirAdr);
}

VirtualFS::~VirtualFS() {
	file.close();
	updateMemory();
}

vector<string> VirtualFS::getCurrentDirNames() {
	vector<string> names;
	if (curDirDesc.parentAdr) names.push_back("..");
	DirsBlock dirsBlock;
	file.seekg((long long)curDirDesc.dirsBlockAdr * BLOCK_SIZE);
	file.read((char*)&dirsBlock, sizeof(DirsBlock));
	while (true) {
		for (unsigned long i = 0; i < 31; i++)
			if (dirsBlock.dirs[i] != NULL) {
				file.seekg((long long)dirsBlock.dirs[i] * BLOCK_SIZE);
				DirDesriptor temp;
				file.read((char*)&temp, sizeof(DirDesriptor));
				names.push_back(temp.name);
			}
			else break;
		if (dirsBlock.nextDirsBlock != NULL) {
			file.seekg((long long)dirsBlock.nextDirsBlock * BLOCK_SIZE);
			file.read((char*)&dirsBlock, sizeof(DirsBlock));
		}
		else break;
	}
	return names;
}

vector<string> VirtualFS::getCurrentFileNames() {
	vector<string> names;
	FilesBlock filesBlock;
	readBlock(curDirDesc.filesBlockAdr, (char*)&filesBlock);
	while (true) {
		for (unsigned long i = 0; i < 31; i++)
			if (filesBlock.files[i]) {
				FileDescriptor tempFileDesc;
				readBlock(filesBlock.files[i], (char*)&tempFileDesc);
				names.push_back(tempFileDesc.name);
			}
			else break;
		if (filesBlock.nextFilesBlock) readBlock(filesBlock.nextFilesBlock, (char*)&filesBlock);
		else break;
	}
	return names;
}

DirDesriptor VirtualFS::getDirDescriptor(ADRESS adr) {
	DirDesriptor desc;
	file.seekg((long long)adr * BLOCK_SIZE);
	file.read((char*)&desc, BLOCK_SIZE);
	return desc;
}

void VirtualFS::createVirtualFs(string name) {
	ofstream file;
	file.open(name);
	char emptyBlock[BLOCK_SIZE] = { 0 };
	emptyBlock[0] = 0b00001111;
	file.write(emptyBlock, BLOCK_SIZE);
	emptyBlock[0] = 0;
	DirDesriptor root = { 0 };
	strcpy_s(root.name, "root");
	root.dirsBlockAdr = 2;
	root.filesBlockAdr = 3;
	file.write((char*)&root, BLOCK_SIZE);
	file.write(emptyBlock, BLOCK_SIZE);
	file.write(emptyBlock, BLOCK_SIZE);
	file.close();
}

void VirtualFS::createDir(string dirName) {
	if (findDirBlock(dirName)) return;
	DirsBlock dirsBlock;
	readBlock(curDirDesc.dirsBlockAdr, (char*)&dirsBlock);
	while (dirsBlock.nextDirsBlock) readBlock(dirsBlock.nextDirsBlock, (char*)&dirsBlock);
	while (true) {
		for (char i = 0; i < 31; i++)
			if (!dirsBlock.dirs[i]) {
				dirsBlock.dirs[i] = allocateBlock();
				DirDesriptor dirDesc = { 0 };
				dirDesc.dirsBlockAdr = allocateBlock();
				dirDesc.filesBlockAdr = allocateBlock();
				dirDesc.parentAdr = curDirAdr;
				strcpy_s(dirDesc.name, dirName.c_str());
				writeBlock(dirsBlock.dirs[i], (char*)&dirDesc);
				writeBlock(curDirDesc.dirsBlockAdr, (char*)&dirsBlock);
				return;
			}
		dirsBlock.nextDirsBlock = allocateBlock();
		readBlock(dirsBlock.nextDirsBlock, (char*)&dirsBlock);
	}
}

void VirtualFS::deleteDir(string dirName) {
	if (!findDirBlock(dirName))return;
	enterDir(dirName);
	vector<string> tempDirNames = getCurrentDirNames();
	vector<string> tempFileNames = getCurrentFileNames();
	for (auto it : tempFileNames) deleteFile(it);
	for (unsigned long i = 1; i < tempDirNames.size(); i++) deleteDir(tempDirNames[i]);
	enterDir("..");
	DirsBlock dirsBlock;
	ADRESS temp = findDirBlock(dirName);
	if (temp == 0) return;
	readBlock(temp, (char*)&dirsBlock);
	char numberToDelete;
	for (char i = 0; i < 31; i++)
		if (dirsBlock.dirs[i]) {
			DirDesriptor dirDesc;
			readBlock(dirsBlock.dirs[i], (char*)&dirDesc);
			if (string(dirDesc.name) == dirName) numberToDelete = i;
		}
	DirsBlock lastDirsBlock = dirsBlock;
	ADRESS lastDirsBlockAdr = temp;
	ADRESS subLastDirsBlockAdr = temp;
	while (lastDirsBlock.nextDirsBlock) {
		subLastDirsBlockAdr = lastDirsBlockAdr;
		lastDirsBlockAdr = lastDirsBlock.nextDirsBlock;
		readBlock(lastDirsBlock.nextDirsBlock, (char*)&lastDirsBlock);
	}
	for (char i = 0; i < 31; i++)
		if (!lastDirsBlock.dirs[i] || i == 30) {
			if (i == 30) i = 31;
			DirDesriptor dirDesc;
			readBlock(dirsBlock.dirs[numberToDelete], (char*)&dirDesc);
			DirsBlock toDelDirsBlock;
			FilesBlock toDelFilesBlock;
			readBlock(dirDesc.dirsBlockAdr, (char*)&toDelDirsBlock);
			readBlock(dirDesc.filesBlockAdr, (char*)&toDelFilesBlock);
			freeBlock(dirDesc.dirsBlockAdr);
			freeBlock(dirDesc.filesBlockAdr);
			freeBlock(dirsBlock.dirs[numberToDelete]);
			while (toDelDirsBlock.nextDirsBlock) {
				freeBlock(toDelDirsBlock.nextDirsBlock);
				readBlock(toDelDirsBlock.nextDirsBlock, (char*)&toDelDirsBlock);
			}
			while (toDelFilesBlock.nextFilesBlock) {
				freeBlock(toDelFilesBlock.nextFilesBlock);
				readBlock(toDelFilesBlock.nextFilesBlock, (char*)&toDelFilesBlock);
			}
			if (temp != lastDirsBlockAdr) {
				dirsBlock.dirs[numberToDelete] = lastDirsBlock.dirs[i - 1];
				lastDirsBlock.dirs[i - 1] = 0;
				writeBlock(temp, (char*)&dirsBlock);
				writeBlock(lastDirsBlockAdr, (char*)&lastDirsBlock);
			}
			else {
				dirsBlock.dirs[numberToDelete] = dirsBlock.dirs[i - 1];
				dirsBlock.dirs[i - 1] = 0;
				writeBlock(temp, (char*)&dirsBlock);
			}
			break;
		}
	if (lastDirsBlock.dirs[0] == 0 && lastDirsBlockAdr != curDirDesc.dirsBlockAdr) {
		DirsBlock subLastDirsBlockDesc;
		readBlock(subLastDirsBlockAdr, (char*)&subLastDirsBlockDesc);
		subLastDirsBlockDesc.nextDirsBlock = 0;
		writeBlock(subLastDirsBlockAdr, (char*)&subLastDirsBlockDesc);
		freeBlock(lastDirsBlockAdr);
	}
}

void VirtualFS::renameDir(string dirName, string newName) {
	if (findDirBlock(newName) || !findDirBlock(dirName)) return;
	ADRESS toRenameDirAdr = findDirDesc(dirName);
	DirDesriptor toRenameDirDesc;
	readBlock(toRenameDirAdr, (char*)&toRenameDirDesc);
	strcpy_s(toRenameDirDesc.name, newName.c_str());
	writeBlock(toRenameDirAdr, (char*)&toRenameDirDesc);
}

void VirtualFS::enterDir(string dirName) {
	if (dirName == "..") {
		currentPath.pop_back();
		curDirAdr = curDirDesc.parentAdr;
		readBlock(curDirAdr, (char*)&curDirDesc);
	}
	else {
		currentPath.push_back(dirName);
		curDirAdr = findDirDesc(dirName);
		readBlock(curDirAdr, (char*)&curDirDesc);
	}
}

ADRESS VirtualFS::findDirDesc(string dirName) {
	DirsBlock dirsBlock;
	ADRESS temp = findDirBlock(dirName);
	if (temp == 0) return 0;
	readBlock(temp, (char*)&dirsBlock);
	for (char i = 0; i < 31; i++)
		if (dirsBlock.dirs[i]) {
			DirDesriptor dirDesc;
			readBlock(dirsBlock.dirs[i], (char*)&dirDesc);
			if (string(dirDesc.name) == dirName) return dirsBlock.dirs[i];
		}
}

ADRESS VirtualFS::findDirBlock(string dirName) {
	ADRESS result = curDirDesc.dirsBlockAdr;
	DirsBlock dirsBlock;
	readBlock(curDirDesc.dirsBlockAdr, (char*)&dirsBlock);
	while (true) {
		for (char i = 0; i < 31; i++)
			if (dirsBlock.dirs[i]) {
				DirDesriptor dirDesc;
				readBlock(dirsBlock.dirs[i], (char*)&dirDesc);
				if (string(dirDesc.name) == dirName) return result;
			}
			else return 0;
		if (dirsBlock.nextDirsBlock) {
			result = dirsBlock.nextDirsBlock;
			readBlock(dirsBlock.nextDirsBlock, (char*)&dirsBlock);
		}
		else return 0;
	}
}

ADRESS VirtualFS::findFileBlock(string fileName) {
	ADRESS result = curDirDesc.filesBlockAdr;
	FilesBlock filesBlock;
	readBlock(curDirDesc.filesBlockAdr, (char*)&filesBlock);
	while (true) {
		for (char i = 0; i < 31; i++)
			if (filesBlock.files[i]) {
				FileDescriptor fileDesc;
				readBlock(filesBlock.files[i], (char*)&fileDesc);
				if (string(fileDesc.name) == fileName) return result;
			}
			else return 0;
		if (filesBlock.nextFilesBlock) {
			result = filesBlock.nextFilesBlock;
			readBlock(filesBlock.nextFilesBlock, (char*)&filesBlock);
		}
		else return 0;
	}
}

ADRESS VirtualFS::findFileDesc(string fileName) {
	FilesBlock	filesBlock;
	ADRESS temp = findFileBlock(fileName);
	if (temp == 0) return 0;
	readBlock(temp, (char*)&filesBlock);
	for (char i = 0; i < 31; i++)
		if (filesBlock.files[i]) {
			FileDescriptor fileDesc;
			readBlock(filesBlock.files[i], (char*)&fileDesc);
			if (string(fileDesc.name) == fileName) return filesBlock.files[i];
		}
}

ADRESS VirtualFS::allocateBlock() {
	for (ADRESS i = 0; i < memoryBlock.size(); i++)
		if (!memoryBlock[i]) {
			memoryBlock[i] = true;
			char emptyblock[BLOCK_SIZE] = { 0 };
			writeBlock(i, emptyblock);
			updateMemory();
			return i;
		}
	return 0;
}

void VirtualFS::freeBlock(ADRESS adr) {
	memoryBlock[adr] = false;
	updateMemory();
}

void VirtualFS::readBlock(ADRESS adr, char* dest) {
	file.seekg((long long)adr * BLOCK_SIZE);
	file.read(dest, sizeof(DirsBlock));
}

void VirtualFS::writeBlock(ADRESS adr, char* source) {
	file.seekp((long long)adr * BLOCK_SIZE);
	file.write(source, sizeof(DirsBlock));
}

vector<string> VirtualFS::getCurrentPath() {
	return currentPath;
}

unsigned long VirtualFS::getUsedMemory() {
	return memoryBlock.count();
}


void VirtualFS::createFile(string fileName) {
	if (findFileBlock(fileName)) return;
	FilesBlock filesBlock;
	readBlock(curDirDesc.filesBlockAdr, (char*)&filesBlock);
	while (filesBlock.nextFilesBlock) readBlock(filesBlock.nextFilesBlock, (char*)&filesBlock);
	while (true) {
		for (char i = 0; i < 31; i++)
			if (!filesBlock.files[i]) {
				filesBlock.files[i] = allocateBlock();
				FileDescriptor fileDesc = { 0 };
				fileDesc.dataBlockAdr = allocateBlock();
				strcpy_s(fileDesc.name, fileName.c_str());
				writeBlock(filesBlock.files[i], (char*)&fileDesc);
				writeBlock(curDirDesc.filesBlockAdr, (char*)&filesBlock);
				return;
			}
		filesBlock.nextFilesBlock = allocateBlock();
		readBlock(filesBlock.nextFilesBlock, (char*)&filesBlock);
	}
}

void VirtualFS::renameFile(string fileName, string newName) {
	if (findFileBlock(newName) || !findFileBlock(fileName)) return;
	ADRESS toRenameFileAdr = findFileDesc(fileName);
	FileDescriptor toRenameFileDesc;
	readBlock(toRenameFileAdr, (char*)&toRenameFileDesc);
	strcpy_s(toRenameFileDesc.name, newName.c_str());
	writeBlock(toRenameFileAdr, (char*)&toRenameFileDesc);
}

void VirtualFS::editFile(string fileName, string newData) {
	FileDescriptor fileDesc;
	DataBlock dataBlock;
	readBlock(findFileDesc(fileName), (char*)&fileDesc);
	readBlock(fileDesc.dataBlockAdr, (char*)&dataBlock);
	ADRESS currDataBlockAdr = fileDesc.dataBlockAdr;
	for (unsigned long i = 0; i < newData.size(); i += 124) {
		for (char j = 0; j < 124 && i + j < newData.size(); j++) dataBlock.data[j] = newData[j + i];
		if (i + 124 < newData.size()) {
			if (!dataBlock.nextDataBlock) dataBlock.nextDataBlock = allocateBlock();
			writeBlock(currDataBlockAdr, (char*)&dataBlock);
			currDataBlockAdr = dataBlock.nextDataBlock;
			readBlock(currDataBlockAdr, (char*)&dataBlock);
		}
		else writeBlock(currDataBlockAdr, (char*)&dataBlock);
	}
}

string VirtualFS::readFile(string fileName) {
	string result;
	FileDescriptor fileDesc;
	DataBlock dataBlock;
	readBlock(findFileDesc(fileName), (char*)&fileDesc);
	readBlock(fileDesc.dataBlockAdr, (char*)&dataBlock);
	for (char i = 0; i < 124; i++)
		if (dataBlock.data[i]) result += dataBlock.data[i];
		else break;
	while (dataBlock.nextDataBlock) {
		readBlock(dataBlock.nextDataBlock, (char*)&dataBlock);
		result += dataBlock.data;
	}
	return result;
}

void VirtualFS::updateMemory() {
	char block[128];
	for (unsigned char i = 0; i < 128; i++)
		block[i] = memoryBlock[i * 8] * 1 + memoryBlock[i * 8 + 1] * 2 + memoryBlock[i * 8 + 2] * 4 + memoryBlock[i * 8 + 3] * 8
		+ memoryBlock[i * 8 + 4] * 16 + memoryBlock[i * 8 + 5] * 32 + memoryBlock[i * 8 + 6] * 64 + memoryBlock[i * 8 + 7] * 128;
	writeBlock(0, block);
}

void VirtualFS::deleteFile(string fileName) {
	ADRESS filesBlockAdr = findFileBlock(fileName);
	if (!filesBlockAdr) return;
	ADRESS fileDescAdr = findFileDesc(fileName);
	FilesBlock filesBlock;
	readBlock(filesBlockAdr, (char*)&filesBlock);
	char numberToDelete;
	for (char i = 0; i < 31; i++)
		if (filesBlock.files[i] == fileDescAdr) {
			numberToDelete = i;
			break;
		}
	FilesBlock lastFilesBlock = filesBlock;
	ADRESS lastFilesBlockAdr = filesBlockAdr;
	ADRESS subLastFilesBlockAdr = filesBlockAdr;
	while (lastFilesBlock.nextFilesBlock) {
		subLastFilesBlockAdr = lastFilesBlockAdr;
		lastFilesBlockAdr = lastFilesBlock.nextFilesBlock;
		readBlock(lastFilesBlock.nextFilesBlock, (char*)&lastFilesBlock);
	}
	for (char i = 0; i < 31; i++)
		if ((i == 30 ? true : !lastFilesBlock.files[i + 1])) {
			FileDescriptor fileDesc;
			readBlock(fileDescAdr, (char*)&fileDesc);
			freeBlock(fileDescAdr);
			freeBlock(fileDesc.dataBlockAdr);
			DataBlock toDelDataBlock;
			readBlock(fileDesc.dataBlockAdr, (char*)&toDelDataBlock);
			while (toDelDataBlock.nextDataBlock) {
				freeBlock(toDelDataBlock.nextDataBlock);
				readBlock(toDelDataBlock.nextDataBlock, (char*)&toDelDataBlock);
			}
			if (lastFilesBlockAdr == filesBlockAdr) {
				filesBlock.files[numberToDelete] = filesBlock.files[i];
				filesBlock.files[i] = 0;
				writeBlock(filesBlockAdr, (char*)&filesBlock);
			}
			else {
				filesBlock.files[numberToDelete] = lastFilesBlock.files[i];
				lastFilesBlock.files[i] = 0;
				writeBlock(filesBlockAdr, (char*)&filesBlock);
				writeBlock(lastFilesBlockAdr, (char*)&lastFilesBlock);
			}
			break;
		}
	if (lastFilesBlock.files[0] == 0 && lastFilesBlockAdr != curDirDesc.filesBlockAdr) {
		FilesBlock subLastFilesBlockDesc;
		readBlock(subLastFilesBlockAdr, (char*)&subLastFilesBlockDesc);
		subLastFilesBlockDesc.nextFilesBlock = 0;
		writeBlock(subLastFilesBlockAdr, (char*)&subLastFilesBlockDesc);
		freeBlock(lastFilesBlockAdr);
	}
}