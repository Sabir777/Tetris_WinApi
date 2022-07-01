#pragma once
#include <Windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "resource.h"


using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);
using WSTR = std::wstring;
using STR = std::string;


class Picture {
public:
	Picture();
	~Picture();

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //����������� �������
	LRESULT CALLBACK application_proc(); //��������� ������� CALLBACK

	void create_window(); //������ ����, ���������� ����������
	HWND create_hwnd_window(); //������ ���� - ��������� HWND
	void create_bitmap(); //������ ������
	void show_window(); //�������� ����
	void close(); //������ ����
	int wm_paint(); //�������� � ����
	
private:
	HWND m_hwnd{}; //���������� ����
	UINT m_uMsg{}; //��������� �� ������� ���������
	WPARAM m_wParam{}; //wParam �� ������� ���������
	LPARAM m_lParam{}; //lParam �� ������� ���������

	WSTR name_class_window{ L"Picture" }; //����� ��� ������ ����
	WSTR name_header_window{ L"����� ������!!!" }; //����� ��������� ����
	int width_window; //������ ����
	int height_window; //������ ����

	//�������
	HDC memBit; //����������� �������� � ������
	HANDLE hBitmap; //����������� bitmap
	HANDLE hOldBitmap; //�������� bitmap
	BITMAP Bitmap; //������� �������
};