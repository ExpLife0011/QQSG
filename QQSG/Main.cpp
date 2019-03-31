#include "Main.h"
using namespace ResouceFile;
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	WNDCLASS WinClass = { 0 };
	WinClass.hInstance = hInstance;
	WinClass.lpszClassName = ClassName;
	WinClass.lpfnWndProc = WndProc;
	WinClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WinClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WinClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	int WinX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (CL_WinWidth / 2),
		WinY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (CL_WinHeight / 2);
	if (!RegisterClass(&WinClass))MessageBox(0, L"ע�ᴰ������ʧ��!", NULL, MB_OK);
	G_hWnd = CreateWindow(ClassName, L"���Ǽ�����������", 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		WinX, WinY, CL_WinWidth, CL_WinHeight, NULL, NULL, hInstance, NULL);
	if (!G_hWnd)MessageBox(0, L"��������ʧ��!", NULL, MB_OK);
	ShowWindow(G_hWnd, SW_SHOWNORMAL);
	UpdateWindow(G_hWnd);
	MSG msg = { 0 };
	while (GetMessage(&msg,NULL,NULL,NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char path[256] = { 0 };
	switch (uMsg)
	{
	case WM_CREATE: {
		//CoInitialize(nullptr);
		Ime.SetMaxLen(14);
		GetExePath(ExePath, sizeof(ExePath));
		//��ʼ������;
		if (!D2Dx9.InitD3D(hWnd, CL_WinWidth, CL_WinHeight, false)) { MessageBox(hWnd, L"��ʼ��D3Dʧ��!", nullptr, MB_OK); ExitProcess(NULL); }
		if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, hWnd, NULL)) { MessageBox(hWnd, L"��ʼ��BASS��Ƶ��ʧ��!", nullptr, MB_OK); ExitProcess(NULL); }
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		D3DDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
		//����BGM;
		PlayerBGM_Login("Login1.mp3");
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitDrawCreatePlayer, NULL, NULL, NULL);
		InitDrawCreatePlayer();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)D3DGameRun, NULL, NULL, NULL);
		/*strcat(path, "E:\\ProjectCode\\C&C++\\Game\\QQSG\\Debug\\ResData.db");
		char IT[512] = "E:\\ProjectCode\\C&C++\\Game\\QQSG\\Debug\\ResData.db";
		int ilen = compressBound(strlen(IT));
		char OT[1024] = { 0 };
		compress((Bytef*)OT, (uLongf*)&ilen, (Bytef*)IT, 1024);
		char OiT[1024] = { 0 };
		int olen = 1024;
		uncompress((Bytef*)OiT, (uLongf*)&olen, (Bytef*)OT, ilen);
		RNumber = ResData.GetResDataIndex(path, (void**)&ResFa);
		/ *Fir = LocalAlloc(LMEM_ZEROINIT, ResFa[1].pOriginSize);
		Lor = LocalAlloc(LMEM_ZEROINIT, ResFa[1].pDataSize);
		filr = fopen(path, "rb");
		fseek(filr, ResFa[1].pOffset, SEEK_SET);
		fread(Lor, ResFa[1].pDataSize, 1, filr);
		uncompress((Bytef*)Fir, (uLongf*)&ResFa[1].pOriginSize, (Bytef*)Lor, ResFa[1].pDataSize);* /
		//fclose(filr);
		//LocalFree(Imgdata);
		for (int i = 0; i < RNumber; i++) {
			OutputDebugStringA((char*)ResFa[i].pFileName);
			OutputDebugStringA("\n");
			char write[256] = "E:\\ProjectCode\\C&C++\\Game\\QQSG\\Debug\\New\\";
			Fir = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pOriginSize);
			Lor = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pDataSize);
			filr = fopen(path, "rb");
			fseek(filr, ResFa[i].pOffset, SEEK_SET);
			fread(Lor, ResFa[i].pDataSize, 1, filr);
			uncompress((Bytef*)Fir, (uLongf*)&ResFa[i].pOriginSize, (Bytef*)Lor, ResFa[i].pDataSize);
			fclose(filr);
			//GetExePathA(write, sizeof(write));
			strcat(write, (char*)ResFa[i].pFileName);
			CreateDirectoryEc(write);
			filr = fopen(write, "wb");
			if (!filr) {
				LocalFree(Fir);
				LocalFree(Lor);
				LocalFree(ResFa[i].pFileName);
				continue; }
			else {
				fwrite(Fir, ResFa[i].pOriginSize, 1, filr);
				fclose(filr);
				LocalFree(Fir);
				LocalFree(Lor);
				LocalFree(ResFa[i].pFileName);
			}
		}
		LocalFree(ResFa);
	}*/
		break;
	}
	case WM_CLOSE:
		BASS_Free();
		ExitProcess(0);
		break;
	case WM_KEYDOWN:
		Ime.SetKeyState(TRUE);
		ImeEvent(wParam);
		if (LOWORD(wParam) == VK_UP) {
			Countf++;
		}
		if (LOWORD(wParam) == VK_DOWN) {
			Countf--;
		}
		break;
	case WM_KEYUP:
		Ime.SetKeyState(FALSE);
		break;
	case WM_MOUSEMOVE:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//���뷨��������;
