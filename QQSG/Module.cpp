#include "Module.h"
//ȡ����Ŀ¼(UNICODE����)
void GetExePathW(WCHAR*dirbuf, int len) {
	GetModuleFileName(NULL, dirbuf, len);
	(wcsrchr(dirbuf, '\\'))[1] = 0;
}
//ȡ����Ŀ¼(ANSII����);
void GetExePathA(char*dirbuf, int len) {
	GetModuleFileNameA(NULL, dirbuf, len);
	(strrchr(dirbuf, '\\'))[1] = 0;
}
//�����༶Ŀ¼;
void CreateDirectoryEc(char* Dir) {
	char NewDir[256] = { 0 };
	CopyMemory(NewDir, Dir, strlen(Dir) * sizeof(char));
	(strrchr(NewDir, '\\'))[1] = 0;
	SHCreateDirectoryExA(nullptr, (LPCSTR)NewDir, nullptr);
}
