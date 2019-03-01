#ifndef WINAPI_HPP_INCLUDED
#define WINAPI_HPP_INCLUDED

#include "GameColor.hpp"
#include <string>
#include <map>
#include <windows.h>

#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")

using std::string;

bool IsKey(int vKey);
bool IsKeyDown(int vKey);

class FilterModel
{
private:
	LPTSTR m_filter;
	std::map<string, string> m_hintToSuffix;
	void CreateFilter();
	void ReleaseFilter();
public:
	FilterModel(string hint, string suffix);
	FilterModel(const std::map<string, string> &hintToSuffix);
	FilterModel(std::map<string, string> &&hintToSuffix);
	~FilterModel();
	LPCTSTR ToFilter() const;
};
extern const FilterModel FILTER_BATTLECITY_MAP;

class WindowManager
{
	HANDLE m_handle;
public:
	// 设置控制台标题，根据宽高设置控制台窗口的大小，隐藏控制台光标
	void Initialize(const std::string &title, SHORT width, SHORT height);
	// 设置控制台颜色
	void SetColor(ConsoleColor color);
	// 设置光标的x、y坐标-正常坐标轴(x左至右，y上至下)
	void SetPosition(SHORT x, SHORT y);
public:
	~WindowManager() { instance = nullptr; }

	static WindowManager* Get() 
	{
		if (instance == nullptr)
			instance = new WindowManager;
		return instance;
	}
private:
	WindowManager();
	static WindowManager* instance;
};

class FileManager
{
private:
	static TCHAR PathBufferForWindows[MAX_PATH];
	HWND m_window;
public:
	static const TCHAR DirectorySeparator = '\\';
	static const std::string& DefaultPath;
private:
	static std::string GetFolderPath(int csidl);
	static std::string GenerateDefaultPath();
public:
	static bool Exist(const std::string &path);
	static void CreateDirectory(std::string &root, const std::string &directoryName);;
	static bool CreateFile(const std::string &path);
	static bool DeleteFile(const std::string &path);
private:
	void SetForBrowerFile(OPENFILENAME &ofn);
public:
	// Code from https://blog.csdn.net/zuishikonghuan/article/details/47441163
	bool BrowseFileForLoad(std::string &path, const FilterModel &filter = FILTER_BATTLECITY_MAP);
	bool BrowseFileForSave(std::string &path, const FilterModel &filter = FILTER_BATTLECITY_MAP);
public:
	FileManager();
	~FileManager() { instance = nullptr; }

	static FileManager* Get() 
	{
		if (instance == nullptr)
			instance = new FileManager;
		return instance;
	}
private:
	static FileManager* instance;
};

#endif