void ImeEvent(WPARAM wParam) {
	int Keych = LOWORD(wParam);
	if (Keych == '\b') {
		Ime.DeleteImeStr();
	}
	else {
		char chc[3] = { 0 };
		if (GetKeyState(VK_CAPITAL)) {
			chc[0] = Keych;
		}
		else {
			if (Keych >= 'A' && Keych <= 'Z') {
				chc[0] = Keych + 32;
			}
			else if (Keych >= VK_NUMPAD0 && Keych <= VK_NUMPAD9) {
				sprintf(chc, "%d", Keych - VK_NUMPAD0);
			}
			else if (Keych >= '0' && Keych <= '9') {
				sprintf(chc, "%d", Keych - '0');
			}
		}
		Ime.SetImeChar(chc);
	}
}
//���ŵ�¼��������
void PlayerBGM_Login(const char *MusicName) {
	char path[256] = { 0 };
	ResouceDataFile ResData;
	ResouceDataFile::ResDataIndexInfo *ResFa = nullptr;
	int RNumber = 0;
	GetExePathA(path, MAX_PATH);
	strcat(path, "Music.db");
	ResFa = ResData.GetResDataIndex(path);
	RNumber = ResData.GetResIndexNumber();
	for (int i = 0; i < RNumber; i++) {
		//����ǵ�¼1��������,�򲥷�;
		char *pFileName = (char*)ResFa[i].pFileName;
		if(!pFileName)continue;
		if (strcmp((char*)ResFa[i].pFileName, MusicName) == 0) {
			//�����ڴ���Ƶ��������
			BGM_Login1 = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pOriginSize);
			if (!BGM_Login1) {
				MessageBox(0, L"�ڴ治��,����Ƶʧ��!", nullptr, MB_OK);
				ExitProcess(NULL);
			}
			//������ʱ��ѹ�ڴ���Ƶ��������;
			void *TmpAlloc = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pDataSize);
			if (!TmpAlloc) {
				MessageBox(0, L"�ڴ治��,����Ƶʧ��!", nullptr, MB_OK);
				ExitProcess(NULL);
			}
			//��ȡ��Ƶ�����ݴ����ڴ�
			FILE *pFile = fopen(path, "rb");
			if (!pFile) {
				MessageBox(0, L"��Ƶ��ʧ��,��ȷ���ļ��Ƿ�ʧ!", nullptr, MB_OK);
				ExitProcess(NULL);
			}
			fseek(pFile, ResFa[i].pOffset, SEEK_SET);
			fread(TmpAlloc, ResFa[i].pDataSize, 1, pFile);
			uLongf UnzipLen = ResFa[i].pDataSize;
			//��ѹ��Ƶ������;
			uncompress((Bytef*)BGM_Login1, &UnzipLen, (Bytef*)TmpAlloc, ResFa[i].pDataSize);
			//�ͷ��������Դ;
			LocalFree(TmpAlloc);
			fclose(pFile);
			StreamBGM1 = BASS_StreamCreateFile(TRUE, BGM_Login1, 0, UnzipLen, BASS_SAMPLE_LOOP);
			if (!StreamBGM1) {
				MessageBox(0, L"��Ƶ��ʧ��,��ȷ����Դ�ļ��Ƿ���!", nullptr, MB_OK);
				ExitProcess(NULL);
			}
			break;
		}
	}
	//������Ƶ
	BASS_ChannelPlay(StreamBGM1, TRUE);
}
//��Ϸ����
void D3DGameRun() {
	while (DrawGame)
	{
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		if (!D3DDevice9)return;
		D2Dx9.Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(D3DDevice9->BeginScene())) {
			ShowFPS();
			//������ʾ��;
			if (DrawTip) {
				DrawTipBox();
			}
			if (DrawCreatPlyer) {
				DrawCreatePlayer();
			}
			D3DDevice9->EndScene();
			D3DDevice9->Present(NULL, NULL, NULL, NULL);
		}
		Sleep(1);
	}

}
//��ʾFPS֡��:
void ShowFPS() {
	//����ÿ��FPS֡��;
	if (OldFpsTick == 0 || GetTickCount() - OldFpsTick > 1100) {
		OldFpsTick = GetTickCount();
		NewFPSCount = FPSCount;
		FPSCount = 0;
	}
	else FPSCount++;
	char FPSOut[50];
	sprintf(FPSOut, "FPS:%d", NewFPSCount);
	RECT lrect = { CL_WinWidth - 60,10,CL_WinWidth,50 };
	D2Dx9.DrawFont(FPSOut, 12, NULL, FALSE, "����", &lrect, D3DCOLOR_XRGB(255, 0, 0));
}
//������ʾ��;
void DrawTipBox() {
	UINT TipWidth = 500, TipHeight = 600, TipTop = 30;
	WCHAR szPath[MAX_PATH];
	GetExePath(szPath, sizeof(szPath));
	WCHAR szImg_Tip[MAX_PATH];
	wcscpy(szImg_Tip, szPath);
	wcscat(szImg_Tip, L"UIRes\\��¼UI\\��¼��ɫ\\��ʾ��-1.bmp");
	LPDIRECT3DTEXTURE9 TipBox = D2Dx9.LoadRectTexture(szImg_Tip, TipWidth, TipHeight);
	WCHAR SubmitT[3][256] = { L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-1.bmp",L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-2.bmp",L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-3.bmp" };
	LPDIRECT3DTEXTURE9 Submit[3] = { 0 };
	for (int i = 0; i < 3; i++) {
		wcscpy(szImg_Tip, szPath);
		wcscat(szImg_Tip, SubmitT[i]);
		Submit[i] = D2Dx9.LoadTexture(szImg_Tip);
	}
	D2Dx9.DrawTexture(TipBox, (float)TipWidth / 2, (float)TipTop);
	char TipT[] = "������Ϸ:";
	RECT lrect = { (LONG)TipWidth - 70,(LONG)TipTop + 5,CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawFont(TipT, 25, 10, TRUE, "����", &lrect, D3DCOLOR_XRGB(255, 0, 0));
	char TipText[] = "*.����Ϸ����ѹ�����Ϸ,ʹ�õ���Ϸ��Դ�󲿷�ȡ��\r\n<QQ����>��Ϸ.��:����ʹ�ñ���Ϸ����������;\r\n��������������!"
		"\r\n\r\n*.����Ϸ��<QQ����>���淨����,�ڲ����ݺ��淨ʵ��\r\n����ȫ��ͬ�޷�����<QQ����>�������!�����ע��."
		"\r\n\r\n*.����Ϸ���������뷨�޷��л�,���л������л�����\r\n�������л���Ϸ����"
		"\r\n\r\n*.��������Ϸ��Դ����ʧ��,��ʹ�ü���ģʽ���ҹ���\r\nԱ����."
		"\r\n\r\n*.����Ϸֻ����Ϸ������׷�������֮��,������\r\n<QQ����>����.";
	RECT lprect = { (LONG)TipWidth / 2 +10,(LONG)TipTop + 40,CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawTextFont(TipText, 20, 10, FALSE, "����", &lprect, D3DCOLOR_XRGB(0, 150, 230));
	if (MousePoint.x > (int)TipWidth - 35 && MousePoint.x < (int)TipWidth + 17 && MousePoint.y < (int)TipHeight - 20 && MousePoint.y >(int)TipHeight - 50) {
		if (MousePoint.KeyCode == MK_LBUTTON) { D2Dx9.DrawTexture(Submit[2], (float)TipWidth - 30, (float)TipHeight - 20); DrawTip = FALSE; DrawCreatPlyer = TRUE; }
		else D2Dx9.DrawTexture(Submit[0], (float)TipWidth - 30, (float)TipHeight - 20);
	}
	else {
		D2Dx9.DrawTexture(Submit[1], (float)TipWidth - 30, (float)TipHeight - 20);
	}
	TipBox->Release();
	for (int i = 0; i < 3; i++) {
		Submit[i]->Release();
	}
}
//��ʼ�����ƴ��������Դ;
void InitDrawCreatePlayer() {
	char szPath[MAX_PATH];
	GetExePathA(szPath, sizeof(szPath));
	strcat(szPath, "ResMap.map");
	FILE *pFile = fopen(szPath, "rb");
	if(!pFile) { MessageBox(0, L"��Դ������ʧ��!", NULL, MB_OK); ExitProcess(NULL); }
	ResouceDataFile Resdatfile;
	ResouceDataFile::ResMapOInfo *MpImginfo = Resdatfile.GetMapImageInfo(szPath);
	if (!MpImginfo)return;
	if (DrawImgInf.MaxInt > 0) { MessageBox(0, L"���������Դ����δ֪!", NULL, MB_OK); ExitProcess(NULL); }
	DrawImgInf.DrawMap = (PDrawMapInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(DrawMapInfo)*MpImginfo->MaxCount);
	DrawImgInf.MaxInt = MpImginfo->MaxCount;
	if(!DrawImgInf.DrawMap) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
	for (UINT i = 0; i < MpImginfo->MaxCount; i++) {
		ReadResMapInfo mpinf = MpImginfo->Mapinfo[i];
		PDrawMapInfo DrawMp = &DrawImgInf.DrawMap[i];
		DrawMp->x = mpinf.x;
		DrawMp->y = mpinf.y;
		DrawMp->width = mpinf.Width;
		DrawMp->height = mpinf.Height;
		DrawMp->Scale = mpinf.Scale;
		DrawMp->ImgLoadType = mpinf.ImgLoadType;
		DrawMp->rotation = 0.f;
		DrawMp->AnimateMaxCount = mpinf.AnimateCount;
		DrawMp->AnimateDelay = mpinf.AnimateDelay;
		DrawMp->AnimateOldTick = GetTickCount();
		DrawMp->AnimateTickIndex = 0;
		if (mpinf.ImgLoadType == Image) {
			Bytef *pUnzipAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, mpinf.ImageOriginSize);
			if (!pUnzipAlloc) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
			Bytef *pDataAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, mpinf.ImageDataSize);
			if (!pDataAlloc) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
			fseek(pFile, mpinf.ImageOffset, SEEK_SET);
			fread(pDataAlloc, 1, mpinf.ImageDataSize, pFile);
			UINT UnzipLen = mpinf.ImageOriginSize;
			uncompress(pUnzipAlloc, (uLongf*)&UnzipLen, pDataAlloc, mpinf.ImageDataSize);
			DrawMp->Texture = D2Dx9.LoadMemTexture(pUnzipAlloc, UnzipLen);
			DrawMp->ResAlloc = pUnzipAlloc;
			LocalFree(pDataAlloc);
		}
		else if (mpinf.ImgLoadType == Animate) {
			DrawMp->Animate = (PDrawMapInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(DrawMapInfo)*mpinf.AnimateCount);
			if (!DrawMp->Animate) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
			for (UINT n = 0; n < mpinf.AnimateCount; n++) {
				ReadResMapInfo Anminf = mpinf.Animate[n];
				PDrawMapInfo DrawMpAnimate = DrawMp->Animate;
				Bytef *pUnzipAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, Anminf.ImageOriginSize);
				if (!pUnzipAlloc) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
				Bytef *pDataAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, Anminf.ImageDataSize);
				if (!pDataAlloc) { MessageBox(0, L"�����ڴ�ʧ��,�ڴ治��!", NULL, MB_OK); ExitProcess(NULL); }
				fseek(pFile, Anminf.ImageOffset, SEEK_SET);
				fread(pDataAlloc, 1, Anminf.ImageDataSize, pFile);
				UINT UnzipLen = Anminf.ImageOriginSize;
				uncompress(pUnzipAlloc, (uLongf*)&UnzipLen, pDataAlloc, Anminf.ImageDataSize);
				/*char fgg[MAX_PATH];
				GetExePathA(fgg, MAX_PATH);
				char obt[50];
				sprintf(obt, "%d_%d.bmp", i, n);
				strcat(fgg, obt);
				FILE *fil = fopen(fgg, "wb");
				fwrite(pUnzipAlloc, UnzipLen, 1, fil);
				fflush(fil);
				fclose(fil);*/
				DrawMpAnimate[n].Texture = D2Dx9.LoadMemTexture(pUnzipAlloc, Anminf.ImageOriginSize);
				DrawMpAnimate[n].ResAlloc = pUnzipAlloc;
				DrawMpAnimate[n].x = Anminf.x;
				DrawMpAnimate[n].y = Anminf.y;
				DrawMpAnimate[n].width = Anminf.Width;
				DrawMpAnimate[n].height = Anminf.Height;
				DrawMpAnimate[n].Scale = Anminf.Scale;
				DrawMpAnimate[n].ImgLoadType = Anminf.ImgLoadType;
				DrawMpAnimate[n].rotation = 0.f;
				DrawMpAnimate[n].AnimateMaxCount = Anminf.AnimateCount;
				DrawMpAnimate[n].AnimateDelay = Anminf.AnimateDelay;
				LocalFree(pDataAlloc);
			}
		}
	}
	fclose(pFile);
}
//������ҽ�ɫ����;
void DrawCreatePlayer() {
	for (UINT i = 0; i < DrawImgInf.MaxInt; i++) {
		PDrawMapInfo DrwMp = &DrawImgInf.DrawMap[i];
		if (DrwMp->ImgLoadType == Image) {
			D2Dx9.DrawTexture(DrwMp->Texture, DrwMp->x, DrwMp->y, DrwMp->width, DrwMp->height, DrwMp->Scale, DrwMp->rotation);
		}
		else if (DrwMp->ImgLoadType == Animate) {
			if (GetTickCount() - DrwMp->AnimateOldTick > DrwMp->AnimateDelay) {
				DrwMp->AnimateTickIndex++;
				DrwMp->AnimateOldTick = GetTickCount();
				if (DrwMp->AnimateTickIndex >= DrwMp->AnimateMaxCount)
					DrwMp->AnimateTickIndex = 0;
			}
			DrawMapInfo DrwMpAn = DrwMp->Animate[DrwMp->AnimateTickIndex];
			D2Dx9.DrawTexture(DrwMpAn.Texture, DrwMpAn.x, DrwMpAn.y, DrwMpAn.width, DrwMpAn.height, DrwMpAn.Scale, DrwMpAn.rotation);
		}
	}
}
