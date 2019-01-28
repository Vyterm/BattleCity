#include "winapi.hpp"

//�򿪱����ļ��Ի���
#include<Commdlg.h>
//��ȡ���ҵ��ĵ���·��
#include <ShlObj.h>

bool IsKey(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) == 0x8000;
}

bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

void ResetCursor()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	CONSOLE_CURSOR_INFO cci;                    //����̨�����Ϣ�ṹ����
	cci.dwSize = 1;                             //����С
	cci.bVisible = FALSE;                       //�Ƿ���ʾ���
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);      //����ָ������̨��Ļ����������С�Ϳɼ���
}

void SetTitle(const char * title)
{
	SetConsoleTitle(title);
}

void SetColor(ConsoleColor color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);                    //��ȡ��ǰ���ھ��
	SetConsoleTextAttribute(handle, short(color.fore) + short(color.back) * 0x10);//������ɫ
	//CloseHandle(handle);
}

void SetConsoleWindowSize()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//���ÿ���̨���ڵĻ�������С
	COORD size = { WIN_WIDTH, WIN_HEIGHT };
	SetConsoleScreenBufferSize(handle, size);
	//���ÿ���̨���ڴ�С,��������ǻ�������С-1
	SMALL_RECT rect = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };
	SetConsoleWindowInfo(handle, 1, &rect);
	//CloseHandle(handle);
}

void SetPosition(int x, int y)
{
	COORD cursorPosition;   //��������
	HANDLE handle;          //��ǰ���ھ��
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorPosition.X = x * 2;   //����x��y����
	cursorPosition.Y = y;
	SetConsoleCursorPosition(handle, cursorPosition);
	//CloseHandle(handle);
}

inline string GetDocumentPath()
{
	char homePath[1024] = { 0 };
	SHGetSpecialFolderPath(nullptr, homePath, 5, false);
	return homePath;
}

inline void InitOFN(OPENFILENAME &ofn, LPCSTR filter, string title)
{
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter = filter;//���ù���
	ofn.nFilterIndex = 1;//����������
	ofn.lpstrTitle = TEXT(title.c_str());//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrInitialDir = GetDocumentPath().c_str();//��ʼĿ¼Ϊ�ҵ��ĵ�
}

string OpenFile(LPCSTR filter)
{
	OPENFILENAME ofn = { 0 };
	InitOFN(ofn, filter, "��ѡ��һ���ļ�");

	TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFilename);//����������

	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��

	//while (!GetOpenFileName(&ofn))
	//	MessageBox(NULL, TEXT("��ѡ��һ���ļ�"), NULL, MB_ICONERROR);
	if (!GetOpenFileName(&ofn)) return "";
	
	//MessageBox(NULL, strFilename, TEXT("ѡ����ļ�"), 0);
	return strFilename;
}

string SaveFile(LPCSTR filter)
{
	OPENFILENAME ofn = { 0 };
	InitOFN(ofn, filter, "���浽");
	ofn.lpstrDefExt = TEXT("bcm");//Ĭ��׷�ӵ���չ��

	TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFilename);//����������

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������

	while (!GetSaveFileName(&ofn))
		MessageBox(NULL, TEXT("������һ���ļ���"), NULL, MB_ICONERROR);
	
	//MessageBox(NULL, strFilename, TEXT("���浽"), 0);
	return strFilename;
}