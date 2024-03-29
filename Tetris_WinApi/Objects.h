#pragma once
#include <Windows.h>
#include <array>
#include <utility>
#include "Options.h"
#include "Figure.h"
#include "Game.h"


using std::array;
using std::pair;
using ARRAY_10_20 = array<array<int, 10>, 20>;
using IC = pair<int, COLORREF>;

using ARR_COLOR = array<COLORREF, n_color_brush>;
using ARR_BRUSH = array<HBRUSH, n_color_brush>;

const int n_color_pen = 2;
using ARR_COLOR_PEN = array<pair<int, COLORREF>, n_color_pen>;
using ARR_PEN = array<HPEN, n_color_pen>;


class Objects {
public:
	Objects() = delete; //��� �� ����� ����������� ��� ����������!!!
	Objects(HWND&, UINT&, WPARAM&, LPARAM&);
	~Objects();

/*-------------------------------------------------------------
						 ������ Field
--------------------------------------------------------------*/
	void field_paint(HDC); //���������� ����
	void field_fill_rect_ceil(HDC); //���������� ������

/*-------------------------------------------------------------
				  ������� �������������� ������
--------------------------------------------------------------*/
	void field_fill_side(HDC); //����� ������� �������������� ������
	void text_Esc(HDC); //���� �������: "Esc - �����"
	
/*-------------------------------------------------------------
						 ������ Figure
--------------------------------------------------------------*/
	void get_key_state(); //��������� ��������� ����������
	void left(); //������� �����
	void right(); //������� ������
	int up(); //������� �����
	int down(); //������� ����
	void down_figure(); //������� ������ �� �������
	void update_figure(); //���������� ������

/*-------------------------------------------------------------
						��������� �����
--------------------------------------------------------------*/
	void paint(HDC);

/*-------------------------------------------------------------
			�������� ������, ���������� � �������
--------------------------------------------------------------*/
	void brush_generation();
	void pen_generation();
	void font_generation();

/*-------------------------------------------------------------
						��������� ����
---------------------------------------------------------------
					  ������ ����� ����
--------------------------------------------------------------*/
	int new_game_button(); //�� ������
	int new_game_menu(); //�� ����

/*-------------------------------------------------------------
					 ��������� ��������� ����
--------------------------------------------------------------*/
	void game_state();

/*-------------------------------------------------------------
							  �����
--------------------------------------------------------------*/
	int pause_esc(); //esc
	int pause_menu(); //����

/*-------------------------------------------------------------
				  �������� ������� ��������
--------------------------------------------------------------*/
	int show_records();

/*-------------------------------------------------------------
				  ��������� / �������� ����
--------------------------------------------------------------*/
	int mute();

/*-------------------------------------------------------------
							������
--------------------------------------------------------------*/
private:
	Size_Window sz; //������� ����
	HWND& hwnd; //������ �� ����� ����������
	UINT& uMsg; //������ �� ���������
	WPARAM& wParam; //������ �� wParam
	LPARAM& lParam; //������ �� lParam

/*-------------------------------------------------------------
						 ������� ����
--------------------------------------------------------------*/
	ARRAY_10_20 ceils{ {} };

/*-------------------------------------------------------------
			  ������ - ����������� � �����������
--------------------------------------------------------------*/
	Figure fig{ ceils, arr_brush, sz, &game }; 

/*-------------------------------------------------------------
						  ������ ����
--------------------------------------------------------------*/
	Game game{ &fig, ceils, arr_brush, arr_color_brush, arr_font, sz };

/*-------------------------------------------------------------
			������� ������ - �����, ����� � ������
--------------------------------------------------------------*/

	ARR_COLOR arr_color_brush = {
		RGB(16, 56, 79), // 0 - ���� ���� ����
		RGB(47, 79, 79), // 1 - ���� ������� ������
		RGB(9, 8, 59), //2 - ������ �����
		RGB(154, 205, 50), //3 - YellowGreen
		RGB(255, 215, 0), //4 - Gold
		RGB(220, 20, 60), //5 - Crimson - �������
		RGB(70, 130, 180), //6 - SteelBlue
		RGB(255, 20, 147), //7 - DeepPink
		RGB(184, 134, 11), //8 - DarkGoldenRod
		RGB(255, 127, 80) //9 - Coral
	};
	
	ARR_BRUSH arr_brush; //����� ������

	ARR_COLOR_PEN arr_color_pen = {
		IC{sz.a_edging, RGB(128, 128, 0)}, //���������
		IC{1, RGB(94, 94, 94)} //�����
	};
	ARR_PEN arr_pen; //����� ������

	AR_SIZE_FONT size_font = { 50, //����� 
							  25, //������� "Esc - �����"
							  10
	}; //������� �������
	AR_FONT arr_font{}; //��������� ������
};

