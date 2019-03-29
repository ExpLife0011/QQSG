#include "ResourceFile.h"

ResouceDataFile::ResouceDataFile()
{
	OutputDebugStringA("���캯������!\n");
}

ResouceDataFile::~ResouceDataFile()
{
	if (GetDataIndexAlloc) {
		for (int i = 0; i < DataIndexNumber; i++) {
			if (GetDataIndexAlloc[i].pFileName) {
				LocalFree(GetDataIndexAlloc[i].pFileName);
			}
		}
		LocalFree(GetDataIndexAlloc);
	}
	OutputDebugStringA("������������!\n");
}
//��ȡ��Դ������������(��Դ���ļ���,��������ջ�����)�ɹ���������������,ʧ�ܷ���0. ��ʹ��һ���ǵ��ͷ���Դ;
ResouceDataFile::ResDataIndexInfo *ResouceDataFile::GetResDataIndex(char *FilePath) {
	FILE *ResFile = nullptr;
	ResDataIndexInfo *ResInfo = nullptr;
	int pathlen = 0;
	ResFile = fopen(FilePath, "rb");
	if (!ResFile) {
		MessageBox(nullptr, L"�ļ���ʧ��,��ȷ���Ƿ���Ȩ��!", nullptr, MB_OK);
		DataIndexNumber = 0;
		return nullptr;
	}
	ResDataHeader ResHeader;
	fseek(ResFile, 0, SEEK_SET);
	fread(&ResHeader, sizeof(ResHeader), 1, ResFile);
	fseek(ResFile, ResHeader.IndexPoint, SEEK_SET);
	ResInfo = (ResDataIndexInfo*)LocalAlloc(LMEM_ZEROINIT, sizeof(ResDataIndexInfo)*ResHeader.IndexNumber);
	if (!ResInfo) {
		MessageBox(nullptr, L"�ڴ�����ʧ��!��ȷ���Ƿ��ڴ治��!", nullptr, MB_OK);
		fclose(ResFile);
		DataIndexNumber = 0;
		return nullptr;
	}
	for (int i = 0; i < ResHeader.IndexNumber; i++) {
		fread(&pathlen, 2, 1, ResFile);
		ResInfo[i].pFileName = LocalAlloc(LMEM_ZEROINIT, pathlen + 1);
		if (ResInfo[i].pFileName) {
			fread(ResInfo[i].pFileName, pathlen, 1, ResFile);
		}
		else {
			MessageBox(nullptr, L"�ڴ�����ʧ��!��ȷ���Ƿ��ڴ治��!", nullptr, MB_OK);
			break;
		}
		fread(&ResInfo[i].pNone, sizeof(ResInfo[i].pNone), 1, ResFile);
		fread(&ResInfo[i].pOffset, sizeof(ResInfo[i].pOffset), 1, ResFile);
		fread(&ResInfo[i].pOriginSize, sizeof(ResInfo[i].pOriginSize), 1, ResFile);
		fread(&ResInfo[i].pDataSize, sizeof(ResInfo[i].pDataSize), 1, ResFile);
	}
	fclose(ResFile);
	GetDataIndexAlloc = ResInfo;
	DataIndexNumber = ResHeader.IndexNumber;
	return ResInfo;
}
//��ȡ����������;
UINT ResouceDataFile::GetResIndexNumber() {
	return DataIndexNumber;
}
//��ȡ����Ŀ¼(ANSII����)
void ResouceDataFile::GetExePathA(char*dirbuf, int len) {
	GetModuleFileNameA(NULL, dirbuf, len);
	(strrchr(dirbuf, '\\'))[1] = 0;
}
//����ļ���Դ��(Ԥ����ļ�Ŀ¼);
BOOL ResouceDataFile::PackageResouce(char *FilePath) {
	//����ȫ��Ԥ�����Դ�ļ�������;
	GvResfile = (ResouceFileInfo*)LocalAlloc(LMEM_ZEROINIT, sizeof(ResouceFileInfo) * MaxCount);
	if(!GvResfile)return FALSE;
	//ö��Ԥ�����Դ�ļ�Ŀ¼�ļ�;
	if (GetAllgpxFilepathFromfolder(FilePath) != Succeed)return FALSE;
	char ResDir[MAX_PATH] = { 0 };
	//ȡ����Ŀ¼;
	GetExePathA(ResDir, MAX_PATH);
	//��������Դ���ļ�;
	strcat(ResDir, "ResData.db");
	//��д��Դ�ļ�����;
	FILE *OpenResFile = fopen(ResDir, "wb");
	if (!OpenResFile)return FALSE;
	//д����Դ���ļ�ͷ����Ϣ;
	fseek(OpenResFile, 0, SEEK_SET);
	ResDataHeader ResHeader = { 0 };
	fwrite(&ResHeader, sizeof(ResDataHeader), 1, OpenResFile);
	//д����Դ�ļ�����;
	for (UINT i = 0; i < FileCount; i++) {
		//��ȡ�ļ���ָ��;
		char *pPath = (char*)GvResfile[i].FileName;
		OutputDebugStringA(pPath);
		OutputDebugStringA("\r\n");
		//��Ԥ�����Դ�ļ�;
		if (!pPath) { MessageBox(0, L"���ʧ��!", nullptr, NULL); break; }
		FILE *TmpFile = fopen(pPath, "rb");
		if (!TmpFile) { MessageBox(0, L"���ʧ��!", nullptr, NULL); break; }
		//�ƶ�Ԥ�����Դ�ļ�ָ�벢��ȡ�ļ���С;
		fseek(TmpFile, 0, SEEK_END);
		UINT FileLen = ftell(TmpFile);
		//���뻺����;
		void *TmpData = LocalAlloc(LMEM_ZEROINIT, FileLen);
		if (!TmpData)return FALSE;
		//����Ԥ�����Դ�ļ�ָ��;
		fseek(TmpFile, 0, SEEK_SET);
		//��ȡ�������Դ�ļ�;
		UINT ReadLen = fread(TmpData, sizeof(char), FileLen, TmpFile);
		//����ѹ��������;
		void *zlibData = LocalAlloc(LMEM_ZEROINIT, ReadLen);
		if (!zlibData)return FALSE;
		//ѹ����Դ������;
		compress((Bytef*)zlibData, (uLongf*)&ReadLen, (Bytef*)TmpData, FileLen);
		GvResfile[i].pOriginSize = FileLen;
		GvResfile[i].pDataSize = ReadLen;
		//д����ѹ������;
		GvResfile[i].pOffset = ftell(OpenResFile);
		fwrite(zlibData, sizeof(char), ReadLen, OpenResFile);
		//�رմ򿪵���Դ�ļ�;
		LocalFree(TmpData);
		LocalFree(zlibData);
		fclose(TmpFile);
		//ˢ����������;
		fflush(OpenResFile);
	}
	//д����Դ�ļ�������;
	ResHeader.IndexPoint = ftell(OpenResFile);
	memcpy(ResHeader.Header, "PKS", sizeof(ResHeader.Header));//�ļ�ͷ��ʶ
	ResHeader.IndexNumber = FileCount;
	ResouceDataFile::ResDataIndexInfo2 Resinf = { 0 };
	for (UINT i = 0; i < FileCount; i++) {
		//��ȡ��Դ·��;
		char *pchar = GetResDirectory(FilePath, (char*)GvResfile[i].FileName);
		if (!pchar) { MessageBox(0, L"��ȡ�ļ�Ŀ¼ʧ��!", 0, 0); return FALSE; }
		//ȡ·������
		WORD slens = (WORD)strlen(pchar) + 1;
		//ȡ��������
		ResHeader.IndexSize += (WORD)((sizeof(ResDataIndexInfo2) + slens) + sizeof(WORD));
		//���������ṹ
		Resinf.pNone = 1;
		Resinf.pOffset = GvResfile[i].pOffset;
		Resinf.pOriginSize = GvResfile[i].pOriginSize;
		Resinf.pDataSize = GvResfile[i].pDataSize;
		//д���������ļ�������;
		fwrite(&slens, sizeof(WORD), 1, OpenResFile);
		//д���������ļ���;
		fwrite(pchar, slens, 1, OpenResFile);
		//д�������ṹ;
		fwrite(&Resinf, sizeof(ResDataIndexInfo2), 1, OpenResFile);
	}
	//ˢ����������;
	fflush(OpenResFile);
	//д���ļ�ͷ������;
	fseek(OpenResFile, 0, SEEK_SET);
	fwrite(&ResHeader, sizeof(ResDataHeader), 1, OpenResFile);
	fflush(OpenResFile);
	//�رմ򿪵���Դ���ļ�;
	fclose(OpenResFile);
	for (UINT i = 0; i < FileCount; i++) {
		if (GvResfile[i].FileName) {
			LocalFree(GvResfile[i].FileName);
		}
	}
	LocalFree(GvResfile);
	return TRUE;
}
//��ȡ��Դ·��;
char *ResouceDataFile::GetResDirectory(char*dirbuf, char*sourbuf) {
	int ilen = strlen(sourbuf);
	char *pdir = sourbuf;
	for (int i = 0; i < ilen; i++) {
		if (sourbuf[i] == dirbuf[i])pdir++;
		else { pdir++; break; }
	}
	return pdir;
}
//��ȡ�ļ��ַ�������;
BOOL ResouceDataFile::freadc(FILE *hFile, void *vReadBuffer, UINT ReadLen) {
	char *pBuf = (char*)vReadBuffer;
	char GetChar;
	for (UINT i = 0; i < ReadLen; i++) {
		GetChar = fgetc(hFile);
		if (GetChar == EOF)return FALSE;
		pBuf[i] = GetChar;
	}
	return TRUE;
}
//д���ļ��ַ�������;
BOOL ResouceDataFile::fwritec(FILE *hFile, void *vReadBuffer, UINT ReadLen) {
	char *pBuf = (char*)vReadBuffer;
	for (UINT i = 0; i < ReadLen; i++) {
		if (i >= 4095) {
			int ir = 0;
		}
		if (fputc(pBuf[i], hFile) == EOF)return FALSE;
	}
	return 0;
}
//����ȫ����Դ�ļ�������;
void* ResouceDataFile::AmplifyIndexArray(UINT MaxCount) {
	void *Alloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ResouceFileInfo) * MaxCount);
	if (!Alloc)return nullptr;
	void *OldAlloc = GvResfile;
	memcpy(&GvResfile, &Alloc, 4);
	memcpy(GvResfile, OldAlloc, sizeof(ResouceFileInfo) *FileCount);
	LocalFree(OldAlloc);
	return Alloc;
}
//��ȡĿ¼�����ļ���(Ŀ¼,��Դ�ļ������黺����,���鵱ǰ����,�������ֵ);
ResouceDataFile::GetAllgpxFilepathFromfolderError ResouceDataFile::GetAllgpxFilepathFromfolder(const char *Path)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;
	strcpy(szFind, Path);
	strcat(szFind, "\\*.*");
	HANDLE hFind = FindFirstFileA(szFind, &FindFileData);
	char *RootItem = nullptr;
	if (INVALID_HANDLE_VALUE == hFind)return None_Find;
	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
			{
				//������Ŀ¼���ݹ�֮
				char szFile[MAX_PATH] = { 0 };
				strcpy(szFile, Path);
				strcat(szFile, "\\");
				strcat(szFile, FindFileData.cFileName);
				if (GetAllgpxFilepathFromfolder(szFile) != Succeed)return Out_of_memory;
			}
		}
		else
		{
			char szFile[MAX_PATH] = { 0 };
			strcpy(szFile, Path);
			strcat(szFile, "\\");
			strcat(szFile, FindFileData.cFileName);
			if (FileCount >= MaxCount) {
				MaxCount += 100;
				AmplifyIndexArray(MaxCount);
			}
			UINT szflen = strlen(szFile) + 1;
			if (!szflen)break;
			void *FileAlloc = LocalAlloc(LMEM_ZEROINIT, szflen);
			if (!FileAlloc)return Out_of_memory;
			memcpy(FileAlloc, szFile, szflen);
			GvResfile[FileCount].FileName = FileAlloc;

			FileCount++;
		}
	} while (FindNextFileA(hFind, &FindFileData));
	FindClose(hFind);
	return Succeed;
}
//�����ͼ��;
BOOL ResouceDataFile::PackageMap(ResMapInfo *ResMap, UINT MapCount) {
	char FileName[256] = { 0 };
	GetExePathA(FileName, sizeof(FileName));
	strcat(FileName, "\\ResMap.map");
	FILE *pFile = fopen(FileName, "wb");
	if (!pFile) return FALSE;
	ResMapInfoHeader ResMapHeader = { 0 };
	strcpy(ResMapHeader.header, "Map");
	fwrite(&ResMapHeader, sizeof(ResMapHeader), 1, pFile);
	void *Tmpdata = LocalAlloc(LMEM_ZEROINIT, (4 * 1024) * 1024);
	if (!Tmpdata)return FALSE;
	for (UINT i = 0; i < MapCount; i++) {
		
	}
	LocalFree(Tmpdata);
	fclose(pFile);
	return TRUE;
}

