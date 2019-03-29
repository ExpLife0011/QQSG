#include "Main.h"
DirectX92D D2Dx9;
HWND G_hWnd;
BOOL DrawThread = TRUE;
WNDPROC OldWndProc = nullptr;
MapImageInfo ImageInfo = { 0 };
MouseInfo MousePoint = { 0 };
HINSTANCE G_hInst;
CRITICAL_SECTION MutexLock, ThreadMutexLock;
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	G_hInst = hInstance;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)WndProcc, NULL);
	return 0;
}
RECT GlRect, ClRect, lRect;
BOOL CALLBACK WndProcc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG: {
		G_hWnd = hWnd;
		HWND MapHwnd = GetDlgItem(hWnd, IDC_MapShow);
		GetClientRect(MapHwnd, &lRect);
		UINT Width = lRect.right - lRect.left;
		UINT Height = lRect.bottom - lRect.top;
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_IsRectangle), BM_SETCHECK, BST_CHECKED, NULL);
		if (!D2Dx9.InitD3D(MapHwnd, Width, Height, false)) { MessageBox(hWnd, L"��ʼ��D3Dʧ��!", NULL, MB_OK); ExitProcess(NULL); }
		OldWndProc = (WNDPROC)SetWindowLong(MapHwnd, GWL_WNDPROC, (LONG)DrawWndProc);
		DrawThread = TRUE;
		//��������ԭ��;
		GetWindowRect(G_hWnd, &GlRect);
		GetWindowRect(GetDlgItem(G_hWnd, IDC_MapShow), &ClRect);
		InitializeCriticalSection(&MutexLock);
		InitializeCriticalSection(&ThreadMutexLock);
		HANDLE lpDrawThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DrawD3D, NULL, NULL, NULL);
		if(!lpDrawThread){ MessageBox(hWnd, L"�����߳�����ʧ��!", NULL, MB_OK); ExitProcess(NULL); }
		CloseHandle(lpDrawThread);
		break;
	}
	case WM_CLOSE:
		DrawThread = FALSE;
		EndDialog(hWnd, NULL);
		ExitProcess(NULL);
		break;
	case WM_MOUSEMOVE:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_LBUTTONDOWN;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_RBUTTONDOWN;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_LBUTTONUP;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONUP:
		MousePoint.KeyState = WM_RBUTTONUP;
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		GetClientRect(GetDlgItem(G_hWnd, IDC_MapShow), &lRect);
		if (MousePoint.x > lRect.left && MousePoint.x < lRect.right &&
			MousePoint.y > lRect.top && MousePoint.y < lRect.bottom) {
			HMENU hMenu = LoadMenu(G_hInst, MAKEINTRESOURCE(IDR_MENU1));
			HMENU hSubMenu = GetSubMenu(hMenu, NULL);
			POINT Mouse;
			GetCursorPos(&Mouse);
			TrackPopupMenu(hSubMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, Mouse.x, Mouse.y, NULL, hWnd, NULL);
			DestroyMenu(hMenu);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MENU_40003:
			DeleteImageInfo();
			break;
		case ID_MENU_40004:
			DeleteAllImage();
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
//���ƴ��ڴ�����;
HRESULT CALLBACK DrawWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_NCHITTEST:
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_DROPFILES: {
		HDROP hDropFile = (HDROP)wParam;
		UINT nFile = DragQueryFile(hDropFile, -1, NULL, 0);
		if (nFile <= 0){ DragFinish(hDropFile); break; }
		EnterCriticalSection(&ThreadMutexLock);
		for (UINT i = 0; i < nFile; i++) {
			TCHAR szFileName[_MAX_PATH] = { 0 };
			if (DragQueryFile(hDropFile, i, szFileName, _MAX_PATH)) {
				if (ImageInfo.MaxImage > 0) {
					UINT MaxI = ImageInfo.MaxImage + 1;
					void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo)*MaxI);
					if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
					CopyMemory(pAlloc, ImageInfo.Image, sizeof(ImageTexturInfo)*ImageInfo.MaxImage);
					LocalFree(ImageInfo.Image);
					ImageInfo.Image = (PImageTexturInfo)pAlloc;
					ImageInfo.MaxImage = MaxI;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Texture = D2Dx9.LoadTexture(szFileName);
					CopyMemory(ImageInfo.Image[ImageInfo.MaxImage - 1].ImageFile, szFileName, _MAX_PATH);
					DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
					ImageInfo.Image[ImageInfo.MaxImage - 1].Width = ImgInfo.Width;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Height = ImgInfo.Height;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Scale = 1.0f;
					ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
				}
				else {
					void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo));
					if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
					ImageInfo.Image = (PImageTexturInfo)pAlloc;
					ImageInfo.MaxImage = 1;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Texture = D2Dx9.LoadTexture(szFileName);
					CopyMemory(ImageInfo.Image[ImageInfo.MaxImage - 1].ImageFile, szFileName, _MAX_PATH);
					DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
					ImageInfo.Image[ImageInfo.MaxImage - 1].Width = ImgInfo.Width;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Height = ImgInfo.Height;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Scale = 1.0f;
					ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
				}
				OutputDebugString(szFileName);
				OutputDebugString(L"\n");
			}
		}
		LeaveCriticalSection(&ThreadMutexLock);
		DragFinish(hDropFile);
		break;
	}
	default:
		break;
	}
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}
//D3D�����߳�;
UINT OldFpsTick = 0, FPSCount = 0, NewFPSCount = 0, SelectIndex = 0x8FFFFFF, CurrencyIndex = 0, UpIndex = 0x8FFFFFF;
MouseInfo BOMouse;
BOOL ISMove, IsTip;
void WINAPI DrawD3D() {
	while (DrawThread) {
		EnterCriticalSection(&MutexLock);
		HANDLE UnLock = GetProp(G_hWnd, L"ThreadMutex-Lock");
		if (UnLock == (HANDLE)0x123456F)continue;
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		if (!D3DDevice9)return;
		D2Dx9.Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(D3DDevice9->BeginScene())) {
			UINT X = (ClRect.left - GlRect.left) - 8, Y = (ClRect.top - GlRect.top) - 31;
			for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
				if(!ImageInfo.Image)break;
				if (MousePoint.x - X > ImageInfo.Image[i].x && MousePoint.x - X < ImageInfo.Image[i].x + ImageInfo.Image[i].Width
					&& MousePoint.y - Y > ImageInfo.Image[i].y && MousePoint.y - Y < ImageInfo.Image[i].y + ImageInfo.Image[i].Height) {
					if (!ISMove)CurrencyIndex = i;
				}
			}
			//�ж��Ƿ���ͼƬѡ����
			if (ImageInfo.MaxImage > CurrencyIndex) {
				if (!ImageInfo.Image)continue;
				if (MousePoint.x - X > ImageInfo.Image[CurrencyIndex].x && MousePoint.x - X < ImageInfo.Image[CurrencyIndex].x + ImageInfo.Image[CurrencyIndex].Width
					&& MousePoint.y - Y > ImageInfo.Image[CurrencyIndex].y && MousePoint.y - Y < ImageInfo.Image[CurrencyIndex].y + ImageInfo.Image[CurrencyIndex].Height) {
					if (MousePoint.KeyCode == MK_LBUTTON && MousePoint.KeyState == WM_LBUTTONDOWN) {
						if (SelectIndex != CurrencyIndex) {
							//�Ƿ����ѡ��ͼƬ;
							if (SelectIndex < ImageInfo.MaxImage) {
								ImageInfo.Image[SelectIndex].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
								ImageInfo.Image[SelectIndex].HeightLight = FALSE;
							}
							ImageInfo.Image[CurrencyIndex].HeightLight = TRUE;
							ImageInfo.Image[CurrencyIndex].RectAngleColor = D3DCOLOR_XRGB(255, 0, 0);
							SelectIndex = CurrencyIndex;
							IsTip = TRUE;//�Ƿ���ʾһ�ε�ǰѡ��ͼƬ��Ϣ;
						}
						ISMove = TRUE;//�Ƿ�ѡ��һ��ͼƬ
						//��¼���ѡ���ǵ������
						if (BOMouse.OldTick == 0xF841F || BOMouse.OldTick == 0) {
							BOMouse.x = MousePoint.x - (int)ImageInfo.Image[CurrencyIndex].x;
							BOMouse.y = MousePoint.y - (int)ImageInfo.Image[CurrencyIndex].y;
							BOMouse.OldTick = 0x6F414;
						}
						//��������ƶ��ƶ�ѡ��ͼƬ����x;
						ImageInfo.Image[CurrencyIndex].x = (float)MousePoint.x - BOMouse.x;
						//����ͼƬ������������
						if (ImageInfo.Image[CurrencyIndex].x < 0)ImageInfo.Image[CurrencyIndex].x = 0;
						else if (ImageInfo.Image[CurrencyIndex].x > (lRect.right - lRect.left) - ImageInfo.Image[CurrencyIndex].Width)
							ImageInfo.Image[CurrencyIndex].x = (float)(lRect.right - lRect.left) - ImageInfo.Image[CurrencyIndex].Width;
						//��������ƶ��ƶ�ѡ��ͼƬ����y;
						ImageInfo.Image[CurrencyIndex].y = (float)MousePoint.y - BOMouse.y;
						//����ͼƬ������������
						if (ImageInfo.Image[CurrencyIndex].y < 0)ImageInfo.Image[CurrencyIndex].y = 0;
						else if (ImageInfo.Image[CurrencyIndex].y > (lRect.bottom - lRect.top) - ImageInfo.Image[CurrencyIndex].Height)
							ImageInfo.Image[CurrencyIndex].y = (float)(lRect.bottom - lRect.top) - ImageInfo.Image[CurrencyIndex].Height;
					}
					if (MousePoint.KeyCode == MK_RBUTTON && MousePoint.KeyState == WM_RBUTTONDOWN) {
						if (SelectIndex != CurrencyIndex) {
							//�Ƿ����ѡ��ͼƬ;
							if (SelectIndex < ImageInfo.MaxImage) {
								ImageInfo.Image[SelectIndex].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
								ImageInfo.Image[SelectIndex].HeightLight = FALSE;
							}
							ImageInfo.Image[CurrencyIndex].HeightLight = TRUE;
							ImageInfo.Image[CurrencyIndex].RectAngleColor = D3DCOLOR_XRGB(255, 0, 0);
							SelectIndex = CurrencyIndex;
							IsTip = TRUE;//�Ƿ���ʾһ�ε�ǰѡ��ͼƬ��Ϣ;
						}
					}
				}
			}
			//�ж��Ƿ��ͷ�ѡ��ͼƬ
			if (MousePoint.KeyState == WM_LBUTTONUP) {
				ISMove = FALSE;
				BOMouse.x = 0;
				BOMouse.y = 0;
				BOMouse.OldTick = 0xF841F;
			}
			//��������ͼƬ
			for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
				if (ImageInfo.Image != nullptr) {
					if (ImageInfo.Image[i].Texture) {
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_HeightLight), BM_GETCHECK, 0, 0) == BST_CHECKED) {
							if (ImageInfo.Image[i].HeightLight)
								D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
							else D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale);
						}
						else D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale);
						if(SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_IsRectangle), BM_GETCHECK, 0, 0) == BST_CHECKED)
							D2Dx9.DrawRectagle(ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].x + ImageInfo.Image[i].Width, ImageInfo.Image[i].y + ImageInfo.Image[i].Height,
								1.0f, ImageInfo.Image[i].RectAngleColor);
					}
				}
			}
			//��ʾ����֡��;
			if (OldFpsTick == 0 || GetTickCount() - OldFpsTick > 1100) {
				OldFpsTick = GetTickCount();
				NewFPSCount = FPSCount;
				FPSCount = 0;
			}
			else FPSCount++;
			char FPSOut[50];
			sprintf(FPSOut, "FPS:%d", NewFPSCount);
			D2Dx9.DrawFont(FPSOut, 12, NULL, FALSE, "����", &lRect, D3DCOLOR_XRGB(255, 0, 0));
			D3DDevice9->EndScene();
			D3DDevice9->Present(NULL, NULL, NULL, NULL);
			if (IsTip) {
				char ImgInfo[256] = { 0 };
				sprintf(ImgInfo, "ͼƬX:%.2f\tͼƬY:%.2f\r\n\r\nͼƬ��:%d\tͼƬ��:%d\r\n\r\nͼƬ����:%d\tͼƬ����:%d\t", ImageInfo.Image[SelectIndex].x, ImageInfo.Image[SelectIndex].y,
					ImageInfo.Image[SelectIndex].Width, ImageInfo.Image[SelectIndex].Height, CurrencyIndex, ImageInfo.MaxImage);
				HWND EditHwnd = GetDlgItem(G_hWnd, IDC_EDIT_Path);
				SetWindowText(EditHwnd, ImageInfo.Image[SelectIndex].ImageFile);
				SetWindowTextA(GetDlgItem(G_hWnd, IDC_Label_Tip), ImgInfo);
				UINT Strlen = wcslen(ImageInfo.Image[SelectIndex].ImageFile);
				sprintf(ImgInfo, "%.2f", ImageInfo.Image[SelectIndex].Scale);
				SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_Scale), ImgInfo);
				SendMessage(EditHwnd, EM_SETSEL, Strlen, Strlen);
				IsTip = FALSE;
			}
		}
		else D3DDevice9->EndScene();
		LeaveCriticalSection(&MutexLock);
		Sleep(5);
	}
}
//ɾ����ǰѡ��ͼƬ;
void WINAPI DeleteImageInfo() {
	if (ImageInfo.MaxImage <= 0)return;
	EnterCriticalSection(&ThreadMutexLock);
	SetProp(G_hWnd, L"ThreadMutex-Lock", (HANDLE)0x123456F);
	if (ImageInfo.Image[SelectIndex].Texture)ImageInfo.Image[SelectIndex].Texture->Release();
	for (UINT i = SelectIndex; i < ImageInfo.MaxImage - 1; i++) {
		memcpy(&ImageInfo.Image[i], &ImageInfo.Image[i + 1], sizeof(ImageTexturInfo));
	}
	ImageInfo.MaxImage--;
	if (ImageInfo.MaxImage <= 0) {
		LocalFree(ImageInfo.Image);
		ImageInfo = { 0 };
	}
	SetProp(G_hWnd, L"ThreadMutex-Lock", (HANDLE)0x654321F);
	LeaveCriticalSection(&ThreadMutexLock);
}
//ɾ������ͼƬ;
void WINAPI DeleteAllImage() {
	if (ImageInfo.MaxImage <= 0)return;
	EnterCriticalSection(&ThreadMutexLock);
	SetProp(G_hWnd, L"ThreadMutex-Lock", (HANDLE)0x123456F);
	for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
		if (ImageInfo.Image[i].Texture)ImageInfo.Image[i].Texture->Release();
		ImageInfo.MaxImage--;
	}
	LocalFree(ImageInfo.Image);
	ImageInfo = { 0 };
	SetProp(G_hWnd, L"ThreadMutex-Lock", (HANDLE)0x654321F);
	LeaveCriticalSection(&ThreadMutexLock);
}