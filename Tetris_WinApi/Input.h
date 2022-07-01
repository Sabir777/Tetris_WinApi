#pragma once
#include <Windows.h>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <exception>
#include <stdexcept>
#include "Scroll_Parent.h"
#include "resource.h"


enum class Controls : int { BUTTON = 1001 , EDIT = 2000 };//��������


using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);
using WSTR = std::wstring;
using STR = std::string;

class Input : public Scroll {

	using Arr_Color = std::array<COLORREF, 2>;

public:
	Input() = delete;
	Input(Scroll*);
	~Input();

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //����������� �������
	LRESULT CALLBACK application_proc(); //��������� ������� CALLBACK

	void create_window(); //������ ����, ���������� ����������
	HWND create_hwnd_window(); //������ ���� - ��������� HWND
	void create_controls(); //������ ��������
	void show_window(); //�������� ����
	WSTR read(); //��������� ����� �� ���� �����

	//��������� �������
	int wm_command(); //������� ������
	int wm_close(WSTR name = L"�����������"); //�������� ������
	int wm_paint(); //�������� ������
	int wm_setcursor(); //�������� ���� ����� �� �������������� �������

private:
	HWND m_hwnd{}; //���������� ����
	UINT m_uMsg{}; //��������� �� ������� ���������
	WPARAM m_wParam{}; //wParam �� ������� ���������
	LPARAM m_lParam{}; //lParam �� ������� ���������

	WSTR name_class_window{ L"Input" }; //����� ��� ������ ����
	WSTR name_header_window{ L"����� ������!!!" }; //����� ��������� ����
	int width_window{ 520 }; //������ ����
	int height_window{ 200 }; //������ ����

	HWND hwnd_button{}; //���������� ������
	int x_pos_btn{ 155 }; //��������� x - ������
	int y_pos_btn{ 100 }; //��������� y - ������ 
	int width_button{ 200 }; //������ ������
	int height_button{ 30 }; //������ ������
	
	HWND hwnd_edit{}; //���������� ���� �����
	int x_pos_edit{ 100 }; //��������� x - ���� �����
	int y_pos_edit{ 50 }; //��������� y - ���� �����
	int width_edit{ 300 }; //������ ���� ��� �����
	int height_edit{ 25 }; //������ ���� ��� �����
	WSTR messange = L"         ������� ���� ��� �����...";
	bool flag_focus{ false };

	Arr_Color arr_color = {
		RGB(255, 215, 0), //���
		RGB(255, 0, 0) //�����
	};
	HBRUSH brush{}; //�����

	int n_font{ 20 }; //������ ������
	HFONT font{};	//�����

	Scroll* p_rec; //��������� �� ������� �����
};