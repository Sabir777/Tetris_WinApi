#include "Game.h"

/*-------------------------------------------------------------
				   ����������� � �����������
--------------------------------------------------------------*/
Game::Game(Parrent* pf, ARRAY_10_20& field, AR_BRUSH& brush, 
			ARR_COLOR& color, AR_FONT& font, Size_Window& sz)
	: Parrent(), p_fig{ pf }, field{ field }, brush{ brush },
	color{ color }, font{ font }, sz{ sz }
{ 
	create_bitmap(); //������ �������
}

/*-------------------------------------------------------------
						  ����������
--------------------------------------------------------------*/
Game::~Game() {

	//�����
	DeleteObject(SelectObject(memBit, hOldBitmap)); //��������� �������� � ������ ��������� ������
	DeleteDC(memBit); //������ ����������� ��������

	//�����
	DeleteObject(SelectObject(memBit2, hOldBitmap2)); //��������� �������� � ������ ��������� ������
	DeleteDC(memBit2); //������ ����������� ��������
}


/*-------------------------------------------------------------
		�������� ��������� � ������ � �������� �������
--------------------------------------------------------------*/
void Game::create_bitmap() {

	HWND hwnd = FindWindow(L"My_Window", nullptr); //����� ������� ����

	//�����
	hBitmap = LoadImage(NULL, L"cat.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //�������� ������
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap); //�������� ����������� ������������ bitmap'a

	//�����
	hBitmap2 = LoadImage(NULL, L"skull.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //�������� ������
	GetObject(hBitmap2, sizeof(BITMAP), &Bitmap2); //�������� ����������� ������������ bitmap'a

	HDC hdc = GetDC(hwnd); //������� �������� ����������

	//�����
	memBit = CreateCompatibleDC(hdc); //������� ����������� �������� ���������� � ������
	hOldBitmap = SelectObject(memBit, hBitmap); //������� ������ � ����������� ��������

	//�����
	memBit2 = CreateCompatibleDC(hdc); //������� ����������� �������� ���������� � ������
	hOldBitmap2 = SelectObject(memBit2, hBitmap2); //������� ������ � ����������� ��������

	ReleaseDC(hwnd, hdc); //���������� ���������
}


/*-------------------------------------------------------------
					���������/������ �����
--------------------------------------------------------------*/
void Game::pause() {
	p_fig->pause_figure();
	update_window(); //�������� ����
}

/*-------------------------------------------------------------
					  ������ ����� ����
--------------------------------------------------------------*/
void Game::new_game() {
	field = ARRAY_10_20{ {} }; //�������� ����
	score = 0; //������� ������� ����
	count_row = 0; //������� ���������� ��������� �����
	level = 0; //������� ������� ���������
	p_fig->init_new_figure(); //������� ����� ������
	DestroyWindow(hwnd_btn); //������� ������
	update_window(); //�������� ����
}

/*-------------------------------------------------------------
					��������� ��������� ����
--------------------------------------------------------------*/
void Game::game_state() {

/*-------------------------------------------------------------
				  �������� ������ ����� ����
--------------------------------------------------------------*/
	create_button();

/*-------------------------------------------------------------
	 ���������� ������� ��� ��� ������ ����� � ���� ������
--------------------------------------------------------------*/
	flash_off();

/*-------------------------------------------------------------
					  ��������� �������
--------------------------------------------------------------*/
	flash_generation();

/*-------------------------------------------------------------
			��������� �������� � ����������� �����
--------------------------------------------------------------*/
	score_animation_on();

/*-------------------------------------------------------------
			 ���������� �������� � ����������� �����
--------------------------------------------------------------*/
	score_animation_off();

/*-------------------------------------------------------------
					������� ������������ �����
--------------------------------------------------------------*/
	set_score();

/*-------------------------------------------------------------
					������� ������ ���������
--------------------------------------------------------------*/
	set_level();

/*-------------------------------------------------------------
						��������� ������
--------------------------------------------------------------*/
	play_sound();

}


/*-------------------------------------------------------------
						������� ������
--------------------------------------------------------------*/
void Game::create_button() {

	static Game_State old_gs{ Game_State::PLAY };
	Game_State gs = p_fig->get_game_state(); //��������� ��������� ����

	if (gs == Game_State::GAME_OVER && gs != old_gs) {

		HWND hwnd = FindWindow(L"My_Window", nullptr); //����� ������� ����

		try {
			// ������ ������ ����� ����
			hwnd_btn = CreateWindowEx(0, L"BUTTON", L"����� ����",
				WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, sz.x_bt, sz.y_bt, sz.w_bt, sz.h_bt,
				hwnd, reinterpret_cast<HMENU>(Button::NEW_GAME),
				nullptr, nullptr);

			if (!hwnd_btn)
				throw std::runtime_error("Error, can't create button - New Game!");

			ShowWindow(hwnd_btn, SW_SHOWDEFAULT); //����� ������
		}
		catch (const std::exception& e) {
			std::string expt_data = e.what();
			MessageBox(nullptr, std::wstring(begin(expt_data),
				end(expt_data)).c_str(), L"������", MB_ICONERROR | MB_OK);
			ExitProcess(EXIT_FAILURE); //���������� ������
		}
	}

	old_gs = gs;
}


/*-------------------------------------------------------------
					   ������� ������ ����
--------------------------------------------------------------*/
void Game::destroy_row(set<int> Y_cont) {

	ARRAY_10_20 temp_field{ {} };
	for (int j = sz.y_n - 1, jx = j; j >= 0; --j) {

		if (Y_cont.find(j) == Y_cont.end()) { //���� ������ ������� �� ����� (�� ��� � ������)
			temp_field[jx] = field[j]; //������� ��
			--jx;
		}
	}
	field = temp_field;	
}


/*-------------------------------------------------------------
	  ��������� ����� ������� � ��������� �������� �����
---------------------------------------------------------------
				  ��������� ����� ��������
--------------------------------------------------------------*/
void Game::set_flash(set<int> Y_cont) {
	y_row_flash = Y_cont;
	flag_flash = true;
	
	if (Y_cont.size() == 4) {
		tetris.stop();
		tetris.play();
	}
	else {
		clear_row.stop();
		clear_row.play();
	}
}


/*-------------------------------------------------------------
					  ���������� �������
--------------------------------------------------------------*/
void Game::flash_off() {
	Figure_State fs = p_fig->get_figure_state(); //��������� ��������� ������

	if (flag_flash && fs == Figure_State::DESTROY) {
		flag_flash = false; //�������� ���� ��������
		flash = Flash::OFF; //������ ��������

		update_window();
	}
}


/*-------------------------------------------------------------
					  ��������� �������
--------------------------------------------------------------*/
void Game::flash_generation() {

	if (flag_flash && tm.duration() >= n_t) {
		tm.set_time_point();
		if (flash == Flash::OFF) {
			flash = Flash::ON;
		}
		else {
			flash = Flash::OFF;
		}
		update_window();
	}
}


/*-------------------------------------------------------------
	  ������� ����� ������ ��������� � ����������� �����
--------------------------------------------------------------*/
void Game::score_animation_on() {
	static bool old_flag_flash{ false };

	if (flag_flash && old_flag_flash != flag_flash) {
		
		t_score.set_time_point();
		flag_score_animation = true;
		update_window();
	}
	old_flag_flash = flag_flash;
}

/*-------------------------------------------------------------
	�������� ����� ������ ��������� � ����������� �����
--------------------------------------------------------------*/
void Game::score_animation_off() {
	if (flag_score_animation && t_score.duration() >= n_t_score) {
		flag_score_animation = false;
		update_window();
	}
}


/*-------------------------------------------------------------
		 �������� - ����� ��������� � ����������� �����
--------------------------------------------------------------*/
void Game::score_animation(HDC hdc) {

	if (flag_score_animation) {
		auto messange = [&]() {
			switch (y_row_flash.size()) {
			case 1:
				return WSTR{ L"+100" };
			case 2:
				return WSTR{ L"+300" };
			case 3:
				return WSTR{ L"+700" };
			case 4:
				return WSTR{ L"+1500" };
			default:
				return WSTR{ L"" };
			}
		};

		RECT rect;
		rect.left = sz.xb1 + sz.a_10;
		rect.top = sz.yb3 + sz.a_10;
		rect.right = sz.xb2 - sz.a_10;
		rect.bottom = sz.yb4 - sz.a_10;

		FillRect(hdc, &rect, brush[2]); //����� ������ �������������

		HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //������������ ����� � �������� ����������
		SetTextColor(hdc, color[4]); //������������ ������ ���� ������
		SetBkColor(hdc, color[2]); //������������ ���� ���� ��� �������

		DrawText(hdc, messange().c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //������� ���������
		SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 
	}
}


/*-------------------------------------------------------------
					���������� ����� ����
--------------------------------------------------------------*/
void Game::set_score() {
	static bool old_flag_flash{ false };
	if (flag_flash && old_flag_flash != flag_flash) {
		int size = y_row_flash.size();
		auto plus_score = [&]() {
			switch (size) {
			case 1:
				return 100;
			case 2:
				return 300;
			case 3:
				return 700;
			case 4:
				return 1500;
			}
		};
		score += plus_score();
		count_row += size;

	}
	old_flag_flash = flag_flash;
}


/*-------------------------------------------------------------
					���������� ������ ����
--------------------------------------------------------------*/
void Game::set_level() {

	if (count_row >= 8) {
		count_row = 0;
		++level;

		auto set_time = [&]() {
			switch (level) {
			case 1:
				return 720; //720 ����������
			case 2:
				return 630;
			case 3:
				return 550;
			case 4:
				return 470;
			case 5:
				return 380;
			case 6:
				return 300;
			case 7:
				return 220;
			
			default: return 220;
			}
		};
		
		p_fig->set_time_down(set_time());
	}
}


/*-------------------------------------------------------------
					��������� �������� �����
--------------------------------------------------------------*/
void Game::paint_score(HDC hdc) {

	RECT rect{ sz.xb1, sz.yb2, sz.xb2, sz.yb3 };
	
	FillRect(hdc, &rect, brush[1]); //����� ��� - ���� ������

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[1]); //������������ ����� � �������� ����������
	SetTextColor(hdc, color[4]); //������������ ������ ���� ������
	SetBkColor(hdc, color[1]); //������������ ���� ���� ��� �������

	WSTR str = L"����: " + std::to_wstring(score);

	DrawText(hdc, str.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //������� ���������
	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 
}


/*-------------------------------------------------------------
				   ��������� ������ ���������
--------------------------------------------------------------*/
void Game::paint_level(HDC hdc) {

	RECT rect{ sz.xb1, sz.yb4, sz.xb2, sz.yb5 };

	FillRect(hdc, &rect, brush[1]); //����� ��� - ���� ������

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[1]); //������������ ����� � �������� ����������
	SetTextColor(hdc, color[5]); //������������ ������� ���� ������
	SetBkColor(hdc, color[1]); //������������ ���� ���� ��� �������

	WSTR str = L"�������: " + std::to_wstring(level);

	DrawText(hdc, str.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //������� ���������
	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 
}



/*-------------------------------------------------------------
					 ��������� ���������
--------------------------------------------------------------*/
void Game::paint(HDC hdc) {
	Game_State gs = p_fig->get_game_state();

	paint_score(hdc); //����
	paint_level(hdc); //�������

	switch (gs) {
	case Game_State::PAUSE:
		{
			paint_pause(hdc);
			return;
		}
	case Game_State::PLAY:
		{
			paint_play(hdc);
			score_animation(hdc);
			return;
		}
	case Game_State::GAME_OVER:
		{
			paint_game_over(hdc);
			return;
		}
	}
}

/*-------------------------------------------------------------
						��������� �����
--------------------------------------------------------------*/
void Game::paint_pause(HDC hdc) {
	RECT rect;
	
	rect.left = sz.xb1 + sz.a_10;
	rect.top = sz.yb3 + sz.a_10;
	rect.right = sz.xb2 - sz.a_10;
	rect.bottom = sz.yb4 - sz.a_10;
	
	FillRect(hdc, &rect, brush[2]); //����� ������ �������������

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //������������ ����� � �������� ����������
	SetTextColor(hdc, RGB(65, 224, 40)); //������������ ������� ���� ������
	SetBkColor(hdc, color[2]); //������������ ���� ���� ��� �������

	DrawText(hdc, L"�����", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //������� ���������
	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 

}

/*-------------------------------------------------------------
				 ���������� ��������� - Play
--------------------------------------------------------------*/
void Game::paint_play(HDC hdc) {
	
/*-------------------------------------------------------------
					����� �������� - �����
--------------------------------------------------------------*/
	if (!flag_score_animation) {
		StretchBlt(hdc, //���������� ��������� DC
			sz.xb1 + sz.a_10, //x - ����� ������� ���� ���������
			sz.yb3 + sz.a_10, //� - ����� ������� ���� ���������
			sz.a_side - 2 * sz.a_10, // ������ �������� ��������������
			sz.a_side - 2 * sz.a_10, // ������ �������� ��������������
			memBit, // ���������� ��������� DC
			0, 0, //���������� ������ �������� ���� ���������
			Bitmap.bmWidth, //������ �������
			Bitmap.bmHeight, //������ �������
			SRCCOPY //��� �������� - �����������
		);
	}
}


/*-------------------------------------------------------------
				����������� ��������� - Game over
--------------------------------------------------------------*/
void Game::paint_game_over(HDC hdc) {

/*-------------------------------------------------------------
				  ������ ������� - Game over
--------------------------------------------------------------*/
	RECT rect;

	rect.left = sz.xb1 + sz.a_10;
	rect.top = sz.yb1 + sz.a_10;
	rect.right = sz.xb2 - sz.a_10;
	rect.bottom = sz.yb2 - sz.a_10;

	FillRect(hdc, &rect, brush[2]); //����� ������ �������������

	rect.top += 15;
	HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //������������ ����� � �������� ����������
	SetTextColor(hdc, color[5]); //������������ ������� ���� ������
	SetBkColor(hdc, color[2]); //������������ ���� ���� ��� �������

	DrawText(hdc, L"Game over", -1, &rect,
		DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_WORDBREAK | DT_CENTER);

	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 

/*-------------------------------------------------------------
					����� �������� - �����
--------------------------------------------------------------*/
	StretchBlt(hdc, //���������� ��������� DC
		sz.xb1 + sz.a_10, //x - ����� ������� ���� ���������
		sz.yb3 + sz.a_10, //� - ����� ������� ���� ���������
		sz.a_side - 2 * sz.a_10, // ������ �������� ��������������
		sz.a_side - 2 * sz.a_10, // ������ �������� ��������������
		memBit2, // ���������� ��������� DC
		0, 0, //���������� ������ �������� ���� ���������
		Bitmap2.bmWidth, //������ �������
		Bitmap2.bmHeight, //������ �������
		SRCCOPY //��� �������� - �����������
	);
}

/*-------------------------------------------------------------
				 ������������ - �������� ����
--------------------------------------------------------------*/
void Game::update_window() {

	HWND hwnd = FindWindow(L"My_Window", nullptr); //����� ������� ����

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

/*-------------------------------------------------------------
							������
--------------------------------------------------------------*/
void Game::play_sound() {
	static Game_State old_gs = Game_State::GAME_OVER;
	
	Game_State gs = p_fig->get_game_state();

	if (gs != old_gs) {
		if (gs == Game_State::PLAY) {
			
			if (old_gs == Game_State::GAME_OVER) {
				twinpix.stop(); //�������� ���� - ��������������� � ������������ ��������������� 
				music.play_repeat(); //������� �������� ���� (c ��������) - �����������
			}
			else if (old_gs == Game_State::PAUSE) {
				music.resume();
			}
		}
		else if (gs == Game_State::PAUSE) {

			music.pause();
			game_over.stop();
			game_over.play(); //������� ���� ��������� ����
		}
		else if (gs == Game_State::GAME_OVER) {

			game_over.stop();
			game_over.play(); //������� ���� ��������� ����
			music.stop(); //�������� �������� ����
			twinpix.play_repeat(); //������� ����-���� ��� ��� ���� ���������
		}
	}
	old_gs = gs;
}