#pragma once
#include "bass.h"
#include "Module.h"
#include "GameDirectX9.h"
#include "zlib\zlib.h"
#include "ResourceFile.h"
#include "ImeInput.h"
#include <windows.h>
#include <windowsx.h>


#pragma comment(lib,"bass.lib")
#pragma warning(disable:4996)

#define CL_WinWidth 1024
#define CL_WinHeight 768
#pragma pack(push,1)
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
typedef struct _ImageTexturInfo
{
	int MaxImage;
	int x;
	int y;
	int width;
	int height;
	int MaskLayer;
	float Scale;
	float rotation;
	LPDIRECT3DTEXTURE9 Texture;
	const wchar_t *ImageFile;
}ImageTexturInfo, *PImageTexturInfo;
typedef struct _NPCImageInfo
{
	PImageTexturInfo TextureInfo;
	int iflag;
	int CurrencyImage;
	int OldTickCount;
}NPCImageInfo, *PNPCImageInfo;
#pragma pack(pop)

//���ڴ�����;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��Ϸ��Ⱦ����;
void D3DGameRun();
//���ŵ�¼��������;
void PlayerBGM_Login(const char *MusicName);
//�������;
void DrawPlayerCarrorYY(PNPCImageInfo _PlayerInfo, int PlayDelay);
//������ҽ�ɫ����;
void DrawCreatePlayer();
//���뷨������;
void ImeEvent(WPARAM wParam);
//������ʾ��;
void DrawTipBox();
