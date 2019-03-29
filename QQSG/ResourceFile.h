#ifndef _ResouceFile_H
#define _ResouceFile_H
#include "zlib/zlib.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4996)
class  ResouceDataFile
{

public:
	//��ȡ�ļ����ش���ö����
	enum GetAllgpxFilepathFromfolderError
	{
		Succeed,
		Out_of_memory,
		None_Find,
	};
#pragma pack(push,1)
	//��Դ���ļ�ͷ��
	struct ResDataHeader
	{
		char Header[4];
		int IndexNumber;
		int IndexPoint;
		int IndexSize;
	};
	//��Դ�����������
	struct ResDataIndexInfo {
		int pOffset;
		int pNone;
		int pOriginSize;
		int pDataSize;
		void *pFileName;
	};
	//��Դ�����������
	struct ResDataIndexInfo2 {
		int pNone;
		int pOffset;
		int pOriginSize;
		int pDataSize;
	};
	//��Դ�ļ���Ϣ;
	struct ResouceFileInfo
	{
		void *FileName;
		int pOffset;
		int pOriginSize;
		int pDataSize;
	};
	//��ͼλ����Ϣ;
	struct ResMapInfo {
		UINT Left;
		UINT Right;
		UINT Width;
		UINT Height;
		UINT ImageCount;
		int MouseLeft;
		int MouseRight;
		int MouseTop;
		int MouseBottom;
		char *MapPath;
	};
	struct ResMapInfoHeader {
		char header[4];
		UINT IndexNumber;
		UINT IndexPoint;
		UINT IndexSize;
	};
#pragma pack(pop)
	 ResouceDataFile();
	~ ResouceDataFile();
	//��ȡ��Դ������������(��Դ���ļ���,��������ջ�����)�ɹ���������������,ʧ�ܷ���0;
	ResDataIndexInfo *GetResDataIndex(char *FilePath);
	//�����Դ�ļ���
	BOOL PackageResouce(char *FilePath);
	//��ȡ��Դ����������;
	UINT GetResIndexNumber();
	//�����ͼ��;
	BOOL PackageMap(ResMapInfo *ResMap, UINT MapCount);
private:
	ResDataIndexInfo *GetDataIndexAlloc = nullptr;//ȫ��������;
	int DataIndexNumber = 0;//ȫ������������;
	ResouceFileInfo *GvResfile = nullptr;//ȫ����Դ�ļ���;
	UINT FileCount = 0, MaxCount = 300;//ȫ����Դ�ļ�����,����ļ�������;
	//��ȡ�ļ�·��;
	char *GetResDirectory(char*dirbuf, char*sourbuf);
	//��ȡ����Ŀ¼(ANSII����)
	void GetExePathA(char*dirbuf, int len);
	//ö����Դ�ļ����ļ���;
	GetAllgpxFilepathFromfolderError GetAllgpxFilepathFromfolder(const char*  Path);
	//���󻺳���
	void* AmplifyIndexArray(UINT MaxCount);
	//��ȡ�ļ��ַ���
	BOOL freadc(FILE *hFile, void *vReadBuffer, UINT ReadLen);
	//д���ļ��ַ���
	BOOL fwritec(FILE *hFile, void *vReadBuffer, UINT ReadLen);

};

#endif