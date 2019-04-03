#include "ImeInput.h"



ImeInput::ImeInput()
{
}
ImeInput::~ImeInput()
{
}

BOOL ImeInput::GetKeyState() {
	return ImeKeyState;
}
void ImeInput::SetKeyState(BOOL KeySt) {
	ImeKeyState = KeySt;
}
BOOL ImeInput::SetImeChar(const char*Keychar) {
	if (!Keychar)return FALSE;
	if (ImeStr.length() <= MaxLen) {
		memcpy(&ImeKeyChar, Keychar, sizeof(char));
		ImeStr.append(Keychar);
	}
	return TRUE;
}
BOOL ImeInput::SetImeStr(const char*Str) {
	if (!Str)return FALSE;
	if (ImeStr.length() <= MaxLen) {
		ImeStr.append(Str);
	}
	return TRUE;
}
std::string ImeInput::GetImeString() {
	return ImeStr;
}
void ImeInput::SetMaxLen(UINT Max) {
	MaxLen = Max;
}
void ImeInput::Clear() {
	ImeStr = "";
}
void ImeInput::DeleteImeStr() {
	if (ImeStr.length() > 0) {
		if (ImeStr.at(ImeStr.length() - 1) & 0x8000)
			ImeStr.erase(ImeStr.end() - 1);
		ImeStr.erase(ImeStr.end() - 1);
	}
}
void ImeInput::GetIMEString(HWND hWnd)
{
	if (ImeStr.length() >= MaxLen) {
		return;
	}
	HIMC hIMC = ImmGetContext(hWnd);//��ȡHIMC	
	if (hIMC)
	{
		//������˵��һ�£������롰�й���Ϊ��	
		//�л����������뷨�����롰zhongguo��������ַ�������IME����ַ���	
		//�������뷨�б���ѡ����ַ������й��������IME����ַ���	
		static bool flag = false;//������ɱ�ǣ���������ʱ��IME����ַ�����Ϊ�գ���true��������ɺ�IME����ַ���Ϊ�գ���false	
		DWORD dwSize = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);	//��ȡIME���������ַ����ĳ���	
		if (dwSize > 0)//���IME����ַ�����Ϊ�գ���û�д��󣨴�ʱdwSizeΪ��ֵ��������������ɱ��Ϊtrue	
		{
			if (flag == false)
			{
				flag = true;
			}
		}
		else if (dwSize == 0 && flag)	//���IME����ַ���Ϊ�գ����ұ��Ϊtrue�����ȡIME����ַ���	
		{
			int iSize;	//IME����ַ����Ĵ�С	
			LPSTR pszMultiByte = NULL;//IME����ַ���ָ��	
			int ChineseSimpleAcp = 936;//���ֽ�ת��ʱ���ĵı���	
			WCHAR *lpWideStr = NULL;//���ֽ��ַ�����	
			dwSize = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);//��ȡIME����ַ����Ĵ�С	
			if (dwSize > 0)	//���IME����ַ�����Ϊ�գ���û�д���	
			{
				dwSize += sizeof(WCHAR);//��СҪ����NULL������	
										//Ϊ��ȡIME����ַ�������ռ�	
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				lpWideStr = new WCHAR[dwSize];
				memset(lpWideStr, 0, dwSize);	//��ս���ռ�	
				ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpWideStr, dwSize);//��ȡIME����ַ����������ȡ���ǿ��ֽ�	
				iSize = WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, NULL, 0, NULL, NULL);//���㽫IME����ַ���ת��ΪASCII��׼�ֽں�Ĵ�С	
																									 //Ϊת������ռ�	
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
				pszMultiByte = new char[iSize + 1];
				WideCharToMultiByte(ChineseSimpleAcp, 0, lpWideStr, -1, pszMultiByte, iSize, NULL, NULL);//���ֽ�ת��	
				pszMultiByte[iSize] = '\0';
				ImeStr += pszMultiByte;//��ӵ�string��	
				while(ImeStr.length() > MaxLen) {
					if (ImeStr.at(ImeStr.length() - 1) & 0x8000)
						ImeStr.erase(ImeStr.end() - 1);
					ImeStr.erase(ImeStr.end() - 1);
				}
				//�ͷſռ�	
				if (lpWideStr)
				{
					delete[]lpWideStr;
					lpWideStr = NULL;
				}
				if (pszMultiByte)
				{
					delete[] pszMultiByte;
					pszMultiByte = NULL;
				}
			}
			flag = false;
		}
		ImmDestroyContext(hIMC);
		ImmReleaseContext(hWnd, hIMC);//�ͷ�HIMC	
	}
}
UINT ImeInput::GetImeLen() {
	return ImeStr.length();
}