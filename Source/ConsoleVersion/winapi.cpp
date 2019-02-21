#include "winapi.hpp"
#include "GameDefines.hpp"

#include <stdio.h>
//�򿪱����ļ��Ի���
#include<Commdlg.h>
//��ȡ���ҵ��ĵ���·��
#include <ShlObj.h>
#include <io.h>
#include <direct.h>

const FilterModel FILTER_BATTLECITY_MAP = { "̹�˴�ս��ͼ�ļ�", "bcm" };

bool IsKey(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) == 0x8000;
}

bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

inline string GetDocumentPath()
{
	char homePath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(nullptr, homePath, 5, false);
	return homePath;
}

inline void InitOFN(OPENFILENAME &ofn, const FilterModel &filter, const string &title)
{
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = filter.ToFilter();//���ù���
	ofn.nFilterIndex = 1;//����������
	ofn.lpstrTitle = TEXT(title.c_str());//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.lpstrInitialDir = GetDocumentPath().c_str();//��ʼĿ¼Ϊ�ҵ��ĵ�
}

#pragma region Filter Model

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

#pragma endregion

#pragma region Window Manager

WindowManager* WindowManager::instance;

void WindowManager::Initialize(const std::string &title, SHORT width, SHORT height)
{
	//���ÿ���̨���ڵı���
	SetConsoleTitle(title.c_str());
	//���ÿ���̨���ڵĻ�������С
	SetConsoleScreenBufferSize(m_handle, { width, height });
	//���ÿ���̨���ڴ�С,��������ǻ�������С-1
	SMALL_RECT rect = { 0, 0, width - 1, height - 1 };
	SetConsoleWindowInfo(m_handle, 1, &rect);
	//ģ��SHIFT�����£��ر��������뷨
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	//���ؿ���̨���
	CONSOLE_CURSOR_INFO cci = { 1, FALSE };
	SetConsoleCursorInfo(m_handle, &cci);
}

void WindowManager::SetColor(ConsoleColor color)
{
	SetConsoleTextAttribute(m_handle, short(color.fore) + short(color.back) * 0x10);//������ɫ
}

void WindowManager::SetPosition(SHORT x, SHORT y)
{
	SetConsoleCursorPosition(m_handle, { x * 2,y });
}

WindowManager::WindowManager()
{
	m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

#pragma endregion

#pragma region File Manager

FileManager* FileManager::instance;

std::string FileManager::GetFolderPath(int csidl)
{
	SHGetFolderPath(nullptr, csidl, nullptr, SHGFP_TYPE_CURRENT, PathBufferForWindows);
	return std::string(PathBufferForWindows) + DirectorySeparator;
}

std::string FileManager::GenerateDefaultPath()
{
	std::string str = GetFolderPath(CSIDL_LOCAL_APPDATA);
	CreateDirectory(str, GAME_AUTHOR);
	CreateDirectory(str, GAME_BUNDLE);
	return str;
}

TCHAR FileManager::PathBufferForWindows[MAX_PATH];
const std::string& FileManager::DefaultPath = FileManager::GenerateDefaultPath();

bool FileManager::Exist(const std::string &path)
{
	return 0 != _access(path.c_str(), 0);
}

void FileManager::CreateDirectory(std::string & dest, const std::string & directoryName)
{
	dest += directoryName + DirectorySeparator;
	if (!Exist(dest))
		_mkdir(dest.c_str());
}

bool FileManager::CreateFile(const std::string &path)
{
	// ʹ��::����ȫ�������ռ�ĺ���
	auto file = ::CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	// �رվ����ֹ���ܵ�ɾ���ļ��������ܾ�����
	// �˺����������file��ֵ��˲���Ӱ�����ıȽ�
	CloseHandle(file);
	return INVALID_HANDLE_VALUE == file;
}

bool FileManager::DeleteFile(const std::string &path)
{
	if (!Exist(path)) return false;
	return ::DeleteFile(path.c_str());
}

void FileManager::SetForBrowerFile(OPENFILENAME &ofn)
{
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_window;
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = DefaultPath.c_str();
	ZeroMemory(PathBufferForWindows, sizeof(PathBufferForWindows));
	ofn.lpstrFile = PathBufferForWindows;
	ofn.nMaxFile = sizeof(PathBufferForWindows);
	//Ŀ¼������ڣ�����ֻ��ѡ��
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
}

bool FileManager::BrowseFileForLoad(std::string &path, const FilterModel &filter /*= FILTER_BATTLECITY_MAP*/)
{
	OPENFILENAME ofn = { 0 };
	SetForBrowerFile(ofn);
	ofn.lpstrFilter = filter.ToFilter();
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");
	//�ļ��������
	ofn.Flags |= OFN_FILEMUSTEXIST;

	bool isBrowse = GetOpenFileName(&ofn);
	path = isBrowse ? PathBufferForWindows : "";
	return isBrowse;
}

bool FileManager::BrowseFileForSave(std::string &path, const FilterModel &filter /*= FILTER_BATTLECITY_MAP*/)
{
	OPENFILENAME ofn = { 0 };
	SetForBrowerFile(ofn);
	ofn.lpstrFilter = filter.ToFilter();
	ofn.lpstrTitle = TEXT("���浽");
	//Ĭ��׷�ӵ���չ��
	ofn.lpstrDefExt = TEXT("bcm");
	//�����ļ�ǰ��������
	ofn.Flags |= OFN_OVERWRITEPROMPT;

	bool isBrowse = GetSaveFileName(&ofn);
	path = isBrowse ? PathBufferForWindows : "";
	return isBrowse;
}

FileManager::FileManager()
{
	m_window = nullptr;
}

#pragma endregion
