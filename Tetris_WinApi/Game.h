#pragma once
#include <Windows.h>
#include <array>
#include <exception>
#include <stdexcept> //std::runtime_error
#include <string>
#include <algorithm> //std::find
#include <vector>
#include "Parrent.h"

using WSTR = std::wstring;
using std::vector;

using std::array;
using ARRAY_10_20 = array<array<int, 10>, 20>;

const int n_color_brush = 10;
using ARR_COLOR = array<COLORREF, n_color_brush>;
using AR_BRUSH = array<HBRUSH, n_color_brush>;

const int n_font = 3;
using AR_SIZE_FONT = array<int, n_font>;
using AR_FONT = array<HFONT, n_font>;


class Game : public Parrent {
public:
	Game() = delete;
	Game(Parrent* pf, ARRAY_10_20& field, AR_BRUSH& brush, ARR_COLOR& color, AR_FONT& font, Size_Window& sz);
	~Game();

/*-------------------------------------------------------------
		�������� ��������� � ������ � �������� �������
--------------------------------------------------------------*/
	void create_bitmap();

/*-------------------------------------------------------------
					���������/������ �����
--------------------------------------------------------------*/
	void pause();

/*-------------------------------------------------------------
						������ ����� ����
--------------------------------------------------------------*/
	void new_game();

/*-------------------------------------------------------------
					��������� ��������� ����
--------------------------------------------------------------*/
	void game_state();

/*-------------------------------------------------------------
					��������� ��������� ����
--------------------------------------------------------------*/
	void create_button();

/*-------------------------------------------------------------
						  ������� ����
--------------------------------------------------------------*/
	void destroy_row(set<int>) override;

/*-------------------------------------------------------------
	  ��������� ����� ������� � ��������� �������� �����
---------------------------------------------------------------
				  ��������� ����� ��������
--------------------------------------------------------------*/
	void set_flash(set<int>) override;

/*-------------------------------------------------------------
					  ���������� �������
--------------------------------------------------------------*/
	void flash_off();

/*-------------------------------------------------------------
					  ��������� �������
--------------------------------------------------------------*/
	void flash_generation();

/*-------------------------------------------------------------
				�������� ������� ON-OFF (�������)
--------------------------------------------------------------*/
	Flash get_flash() override { return flash; }
	//������� �������� � ������� ��������� ������ ����

/*-------------------------------------------------------------
		 ������� ������ ����� ������� ������ ������
--------------------------------------------------------------*/
	set<int> get_row() override { return y_row_flash; }

/*-------------------------------------------------------------
	  ������� ����� ������ ��������� � ����������� �����
--------------------------------------------------------------*/
	void score_animation_on();

/*-------------------------------------------------------------
	�������� ����� ������ ��������� � ����������� �����
--------------------------------------------------------------*/
	void score_animation_off();

/*-------------------------------------------------------------
		 �������� - ����� ��������� � ����������� �����
--------------------------------------------------------------*/
	void score_animation(HDC);

/*-------------------------------------------------------------
					���������� ����� ����
--------------------------------------------------------------*/
	void set_score();

/*-------------------------------------------------------------
					���������� ������ ����
--------------------------------------------------------------*/
	void set_level();

/*-------------------------------------------------------------
					��������� �������� �����
--------------------------------------------------------------*/
	void paint_score(HDC);

/*-------------------------------------------------------------
				   ��������� ������ ���������
--------------------------------------------------------------*/
	void paint_level(HDC);

/*-------------------------------------------------------------
					 ��������� ���������
--------------------------------------------------------------*/
	void paint(HDC);

/*-------------------------------------------------------------
						PAUSE (���������)
--------------------------------------------------------------*/
	void paint_pause(HDC);

/*-------------------------------------------------------------
						PLAY (���������)
--------------------------------------------------------------*/
	void paint_play(HDC);

/*-------------------------------------------------------------
					  GAME_OVER (���������)
--------------------------------------------------------------*/
	void paint_game_over(HDC);

/*-------------------------------------------------------------
						�������� ����
--------------------------------------------------------------*/
	void update_window();

/*-------------------------------------------------------------
							������
--------------------------------------------------------------*/
	void play_sound();

/*-------------------------------------------------------------
							������
--------------------------------------------------------------*/
private:
	Parrent* p_fig; //��������� �� ������
	ARRAY_10_20& field; //����
	AR_BRUSH& brush; //�����
	ARR_COLOR& color; //�����
	AR_FONT& font; //������
	Size_Window& sz; //�������

	//�����
	HDC memBit; //����������� �������� � ������
	HANDLE hBitmap; //����������� bitmap
	HANDLE hOldBitmap; //�������� bitmap
	BITMAP Bitmap; //������� �������

	//�����
	HDC memBit2; //����������� �������� � ������
	HANDLE hBitmap2; //����������� bitmap
	HANDLE hOldBitmap2; //�������� bitmap
	BITMAP Bitmap2; //������� �������

	//������
	HWND hwnd_btn{}; //���������� ������

	//������� - �������� ���������� �����
	set<int> y_row_flash{}; //������ ����� ������� ������ ���������, �� ����� ���� ��� ������ ���������
	bool flag_flash{ false }; //���� ��������
	Timer tm; //������ �������
	const int n_t{150}; //������ ������� - 150 ��
	Flash flash{ Flash::OFF }; //��������� �������� - �������
	
	//�������� - ����� ��������� � ��������� ���������� ����� - ������� ������
	bool flag_score_animation{ false }; //���� - �����: ���������� ����������� ����� 
	Timer t_score; //������ �������
	const int n_t_score{ 1500 }; //����� ������ ��������� � ����������� �����

	//������� ����
	int score{ 0 }; //������� ����
	int count_row{ 0 }; //���������� ��������� �����
	
	//�������
	int level{ 0 }; 
};