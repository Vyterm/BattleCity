#include "winapi.hpp"

#include <stdio.h>
//�򿪱����ļ��Ի���
#include<Commdlg.h>
//��ȡ���ҵ��ĵ���·��
#include <ShlObj.h>

const FilterModel FILTER_BATTLECITY_MAP = { "̹�˴�ս��ͼ�ļ�", "bcm" };

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
	char homePath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(nullptr, homePath, 5, false);
	return homePath;
}

inline void InitOFN(OPENFILENAME &ofn, const FilterModel &filter, const string &title)
{
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter = filter.ToFilter();//���ù���
	ofn.nFilterIndex = 1;//����������
	ofn.lpstrTitle = TEXT(title.c_str());//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrInitialDir = GetDocumentPath().c_str();//��ʼĿ¼Ϊ�ҵ��ĵ�
}

string OpenFile(const FilterModel &filter)
{
	OPENFILENAME ofn = { 0 };
	string title = TEXT("��ѡ��һ���ļ�");
	InitOFN(ofn, filter, title);

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

string SaveFile(const FilterModel &filter)
{
	OPENFILENAME ofn = { 0 };
	string title = TEXT("���浽");
	InitOFN(ofn, filter, title);
	ofn.lpstrDefExt = TEXT("bcm");//Ĭ��׷�ӵ���չ��

	TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFilename);//����������

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������

	//while (!GetSaveFileName(&ofn))
	//	MessageBox(NULL, TEXT("������һ���ļ���"), NULL, MB_ICONERROR);
	if (!GetOpenFileName(&ofn)) return "";
	
	//MessageBox(NULL, strFilename, TEXT("���浽"), 0);
	return strFilename;
}

void FilterModel::CreateFilter()
{
	ReleaseFilter();
	size_t size = 1;
	for (auto &hts : m_hintToSuffix)
		size += hts.first.size() + 3 + hts.second.size() + 4 + hts.second.size() + 1;
	m_filter = new TCHAR[size];
	size_t index = 0;
	for (auto &hts : m_hintToSuffix)
	{
		sprintf_s(m_filter + index, size - index, "%s(*.%s)\0", hts.first.c_str(), hts.second.c_str());
		index += hts.first.size() + 3 + hts.second.size() + 2;
		sprintf_s(m_filter + index, size - index, "*.%s\0", hts.second.c_str());
		index += 2 + hts.second.size() + 1;
	}
}

void FilterModel::ReleaseFilter()
{
	if (nullptr != m_filter)
		delete[] m_filter;
	m_filter = nullptr;
}

FilterModel::FilterModel(string hint, string suffix)
{
	m_hintToSuffix[hint] = suffix;
	CreateFilter();
}

FilterModel::FilterModel(const std::map<string, string>& hintToSuffix) : m_hintToSuffix(hintToSuffix)
{
	CreateFilter();
}

FilterModel::FilterModel(std::map<string, string>&& hintToSuffix)
	: m_hintToSuffix(hintToSuffix)
{
	CreateFilter();
}

FilterModel::~FilterModel()
{
	ReleaseFilter();
}

LPCTSTR FilterModel::ToFilter() const
{
	return m_filter;
}
