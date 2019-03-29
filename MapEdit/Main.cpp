#include "Main.h"
#define ThreadMutexLockDeleay 3500
DirectX92D D2Dx9;
HWND G_hWnd;
BOOL DrawThread = TRUE;
WNDPROC OldDrawWndProc = nullptr, OldEditWndProc = nullptr;
MapImageInfo ImageInfo = { 0 };
MouseInfo MousePoint = { 0 };
HINSTANCE G_hInst;
HANDLE G_lpDrawThread = nullptr;
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
		UINT x = GetSystemMetrics(SM_CXSCREEN), y = GetSystemMetrics(SM_CYSCREEN);
		if (x < 1296 && y < 780) {
			if (MessageBox(hWnd, L"��Ļ�ֱ���С�ڴ��ڷֱ���,�����򿪿��ܵ��²��ִ����޷���ʾ!", NULL, MB_YESNO | MB_ICONERROR) == IDYES) {
				ExitProcess(NULL);
			}
		}
		HWND MapHwnd = GetDlgItem(hWnd, IDC_MapShow);
		MoveWindow(MapHwnd, 5, 3, 1024, 768, FALSE);
		G_hWnd = hWnd;
		GetClientRect(MapHwnd, &lRect);
		UINT Width = lRect.right - lRect.left;
		UINT Height = lRect.bottom - lRect.top;
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_IsRectangle), BM_SETCHECK, BST_CHECKED, NULL);
		SendMessage(GetDlgItem(hWnd, IDC_CHECK_PlayAnimate), BM_SETCHECK, BST_CHECKED, NULL);
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_LoadStyle), CB_ADDSTRING, NULL, (LPARAM)L"ͼƬģʽ");
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_LoadStyle), CB_ADDSTRING, NULL, (LPARAM)L"����ģʽ");
		SendMessage(GetDlgItem(hWnd, IDC_COMBO_LoadStyle), CB_SETCURSEL, 0, NULL);
		if (!D2Dx9.InitD3D(MapHwnd, Width, Height, false)) { MessageBox(hWnd, L"��ʼ��D3Dʧ��!", NULL, MB_OK); ExitProcess(NULL); }
		OldDrawWndProc = (WNDPROC)SetWindowLong(MapHwnd, GWL_WNDPROC, (LONG)DrawWndProc);
		OldEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hWnd, IDC_EDIT_Scale), GWL_WNDPROC, (LONG)EditScaleWndProc);
		DrawThread = TRUE;
		//��������ԭ��;
		GetWindowRect(G_hWnd, &GlRect);
		GetWindowRect(GetDlgItem(G_hWnd, IDC_MapShow), &ClRect);
		RunDrawD3D();
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
	case WM_MBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_MBUTTONDOWN;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_MBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_MBUTTONUP;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ImageFile:
			CallThreadFunction(ImportImage);
			break;
		case IDC_BUTTON_AnimateImage:
			CallThreadFunction(ImportAnimateImage);

			break;
		case ID_MENU_40001:
			CallThreadFunction(LayerUpImage);
			break;
		case ID_MENU_40002:
			CallThreadFunction(LayerNextImage);
			break;
		case ID_MENU_40003:
			CallThreadFunction(DeleteImageInfo);
			break;
		case ID_MENU_40004:
			CallThreadFunction(DeleteAllImage);
			break;
		case ID_MENU_40007:
			CallThreadFunction(LayerTopImage);
			break;
		case ID_MENU_40008:
			CallThreadFunction(LayerBottomImage);
			break;
		case ID_MENU_40009:
			CallThreadFunction(UpAnimateFrams);
			break;
		case ID_MENU_40010:
			CallThreadFunction(NextAnimateFrams);
			break;
		case ID_MENU_40011:
			CallThreadFunction(TopAnimateFrams);
			break;
		case ID_MENU_40012:
			CallThreadFunction(BottomAnimateFrams);
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
		//֪ͨ�����߳��˳�
		if (nFile > 0)CloseDrawD3D();
		else { DragFinish(hDropFile); break; }
		for (UINT i = 0; i < nFile; i++) {
			TCHAR szFileName[_MAX_PATH] = { 0 };
			if (DragQueryFile(hDropFile, i, szFileName, _MAX_PATH)) {
				if (ImageInfo.MaxImage > 0) {
					UINT MaxI = ImageInfo.MaxImage + 1;
					void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo)*MaxI);
					if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
					ZeroMemory(pAlloc, sizeof(ImageTexturInfo)*MaxI);
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
					ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Image;
				}
				else {
					void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo));
					if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
					ZeroMemory(pAlloc, sizeof(ImageTexturInfo));
					ImageInfo.Image = (PImageTexturInfo)pAlloc;
					ImageInfo.MaxImage = 1;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Texture = D2Dx9.LoadTexture(szFileName);
					CopyMemory(ImageInfo.Image[ImageInfo.MaxImage - 1].ImageFile, szFileName, _MAX_PATH);
					DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
					ImageInfo.Image[ImageInfo.MaxImage - 1].Width = ImgInfo.Width;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Height = ImgInfo.Height;
					ImageInfo.Image[ImageInfo.MaxImage - 1].Scale = 1.0f;
					ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
					ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Image;
				}
				OutputDebugString(szFileName);
				OutputDebugString(L"\n");
			}
		}
		DragFinish(hDropFile);
		RunDrawD3D();
		break;
	}
	default:
		break;
	}
	return CallWindowProc(OldDrawWndProc, hWnd, uMsg, wParam, lParam);
}
//D3D�����߳�;
UINT OldFpsTick = 0, FPSCount = 0, NewFPSCount = 0, SelectIndex = 0x8FFFFFF, SelectAnimateIndex = 0x8FFFFFF, CurrencyIndex = 0, UpIndex = 0x8FFFFFF;
MouseInfo BOMouse;
BOOL ISMove, IsTip;
void WINAPI DrawD3D() {
	while (DrawThread) {
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		if (!D3DDevice9)continue;
		D3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(D3DDevice9->BeginScene())) {
			UINT X = (ClRect.left - GlRect.left) - 10, Y = (ClRect.top - GlRect.top) - 32;
			for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
				if(!ImageInfo.Image)break;
				//�ж��Ƕ���֡���Ǳ���ͼƬ;
				if (ImageInfo.Image[i].ImgLoadType == Image) {
					if (MousePoint.x - X > ImageInfo.Image[i].x && MousePoint.x + X < ImageInfo.Image[i].x + ImageInfo.Image[i].Width
						&& MousePoint.y - Y > ImageInfo.Image[i].y && MousePoint.y + Y < ImageInfo.Image[i].y + ImageInfo.Image[i].Height) {
						if (!ISMove)CurrencyIndex = i;
					}
				}
				//����Ƕ���֡
				else if (ImageInfo.Image[i].ImgLoadType == Animate) {
					PAnimateImage pAnimat = &ImageInfo.Image[i].Animate[ImageInfo.Image[i].AnimateTickIndex];
					if (MousePoint.x - X > pAnimat->x && MousePoint.x - X < pAnimat->x + pAnimat->Width
						&& MousePoint.y - Y > pAnimat->y && MousePoint.y - Y < pAnimat->y + pAnimat->Height) {
						if (!ISMove)CurrencyIndex = i;
					}
				}
			}
			//�ж��Ƿ���ͼƬѡ����
			if (ImageInfo.MaxImage > CurrencyIndex) {
				if (!ImageInfo.Image)continue;
				if (ImageInfo.Image[CurrencyIndex].ImgLoadType == Image) {
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
				else if (ImageInfo.Image[CurrencyIndex].ImgLoadType == Animate) {
					AnimateImage pAnimat = ImageInfo.Image[CurrencyIndex].Animate[ImageInfo.Image[CurrencyIndex].AnimateTickIndex];
					if (MousePoint.x - X > pAnimat.x && MousePoint.x - X < pAnimat.x + pAnimat.Width
						&& MousePoint.y - Y > pAnimat.y && MousePoint.y - Y < pAnimat.y + pAnimat.Height) {
						if (MousePoint.KeyCode == MK_LBUTTON && MousePoint.KeyState == WM_LBUTTONDOWN) {
							if (SelectIndex != CurrencyIndex|| SelectAnimateIndex != ImageInfo.Image[CurrencyIndex].AnimateTickIndex) {
								//�Ƿ����ѡ��ͼƬ;
								if (SelectIndex < ImageInfo.MaxImage) {
									ImageInfo.Image[SelectIndex].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
									ImageInfo.Image[SelectIndex].HeightLight = FALSE;
								}
								ImageInfo.Image[CurrencyIndex].HeightLight = TRUE;
								ImageInfo.Image[CurrencyIndex].RectAngleColor = D3DCOLOR_XRGB(255, 0, 0);
								SelectIndex = CurrencyIndex;
								SelectAnimateIndex = ImageInfo.Image[CurrencyIndex].AnimateTickIndex;
								IsTip = TRUE;//�Ƿ���ʾһ�ε�ǰѡ��ͼƬ��Ϣ;
							}
							ISMove = TRUE;//�Ƿ�ѡ��һ��ͼƬ
							//��¼���ѡ���ǵ������
							if (BOMouse.OldTick == 0xF841F || BOMouse.OldTick == 0) {
								BOMouse.x = MousePoint.x - (int)pAnimat.x;
								BOMouse.y = MousePoint.y - (int)pAnimat.y;
								BOMouse.OldTick = 0x6F414;
							}
							//��������ƶ��ƶ�ѡ��ͼƬ����x;
							PAnimateImage pAnimat2 = &ImageInfo.Image[CurrencyIndex].Animate[ImageInfo.Image[CurrencyIndex].AnimateTickIndex];
							pAnimat2->x = (float)MousePoint.x - BOMouse.x;
							//����ͼƬ������������
							if (pAnimat2->x < 0) { 
								pAnimat2->x = 0; 
							}
							else if (pAnimat2->x > (lRect.right - lRect.left) - pAnimat2->Width)
								pAnimat2->x = (float)(lRect.right - lRect.left) - pAnimat2->Width;
							//��������ƶ��ƶ�ѡ��ͼƬ����y;
							pAnimat2->y = (float)MousePoint.y - BOMouse.y;
							//����ͼƬ������������
							if (pAnimat2->y < 0)pAnimat2->y = 0;
							else if (pAnimat2->y > (lRect.bottom - lRect.top) - pAnimat2->Height)
								pAnimat2->y = (float)(lRect.bottom - lRect.top) - pAnimat2->Height;
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
						if (MousePoint.KeyCode == MK_MBUTTON && MousePoint.KeyState == WM_MBUTTONDOWN) {
							//��¼���ѡ���ǵ������
							if (BOMouse.OldTick == 0xF841F || BOMouse.OldTick == 0) {
								BOMouse.x = MousePoint.x - (int)pAnimat.x;
								BOMouse.y = MousePoint.y - (int)pAnimat.y;
								BOMouse.OldTick = 0x6F414;
							}
							for (UINT i = 0; i < ImageInfo.Image[CurrencyIndex].AnimateMaxCout; i++) {
								//��������ƶ��ƶ�ѡ��ͼƬ����x;
								PAnimateImage pAnimat2 = &ImageInfo.Image[CurrencyIndex].Animate[i];
								pAnimat2->x = (float)MousePoint.x - BOMouse.x;
								//����ͼƬ������������
								if (pAnimat2->x < 0) {
									pAnimat2->x = 0;
								}
								else if (pAnimat2->x > (lRect.right - lRect.left) - pAnimat2->Width)
									pAnimat2->x = (float)(lRect.right - lRect.left) - pAnimat2->Width;
								//��������ƶ��ƶ�ѡ��ͼƬ����y;
								pAnimat2->y = (float)MousePoint.y - BOMouse.y;
								//����ͼƬ������������
								if (pAnimat2->y < 0)pAnimat2->y = 0;
								else if (pAnimat2->y > (lRect.bottom - lRect.top) - pAnimat2->Height)
									pAnimat2->y = (float)(lRect.bottom - lRect.top) - pAnimat2->Height;
							}
						}
					}
				}
			}
			//�ж��Ƿ��ͷ�ѡ��ͼƬ
			if (MousePoint.KeyState == WM_LBUTTONUP || MousePoint.KeyState == WM_RBUTTONUP|| MousePoint.KeyState == WM_MBUTTONUP) {
				ISMove = FALSE;
				BOMouse.x = 0;
				BOMouse.y = 0;
				BOMouse.OldTick = 0xF841F;
			}
			//��������ͼƬ
			for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
				if (ImageInfo.Image != nullptr) {
					if (ImageInfo.Image[i].ImgLoadType == Image) {
						//ѡ��ͼƬ�Ƿ������ʾ
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_HeightLight), BM_GETCHECK, 0, 0) == BST_CHECKED) {
							if (ImageInfo.Image[i].HeightLight)
								D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
							else D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale);
						}
						else D2Dx9.DrawTexture(ImageInfo.Image[i].Texture, ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].Width, ImageInfo.Image[i].Height, ImageInfo.Image[i].Scale);
						//ѡ��ͼƬ�߿��Ƿ���ʾ
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_IsRectangle), BM_GETCHECK, 0, 0) == BST_CHECKED)
							D2Dx9.DrawRectagle(ImageInfo.Image[i].x, ImageInfo.Image[i].y, ImageInfo.Image[i].x + ImageInfo.Image[i].Width, ImageInfo.Image[i].y + ImageInfo.Image[i].Height,
								1.0f, ImageInfo.Image[i].RectAngleColor);
					}
					else if (ImageInfo.Image[i].ImgLoadType == Animate) {
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_PlayAnimate), BM_GETCHECK, 0, 0) == BST_CHECKED) {
							if (GetTickCount() - ImageInfo.Image[i].OldAnimateTick > ImageInfo.Image[i].AnimateDelay) {
								ImageInfo.Image[i].OldAnimateTick = GetTickCount();
								ImageInfo.Image[i].AnimateTickIndex++;
							}
						}
						if (ImageInfo.Image[i].AnimateMaxCout <= ImageInfo.Image[i].AnimateTickIndex) {
							ImageInfo.Image[i].AnimateTickIndex = 0;
						}
						AnimateImage pAnimat = ImageInfo.Image[i].Animate[ImageInfo.Image[i].AnimateTickIndex];
						//ѡ��ͼƬ�Ƿ������ʾ
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_HeightLight), BM_GETCHECK, 0, 0) == BST_CHECKED) {
							if (ImageInfo.Image[i].HeightLight) {
								D2Dx9.DrawTexture(pAnimat.Texture, pAnimat.x, pAnimat.y, pAnimat.Width, pAnimat.Height, pAnimat.Scale, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
							}
							else D2Dx9.DrawTexture(pAnimat.Texture, pAnimat.x, pAnimat.y, pAnimat.Width, pAnimat.Height, pAnimat.Scale);
						}
						else D2Dx9.DrawTexture(pAnimat.Texture, pAnimat.x, pAnimat.y, pAnimat.Width, pAnimat.Height, pAnimat.Scale);
						//ѡ��ͼƬ�߿��Ƿ���ʾ
						if (SendMessage(GetDlgItem(G_hWnd, IDC_CHECK_IsRectangle), BM_GETCHECK, 0, 0) == BST_CHECKED)
							D2Dx9.DrawRectagle(pAnimat.x, pAnimat.y, pAnimat.x + pAnimat.Width, pAnimat.y + pAnimat.Height, 1.0f, ImageInfo.Image[i].RectAngleColor);
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
			sprintf(FPSOut, "FPS:%d,X:%dY:%d", NewFPSCount, MousePoint.x, MousePoint.y);
			D2Dx9.DrawFont(FPSOut, 12, NULL, FALSE, "����", &lRect, D3DCOLOR_XRGB(255, 0, 0));
			D3DDevice9->EndScene();
			D3DDevice9->Present(NULL, NULL, NULL, NULL);
			if (IsTip) {
				char ImgInfo[256] = { 0 };
				if (ImageInfo.Image[SelectIndex].ImgLoadType == Image) {
					sprintf(ImgInfo, "ͼƬX:%.2f\t\tͼƬY:%.2f\r\n\r\nͼƬ��:%d\t\tͼƬ��:%d\r\n\r\nͼƬ����:%d\t\tͼƬ����:%d\r\n", ImageInfo.Image[SelectIndex].x, ImageInfo.Image[SelectIndex].y,
						ImageInfo.Image[SelectIndex].Width, ImageInfo.Image[SelectIndex].Height, CurrencyIndex, ImageInfo.MaxImage);
					SetWindowTextA(GetDlgItem(G_hWnd, IDC_Label_Tip), ImgInfo);
					HWND EditHwnd = GetDlgItem(G_hWnd, IDC_EDIT_Path);
					SetWindowText(EditHwnd, ImageInfo.Image[SelectIndex].ImageFile);
					UINT Strlen = wcslen(ImageInfo.Image[SelectIndex].ImageFile);
					SendMessage(EditHwnd, EM_SETSEL, Strlen, Strlen);
					sprintf(ImgInfo, "%.2f", ImageInfo.Image[SelectIndex].Scale);
					SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_Scale), ImgInfo);
					SendMessage(GetDlgItem(G_hWnd, IDC_COMBO_LoadStyle), CB_SETCURSEL, 1, NULL);
				}
				else if (ImageInfo.Image[SelectIndex].ImgLoadType == Animate) {
					AnimateImage pAnimat = ImageInfo.Image[SelectIndex].Animate[ImageInfo.Image[SelectIndex].AnimateTickIndex];
					sprintf(ImgInfo, "ͼƬX:%.2f\t\tͼƬY:%.2f\r\n\r\nͼƬ��:%d\t\tͼƬ��:%d\r\n\r\n��������:%d\t\t������֡:%d\r\n\r\n����֡ʱ:%dms/ts\t", pAnimat.x, pAnimat.y,
						pAnimat.Width, pAnimat.Height, ImageInfo.Image[SelectIndex].AnimateTickIndex, ImageInfo.Image[SelectIndex].AnimateMaxCout, ImageInfo.Image[SelectIndex].AnimateDelay);
					SetWindowTextA(GetDlgItem(G_hWnd, IDC_Label_Tip), ImgInfo);
					HWND EditHwnd = GetDlgItem(G_hWnd, IDC_EDIT_Path);
					SetWindowText(EditHwnd, pAnimat.ImageFile);
					UINT Strlen = wcslen(pAnimat.ImageFile);
					SendMessage(EditHwnd, EM_SETSEL, Strlen, Strlen);
					sprintf(ImgInfo, "%.2f", pAnimat.Scale);
					SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_Scale), ImgInfo);
					SendMessage(GetDlgItem(G_hWnd, IDC_COMBO_LoadStyle), CB_SETCURSEL, 0, NULL);
				}
				/*BYTE *Rgb = (BYTE*)&ImageInfo.Image[SelectIndex].RectAngleColor;
				sprintf(ImgInfo, "%d", Rgb[2]);
				SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_ColorR), ImgInfo);
				sprintf(ImgInfo, "%d", Rgb[1]);
				SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_ColorG), ImgInfo);
				sprintf(ImgInfo, "%d", Rgb[0]);
				SetWindowTextA(GetDlgItem(G_hWnd, IDC_EDIT_ColorB), ImgInfo);*/
				IsTip = FALSE;
			}
		}
		else D3DDevice9->EndScene();
		Sleep(5);
	}
}
//ͼƬ���ű������ڴ�����;
HRESULT CALLBACK EditScaleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char Buf[50] = { 0 };
	float fScale;
	switch (uMsg)
	{
	case WM_KEYUP:
		GetWindowTextA(hWnd, Buf, sizeof(Buf));
		fScale = (float)atof(Buf);
		if (fScale > 0.0f) {
			if (ImageInfo.Image[SelectIndex].ImgLoadType == Image) {
				ImageInfo.Image[SelectIndex].Scale = fScale;
			}
			else if (ImageInfo.Image[SelectIndex].ImgLoadType == Animate) {
				ImageInfo.Image[SelectIndex].Animate[ImageInfo.Image[SelectIndex].AnimateTickIndex].Scale = fScale;
			}
		}
		break;
	default:
		break;
	}
	return CallWindowProc(OldEditWndProc, hWnd, uMsg, wParam, lParam);
}
//ɾ����ǰѡ��ͼƬ;
void WINAPI DeleteImageInfo() {
	if (ImageInfo.MaxImage <= 0)return;
	CloseDrawD3D();
	if (ImageInfo.Image[SelectIndex].Texture)
		ImageInfo.Image[SelectIndex].Texture->Release();
	
	for (UINT i = SelectIndex; i < ImageInfo.MaxImage - 1; i++) {
		memcpy(&ImageInfo.Image[i], &ImageInfo.Image[i + 1], sizeof(ImageTexturInfo));
	}
	ImageInfo.MaxImage--;
	if (ImageInfo.MaxImage <= 0) {
		LocalFree(ImageInfo.Image);
		ImageInfo = { 0 };
	}
	RunDrawD3D();
}
//ɾ������ͼƬ;
void WINAPI DeleteAllImage() {
	if (ImageInfo.MaxImage <= 0)return;
	CloseDrawD3D();
	for (UINT i = 0; i < ImageInfo.MaxImage; i++) {
		if (ImageInfo.Image[i].Texture)ImageInfo.Image[i].Texture->Release();
		ImageInfo.MaxImage--;
	}
	LocalFree(ImageInfo.Image);
	ImageInfo = { 0 };
	RunDrawD3D();
}
//��ͼƬ����һ��;
void WINAPI LayerUpImage() {
	if (ImageInfo.MaxImage < 2)return;
	if (SelectIndex > ImageInfo.MaxImage)return;
	CloseDrawD3D();
	ImageTexturInfo TmpImageInfo;
	CopyMemory(&TmpImageInfo, &ImageInfo.Image[SelectIndex], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex], &ImageInfo.Image[SelectIndex + 1], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex + 1], &TmpImageInfo, sizeof(ImageTexturInfo));
	RunDrawD3D();
}
//��ͼƬ����һ��;
void WINAPI LayerNextImage() {
	if (ImageInfo.MaxImage < 2)return;
	if (SelectIndex > ImageInfo.MaxImage)return;
	CloseDrawD3D();
	ImageTexturInfo TmpImageInfo;
	CopyMemory(&TmpImageInfo, &ImageInfo.Image[SelectIndex], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex], &ImageInfo.Image[SelectIndex - 1], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex - 1], &TmpImageInfo, sizeof(ImageTexturInfo));
	RunDrawD3D();
}
//��ͼƬ�ö���;
void WINAPI LayerTopImage() {
	if (ImageInfo.MaxImage < 2)return;
	if (SelectIndex > ImageInfo.MaxImage)return;
	CloseDrawD3D();
	ImageTexturInfo TmpImageInfo;
	CopyMemory(&TmpImageInfo, &ImageInfo.Image[SelectIndex], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex], &ImageInfo.Image[ImageInfo.MaxImage - 1], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[ImageInfo.MaxImage - 1], &TmpImageInfo, sizeof(ImageTexturInfo));
	RunDrawD3D();
}
//��ͼƬ�õײ�;
void WINAPI LayerBottomImage() {
	if (ImageInfo.MaxImage < 2)return;
	if (SelectIndex > ImageInfo.MaxImage)return;
	CloseDrawD3D();
	ImageTexturInfo TmpImageInfo;
	CopyMemory(&TmpImageInfo, &ImageInfo.Image[SelectIndex], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[SelectIndex], &ImageInfo.Image[0], sizeof(ImageTexturInfo));
	CopyMemory(&ImageInfo.Image[0], &TmpImageInfo, sizeof(ImageTexturInfo));
	RunDrawD3D();
}
//���������߳�;
void WINAPI RunDrawD3D() {
	DrawThread = TRUE;
	CloseHandle(G_lpDrawThread);
	G_lpDrawThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DrawD3D, NULL, NULL, NULL);
	if (!G_lpDrawThread) { MessageBox(NULL, L"�����߳�����ʧ��!", NULL, MB_OK); ExitProcess(NULL); }
}
//�رջ����߳�;
void WINAPI CloseDrawD3D() {
	//֪ͨ�����߳��˳�
	DrawThread = FALSE;
	//�ȴ��߳��˳�
	DWORD WaitObject = WaitForSingleObject(G_lpDrawThread, ThreadMutexLockDeleay);
	//����ȴ��̳߳���ʱ��,ǿ����ֹ�߳�;
	if (WaitObject != WAIT_OBJECT_0) {
		DWORD ExitCode;
		GetExitCodeThread(G_lpDrawThread, &ExitCode);
		TerminateThread(G_lpDrawThread, ExitCode);
	}
}
//���뱳��ͼƬ;
void WINAPI ImportImage() {
	OPENFILENAME open = { 0 };
	//�������ѡ���ļ�������;
	TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH];
	TCHAR *p = nullptr;
	int nLen = 0;
	open.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	open.lStructSize = sizeof(open);
	open.lpstrFile = szOpenFileNames;
	open.nMaxFile = sizeof(szOpenFileNames);
	open.lpstrFile[0] = '\0';
	open.lpstrFilter = TEXT("λͼ�ļ�(*.bmp | *.jpg | *.png)\0 * .jpg; *.bmp; *.png\0�����ļ�(*.*)\0 * .*\0");
	CloseDrawD3D();
	if (GetOpenFileName(&open)){
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		lstrcpyn(szPath, szOpenFileNames, open.nFileOffset);
		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		nLen = lstrlen(szPath);
		//���ѡ�˶���ļ�,��������'//'
		if (szPath[nLen - 1] != '\\'){
			lstrcat(szPath, TEXT("\\"));
		}
		//��ָ���Ƶ���һ���ļ�
		p = szOpenFileNames + open.nFileOffset; 
		while (*p)
		{
			ZeroMemory(szFileName, sizeof(szFileName));
			wcscpy(szFileName, szPath);
			wcscat(szFileName, p);
			p += lstrlen(p) + 1;     //������һ���ļ�
			OutputDebugString(szFileName);
			OutputDebugStringA("\n");
			if (ImageInfo.MaxImage > 0) {
				UINT MaxI = ImageInfo.MaxImage + 1;
				void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo)*MaxI);
				if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
				ZeroMemory(pAlloc, sizeof(ImageTexturInfo)*MaxI);
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
				ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Image;
			}
			else {
				void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo));
				if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
				ZeroMemory(pAlloc, sizeof(ImageTexturInfo));
				ImageInfo.Image = (PImageTexturInfo)pAlloc;
				ImageInfo.MaxImage = 1;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Texture = D2Dx9.LoadTexture(szFileName);
				CopyMemory(ImageInfo.Image[ImageInfo.MaxImage - 1].ImageFile, szFileName, _MAX_PATH);
				DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
				ImageInfo.Image[ImageInfo.MaxImage - 1].Width = ImgInfo.Width;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Height = ImgInfo.Height;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Scale = 1.0f;
				ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
				ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Image;
			}
		}
	}
	RunDrawD3D();
}
//���붯��ͼƬ;
void WINAPI ImportAnimateImage() {
	OPENFILENAME open = { 0 };
	//�������ѡ���ļ�������;
	TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szFileName[MAX_PATH];
	TCHAR *p = nullptr;
	int nLen = 0;
	open.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	open.lStructSize = sizeof(open);
	open.lpstrFile = szOpenFileNames;
	open.nMaxFile = sizeof(szOpenFileNames);
	open.lpstrFile[0] = '\0';
	open.lpstrFilter = TEXT("λͼ�ļ�(*.bmp | *.jpg | *.png)\0 * .jpg; *.bmp; *.png\0�����ļ�(*.*)\0 * .*\0");
	CloseDrawD3D();
	if (GetOpenFileName(&open)) {
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		lstrcpyn(szPath, szOpenFileNames, open.nFileOffset);
		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		nLen = lstrlen(szPath);
		//���ѡ�˶���ļ�,��������'//'
		if (szPath[nLen - 1] != '\\') {
			lstrcat(szPath, TEXT("\\"));
		}
		//��ָ���Ƶ���һ���ļ�
		p = szOpenFileNames + open.nFileOffset;
		if (ImageInfo.MaxImage > 0) {
			UINT MaxI = ImageInfo.MaxImage + 1;
			void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo)*MaxI);
			if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
			ZeroMemory(pAlloc, sizeof(ImageTexturInfo)*MaxI);
			CopyMemory(pAlloc, ImageInfo.Image, sizeof(ImageTexturInfo)*ImageInfo.MaxImage);
			LocalFree(ImageInfo.Image);
			ImageInfo.Image = (PImageTexturInfo)pAlloc;
			ImageInfo.MaxImage = MaxI;
			ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
			ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Animate;
			ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateDelay = 200;
			ImageInfo.Image[ImageInfo.MaxImage - 1].OldAnimateTick = GetTickCount();
		}
		else {
			void *pAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(ImageTexturInfo));
			if (!pAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
			ZeroMemory(pAlloc, sizeof(ImageTexturInfo));
			ImageInfo.Image = (PImageTexturInfo)pAlloc;
			ImageInfo.MaxImage = 1;
			ImageInfo.Image[ImageInfo.MaxImage - 1].RectAngleColor = D3DCOLOR_XRGB(255, 255, 255);
			ImageInfo.Image[ImageInfo.MaxImage - 1].ImgLoadType = Animate;
			ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateDelay = 200;
			ImageInfo.Image[ImageInfo.MaxImage - 1].OldAnimateTick = GetTickCount();
		}
		while (*p)
		{
			ZeroMemory(szFileName, sizeof(szFileName));
			wcscpy(szFileName, szPath);
			wcscat(szFileName, p);
			p += lstrlen(p) + 1;     //������һ���ļ�
			OutputDebugString(szFileName);
			OutputDebugStringA("\n");
			if (ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout > 0) {
				UINT MaxCount = ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout + 1;
				void *pAnimateAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(AnimateImage)*MaxCount);
				if (!pAnimateAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
				CopyMemory(pAnimateAlloc, ImageInfo.Image[ImageInfo.MaxImage - 1].Animate, sizeof(AnimateImage)*ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout);
				LocalFree(ImageInfo.Image[ImageInfo.MaxImage - 1].Animate);
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate = (PAnimateImage)pAnimateAlloc;
				ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout = MaxCount;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Texture = D2Dx9.LoadTexture(szFileName);
				wcscpy(ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].ImageFile, szFileName);
				DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Width = ImgInfo.Width;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Height = ImgInfo.Height;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Scale = 1.0f;
			}
			else {
				void *pAnimateAlloc = LocalAlloc(LMEM_ZEROINIT, sizeof(AnimateImage));
				if (!pAnimateAlloc) { MessageBox(NULL, L"�ڴ治��,�����ڴ�ʧ��!", NULL, NULL); ExitProcess(NULL); }
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate = (PAnimateImage)pAnimateAlloc;
				ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout++;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Texture = D2Dx9.LoadTexture(szFileName);
				wcscpy(ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].ImageFile, szFileName);
				DirectX92D::ImageInfo ImgInfo = D2Dx9.GetImageInfo(szFileName);
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Width = ImgInfo.Width;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Height = ImgInfo.Height;
				ImageInfo.Image[ImageInfo.MaxImage - 1].Animate[ImageInfo.Image[ImageInfo.MaxImage - 1].AnimateMaxCout - 1].Scale = 1.0f;
			}
		}
	}
	RunDrawD3D();
	OutputDebugStringA("����\n");
}
//��һ֡����;
void WINAPI UpAnimateFrams() {
	if (ImageInfo.MaxImage < 1)return;
	if (ImageInfo.Image[SelectIndex].ImgLoadType == Animate && ImageInfo.Image[SelectIndex].AnimateMaxCout > 1) {
		ImageInfo.Image[SelectIndex].AnimateTickIndex--;
		if (ImageInfo.Image[SelectIndex].AnimateTickIndex < 0) {
			ImageInfo.Image[SelectIndex].AnimateTickIndex = 0;
		}
	}
}
//��һ֡����;
void WINAPI NextAnimateFrams() {
	if (ImageInfo.MaxImage < 1)return;
	if (ImageInfo.Image[SelectIndex].ImgLoadType == Animate && ImageInfo.Image[SelectIndex].AnimateMaxCout > 1) {
		ImageInfo.Image[SelectIndex].AnimateTickIndex++;
		if (ImageInfo.Image[SelectIndex].AnimateTickIndex > ImageInfo.Image[SelectIndex].AnimateMaxCout) {
			ImageInfo.Image[SelectIndex].AnimateTickIndex = 0;
		}
	}
	return;
}
//�ö�֡����;
void WINAPI TopAnimateFrams(){
	if (ImageInfo.MaxImage < 1)return;
	ImageInfo.Image[SelectIndex].AnimateTickIndex = ImageInfo.Image[SelectIndex].AnimateMaxCout - 1;
	return ;
}
//�õ�֡����;
void WINAPI BottomAnimateFrams(){
	if (ImageInfo.MaxImage < 1)return;
	ImageInfo.Image[SelectIndex].AnimateTickIndex = 0;
	return ;
}
//����һ֡����;
void WINAPI SetUpAnimateFrams() {

}
//����һ֡����;
void WINAPI SetNextAnimateFrams() {

}
//�����߳�
void WINAPI CallThreadFunction(void *FuncAddress) {
	HANDLE lThreadHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FuncAddress, NULL, NULL, NULL);
	CloseHandle(lThreadHandle);
}