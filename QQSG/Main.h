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
#pragma region 全局数据结构
struct MouseInfo {
	int x;
	int y;
	int KeyCode;
	int KeyState;
	UINT OldTick;
};
enum _ButtonType {
	Button_Dan = 0x61,
	Button_Duo = 0x62,
};
typedef struct _DrawMapInfo
{
	float x;
	float y;
	UINT width;
	UINT height;
	float Scale;
	float rotation;
	ImageLoadStyle ImgLoadType;
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
typedef struct _ButtonImageInfo{
	float x;
	float y;
	UINT Width;
	UINT Height;
	UINT SelectIndex;
	UINT ButtonNum;
	_ButtonType ButtonType;
	LPDIRECT3DTEXTURE9 Texture;
	_ButtonImageInfo *MultiButton;
	void *ResAlloc;
}ButtonImageInfo, *PButtonImageInfo;
struct ButtonInfo
{
	PButtonImageInfo Button;
	UINT MaxButton;
};
#pragma pack(pop)
#pragma endregion

#pragma region //全局变量
const WCHAR ClassName[] = L"QQSGDiy";
char szCareerTip[256] = { "        阴阳士\r\n\r\n远程丶术攻丶高攻丶施法慢" };
char szTipBoxText[1024] = { 0 };
HWND G_hWnd;
BOOL DrawGame = TRUE, DrawTip = FALSE, DrawCreatPlyer = TRUE, IsTipBox = FALSE;
void *BGM_Login1 = nullptr;
HSTREAM StreamBGM1;
DirectX92D D2Dx9;
ImeInput Ime;
MouseInfo MousePoint = { 0 };
UINT OldFpsTick = 0, FPSCount = 0, NewFPSCount = 0, OldTickCount = 0, PlayerIndex = 0, Country = 0, ImeTick = 0;
DrawImageInfo CreatePlayerImgInfo = { 0 }, CreatePlayer = { 0 };
UINT 玩家职业 = 0, 玩家国家 = 0;
BOOL 玩家性别是否为男 = FALSE;
char 玩家名字[20] = { 0 };
ButtonInfo ClButton = { 0 }, ClTipButton = { 0 };
#pragma endregion
//窗口处理函数;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//游戏渲染函数;
void D3DGameRun();
//播放登录背景音乐;
void PlayerBGM_Login(const char *MusicName);
//显示FPS帧率:
void ShowFPS();
//资源文件加载;
UINT LoadResourceData(const char *pFile, const char *pResName, void **pAlloc);
//释放加载的资源文件;
void ReleaseResourceData(void *pAlloc);
//地图资源加载;
void LoadMapResourceData(ResouceDataFile::ResMapOInfo *ResMpIOinfo, DrawImageInfo &pMapAlloc, UINT pLen, const char*pMapFile);
//释放已加载的按钮资源
void ReleaseButtonResource(ButtonInfo &PMapImageInfo);
//释放已加载的地图资源
void ReleaseMapResource(DrawImageInfo &PMapImageInfo);
//绘制播放职业玩家;
void PlayerChange(const char *pMapFile, DrawImageInfo &pMapAlloc);
//绘制玩家角色创建;
void DrawCreatePlayer();
//输入法处理函数;
void ImeEvent(WPARAM wParam);
//提示对话框;
void SetTipDialogBoxText(const char *pText);
void DrawTipDialogBox();
//绘制提示框;
void DrawTipBox();
//判断鼠标是否在选区内
BOOL IsRectMouse(float x, float y, UINT Width, UINT Height, MouseInfo const&MousePoint);
//鼠标点击延迟计算;
BOOL ClickMouseDelay();