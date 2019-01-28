#ifndef WINAPI_HPP_INCLUDED
#define WINAPI_HPP_INCLUDED

#include "GameModel.hpp"
#include <string>
#include <windows.h>

#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")

using std::string;

bool IsKey(int vKey);
bool IsKeyDown(int vKey);

void ResetCursor();

// Code from http://www.xuetang9.com
 // ���ÿ���̨����
void SetTitle(const char * title);
// ���ÿ���̨��ɫ
void SetColor(ConsoleColor);
// ���ݿ�����ÿ���̨���ڵĴ�С
void SetConsoleWindowSize();
// ���ù���x��y����-����������(x�����ң�y������)
void SetPosition(int x, int y);

// Code from https://blog.csdn.net/zuishikonghuan/article/details/47441163
string OpenFile(LPCSTR filter = "̹�˴�ս��ͼ�ļ�(*.bcm)\0*.bcm\0\0");
string SaveFile(LPCSTR filter = "̹�˴�ս��ͼ�ļ�(*.bcm)\0*.bcm\0\0");
//string OpenFile(string filter = "̰���ߵ�ͼ�ļ�\0*.vrs\0\0");
//string SaveFile(string filter = "̰���ߵ�ͼ�ļ�\0*.vrs\0\0");

#endif