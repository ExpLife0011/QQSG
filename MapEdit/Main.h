#pragma once
#include "../QQSG/Module.h"
#include "../QQSG/GameDirectX9.h"
#include "../QQSG/ResourceFile.hpp"
#include "resource.h"
#include <string.h>
#include <windows.h>
#include <windowsx.h>
#include <mutex>
using namespace ResouceFile;
#pragma pack(push,1)
struct MouseInfo {
	int x;
	int y;
	int KeyCode;
	int KeyState;
	UINT OldTick;
};
#pragma pack(pop)
BOOL CALLBACK WndProcc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//D3D�����߳�;
void WINAPI DrawD3D();
//���ƴ��ڴ�����;
HRESULT CALLBACK DrawWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ͼƬ���ű������ڴ�����;
HRESULT CALLBACK EditScaleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ɾ����ǰѡ��ͼƬ;
void WINAPI DeleteImageInfo();
//ɾ������ͼƬ;
void WINAPI DeleteAllImage();
//��ͼƬ����һ��;
void WINAPI LayerUpImage();
//��ͼƬ����һ��;
void WINAPI LayerNextImage();
//��ͼƬ�ö���;
void WINAPI LayerTopImage();
//��ͼƬ�õײ�;
void WINAPI LayerBottomImage();
//���������߳�;
void WINAPI RunDrawD3D();
//�رջ����߳�;
void WINAPI CloseDrawD3D();
//���뱳��ͼƬ;
void WINAPI ImportImage();
//���붯��ͼƬ;
void WINAPI ImportAnimateImage();
//��һ֡����;
void WINAPI UpAnimateFrams();
//��һ֡����;
void WINAPI NextAnimateFrams();
//�ö�֡����;
void WINAPI TopAnimateFrams();
//�õ�֡����;
void WINAPI BottomAnimateFrams();
//����һ֡����;
void WINAPI SetUpAnimateFrams();
//����һ֡����;
void WINAPI SetNextAnimateFrams();
//�����߳�
void WINAPI CallThreadFunction(void *FuncAddress);