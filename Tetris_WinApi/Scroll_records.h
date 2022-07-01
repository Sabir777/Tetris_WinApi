#pragma once
#include <Windows.h>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <array>
#include <sstream>
#include "resource.h"
#include "Scroll_Parent.h"


using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);
using WSTR = std::wstring;
using STR = std::string;

const int colors{ 3 };
using ARR_COLOR_TABL = std::array<COLORREF, colors>;

class Scroll_records : public Scroll {
public:
	Scroll_records() = delete;
	Scroll_records(Scroll*);
	~Scroll_records();

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //����������� �������
	LRESULT CALLBACK application_proc(); //��������� ������� CALLBACK

	void create_window(); //������ ����, ���������� ����������
	HWND create_hwnd_window(); //������ ���� - ��������� HWND
	void show_window(); //�������� ����

	//��������� ��������� Windows
	int wm_vscroll();
	int wm_paint();
	
private:
	HWND m_hwnd{}; //���������� ����
	UINT m_uMsg{}; //��������� �� ������� ���������
	WPARAM m_wParam{}; //wParam �� ������� ���������
	LPARAM m_lParam{}; //lParam �� ������� ���������

	WSTR name_class_window{ L"Table_records" }; //����� ��� ������ ����
	WSTR name_header_window{ L"������� ��������" }; //����� ��������� ����
	int width_window{1000}; //������ ����
	int height_window{350}; //������ ����

	//������ ��� ������ ���������
	int nPage;		  //���������� ����� � ��������
	int nLines;		  //���������� ������� � ������� ��������
	int yChar{ 50 };        // ������ ������
	
	int n_font{ yChar }; //������ ������
	HFONT font;			 //��������� �����
	ARR_COLOR_TABL color_tabl = {
		RGB(50, 205, 50),	  //���� ������
		RGB(64, 125, 95),	  //���� �����
		RGB(25, 25, 112)      //���� ����
	};
	
	HBRUSH brush_backgroung;	  //�����
	int d_pen{3};				  //������� ����
	HPEN pen;					  //����

	Scroll* p_rec; //��������� �� ������� �����
};