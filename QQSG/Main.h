#pragma once
#include "bass.h"
#include "Module.h"
#include "GameDirectX9.h"
#include "zlib\zlib.h"
#include "ResourceFile.hpp"
#include "ImeInput.h"
#include <windows.h>
#include <windowsx.h>

#pragma comment(lib,"bass.lib")
#pragma warning(disable:4996)
using namespace ResouceFile;
#define CL_WinWidth 1024
#define CL_WinHeight 768
#pragma pack(push,1)
#pragma region ȫ�����ݽṹ
struct MouseInfo {
	int x;
	int y;
	int KeyCode;
};
enum GameMaskLayer {
	Top,
	First,
	Second,
	Third,
	Four,
	Bottom,
};
typedef struct _DrawMapInfo
{
	float x;
	float y;
	UINT width;
	UINT height;
	float Scale;
	float rotation;
	UINT ImgLoadType;
	LPDIRECT3DTEXTURE9 Texture;
	void *ResAlloc;
	_DrawMapInfo *Animate;
	UINT AnimateMaxCount;
	UINT AnimateTickIndex;
	UINT AnimateOldTick;
	UINT AnimateDelay;
}DrawMapInfo, *PDrawMapInfo;
typedef struct _DrawImageInfo
{
	PDrawMapInfo DrawMap;
	UINT MaxInt;
}DrawImageInfo, *PDrawImageInfo;
#pragma pack(pop)
#pragma endregion

#pragma region //ȫ�ֱ���
const WCHAR ClassName[] = L"QQSGDiy";
HWND G_hWnd;
BOOL DrawGame = TRUE, DrawTip = FALSE, DrawCreatPlyer = TRUE, IsMan = FALSE;
void *BGM_Login1 = nullptr;
HSTREAM StreamBGM1;
DirectX92D D2Dx9;
ImeInput Ime;
MouseInfo MousePoint = { 0 };
UINT OldFpsTick = 0, FPSCount = 0, NewFPSCount = 0, OldTickCount = 0, PlayerIndex = 0, Country = 0;
DrawImageInfo CreatePlayerImgInfo = { 0 }, CreatePlayer = { 0 };
#pragma endregion
//���ڴ�����;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��Ϸ��Ⱦ����;
void D3DGameRun();
//���ŵ�¼��������;
void PlayerBGM_Login(const char *MusicName);
//��ʾFPS֡��:
void ShowFPS();
//��Դ�ļ�����;
UINT LoadResourceData(const char *pFile, const char *pResName, void **pAlloc);
//��ͼ��Դ����;
void LoadMapResourceData(ResouceDataFile::ResMapOInfo *ResMpIOinfo, DrawImageInfo &pMapAlloc, UINT pLen, const char*pMapFile);
//�ͷ��Ѽ��صĵ�ͼ��Դ
void ReleaseMapResource(DrawImageInfo &PMapImageInfo);
//��ʼ�����ƴ��������Դ;
void InitDrawCreatePlayer();
//������ҽ�ɫ����;
void DrawCreatePlayer();
//���뷨������;
void ImeEvent(WPARAM wParam);
//������ʾ��;
void DrawTipBox();
