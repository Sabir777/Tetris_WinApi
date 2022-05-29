#include "Objects.h"

/*-------------------------------------------------------------
						 �����������
--------------------------------------------------------------*/
Objects::Objects(HWND& hwnd, UINT& uMsg, WPARAM& wParam, LPARAM& lParam) 
	: hwnd{ hwnd }, uMsg{ uMsg }, wParam{ wParam }, lParam{ lParam }
{
	brush_generation(); //������� �����
	pen_generation(); //������� �����
	font_generation(); //������� ������
}

/*-------------------------------------------------------------
						  ����������
--------------------------------------------------------------*/
Objects::~Objects() {
	for (auto& v : arr_brush) { //������ �����
		DeleteObject(v);
	}

	for (auto& v : arr_pen) { //������ ����������
		DeleteObject(v);
	}
	
	for (auto& v : arr_font) { //������ ������
		DeleteObject(v);
	}
}

/*-------------------------------------------------------------
						  ������ Field
---------------------------------------------------------------
						���������� ����
--------------------------------------------------------------*/
void Objects::field_paint(HDC hdc) {

	//����� ���������
	//������ ���������� �����
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(hdc, arr_pen[0]);
	Rectangle(hdc, sz.x1, sz.y1, sz.x2, sz.y2); //����� ��������� (����� ����������)

	//����� ���������� ����� �� ������� ������
	MoveToEx(hdc, sz.x3, sz.y3, NULL); //��������� ��������� ��������� �����
	LineTo(hdc, sz.x3, sz.y4); //�������� ����� �����

	//�����
	SelectObject(hdc, arr_pen[1]);
	
	//����� ������������ �����
	for (int i = 0; i < sz.x_n - 1; ++i) {
		MoveToEx( hdc, sz.a_ceil * (i + 1) + i + sz.dx, sz.y3, NULL ); //��������� ��������� ��������� �����
		LineTo( hdc, sz.a_ceil * (i + 1) + i + sz.dx, sz.y5 ); //�������� ����� �����
	}

	//����� �������������� �����
	for (int i = 0; i < sz.y_n - 1; ++i) {
		MoveToEx(hdc, sz.dx, sz.a_ceil * (i + 1) + i + sz.dy, NULL); //��������� ��������� ��������� �����
		LineTo(hdc, sz.x4, sz.a_ceil * (i + 1) + i + sz.dy); //�������� ����� �����
	}

	SelectObject(hdc, hOldBrush); //������ �������� �����
	SelectObject(hdc, hOldPen); //������ �������� ����
}


/*-------------------------------------------------------------
				���������� ������ �������� ����
--------------------------------------------------------------*/
void Objects::field_fill_rect_ceil(HDC hdc) { 

	auto flag_flash = game.get_flash();
	auto flash_row = game.get_row();
	RECT rect;
	

	for (int j = 0; j < sz.y_n; ++j) {
		for (int i = 0; i < sz.x_n; ++i) {	
			rect.left = sz.dx + i + i * sz.a_ceil;
			rect.top = sz.dy + j + j * sz.a_ceil;
			rect.right = sz.a_ceil + rect.left;
			rect.bottom = sz.a_ceil + rect.top;
			
			if (!fig.check_figure_xy(i, j)) { //�� ���������� ���� ��� ������

				if (flag_flash == Flash::ON && flash_row.find(j) != flash_row.end()) {
					FillRect(hdc, &rect, arr_brush[0]); //����� �������� ���� ������ ������
				}
				else {
					FillRect(hdc, &rect, arr_brush[ceils[j][i]]); //���������� ��������� ������ ����
				}
			}
		}
	}
}


/*-------------------------------------------------------------
						������� ������
--------------------------------------------------------------*/
void Objects::field_fill_side(HDC hdc) {

/*-------------------------------------------------------------
				  ������� ���� - ��������� ������
---------------------------------------------------------------
			 ������� ����� ���� - �������(���� ������)
--------------------------------------------------------------*/
	RECT rect_l;
	rect_l.left = sz.xb1;
	rect_l.top = sz.yb1 + sz.a_10;
	rect_l.right = rect_l.left + sz.a_10;
	rect_l.bottom = sz.yb2 - sz.a_10;;

	FillRect(hdc, &rect_l, arr_brush[1]); //����� ����� �������������

	RECT rect_u;
	rect_u.left = sz.xb1;
	rect_u.top = sz.yb1;
	rect_u.right = sz.xb2;
	rect_u.bottom = rect_u.top + sz.a_10;

	FillRect(hdc, &rect_u, arr_brush[1]); //����� ������� �������������
	
	RECT rect_r;
	rect_r.left = sz.xb2 - sz.a_10;
	rect_r.top = sz.yb1 + sz.a_10;
	rect_r.right = sz.xb2;
	rect_r.bottom = sz.yb2 - sz.a_10;

	FillRect(hdc, &rect_r, arr_brush[1]); //����� ������ �������������

	RECT rect_d;
	rect_d.left = sz.xb1;
	rect_d.top = sz.yb2 - sz.a_10;
	rect_d.right = sz.xb2;
	rect_d.bottom = sz.yb2;

	FillRect(hdc, &rect_d, arr_brush[1]); //����� ������ �������������


/*-------------------------------------------------------------
   ���������� ����� - ���� ���� (���� ������ �������� ������)
--------------------------------------------------------------*/

	if (fig.get_game_state() != Game_State::GAME_OVER) {
		RECT rect_bl;
		rect_bl.left = sz.xb1 + sz.a_10;
		rect_bl.top = sz.yb1 + sz.a_10;
		rect_bl.right = rect_bl.left + sz.a_ceil / 2 + 1;
		rect_bl.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_bl, arr_brush[2]); //����� ����� ������ �������������

		RECT rect_bu;
		rect_bu.left = rect_bl.right;
		rect_bu.top = sz.yb1 + sz.a_10;
		rect_bu.right = sz.xb2 - sz.a_10;
		rect_bu.bottom = rect_bu.top + sz.a_ceil / 2 + 1;

		FillRect(hdc, &rect_bu, arr_brush[2]); //����� ������� ������ �������������

		RECT rect_br;
		rect_br.left = sz.xb2 - sz.a_10 - sz.a_ceil / 2 - 1;
		rect_br.top = rect_bu.bottom;
		rect_br.right = sz.xb2 - sz.a_10;
		rect_br.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_br, arr_brush[2]); //����� ������ ������ �������������

		RECT rect_bd;
		rect_bd.left = rect_bl.right;
		rect_bd.top = sz.yb2 - sz.a_10 - sz.a_ceil / 2 - 1;
		rect_bd.right = rect_br.left;
		rect_bd.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_bd, arr_brush[2]); //����� ������ ������ �������������
	}

/*-------------------------------------------------------------
				  ������ ���� ���������� - �����
--------------------------------------------------------------*/
	RECT rect_u2;
	rect_u2.left = sz.xb1;
	rect_u2.top = sz.yb3;
	rect_u2.right = sz.xb2;
	rect_u2.bottom = sz.yb3 + sz.a_10;

	FillRect(hdc, &rect_u2, arr_brush[1]); //����� ������� �������������

	RECT rect_l2;
	rect_l2.left = sz.xb1;
	rect_l2.top = sz.yb3 + sz.a_10;
	rect_l2.right = sz.xb1 + sz.a_10;
	rect_l2.bottom = sz.yb4;

	FillRect(hdc, &rect_l2, arr_brush[1]); //����� ����� �������������
	
	RECT rect_d2;
	rect_d2.left = sz.xb1 + sz.a_10;
	rect_d2.top = sz.yb4 - sz.a_10;
	rect_d2.right = sz.xb2;
	rect_d2.bottom = sz.yb4;

	FillRect(hdc, &rect_d2, arr_brush[1]); //����� ������ �������������

	RECT rect_r2;
	rect_r2.left = sz.xb2 - sz.a_10;
	rect_r2.top = sz.yb3 + sz.a_10;
	rect_r2.right = sz.xb2;
	rect_r2.bottom = sz.yb4 - sz.a_10;

	FillRect(hdc, &rect_r2, arr_brush[1]); //����� ������ �������������

/*-------------------------------------------------------------
				  ���������� �������������
--------------------------------------------------------------*/
	RECT surplus;
	surplus.left = sz.xb1;
	surplus.top = sz.yb5;
	surplus.right = sz.xb2;
	surplus.bottom = sz.yb6;

	FillRect(hdc, &surplus, arr_brush[1]); //����� ������� �������������


/*-------------------------------------------------------------
			������� ���� - ��������� ������ - �����
--------------------------------------------------------------*/
	HPEN hOldPen = (HPEN)SelectObject(hdc, arr_pen[1]); //������� ������ ��������

	
	//����� ������������ �����
	for (int i = 0; i < sz.n_side; ++i) {
		MoveToEx(hdc, sz.a_ceil * (i + 1) + i + sz.xb1 + sz.a_10 - sz.a_ceil / 2, sz.yb1 + sz.a_10, NULL); //��������� ��������� ��������� �����
		LineTo(hdc, sz.a_ceil * (i + 1) + i + sz.xb1 + sz.a_10 - sz.a_ceil / 2, sz.yb2 - sz.a_10); //�������� ����� �����
	}


	//����� �������������� �����
	for (int i = 0; i < sz.n_side; ++i) {
		MoveToEx(hdc, sz.xb1 + sz.a_10, sz.a_ceil * (i + 1) + i + sz.yb1 + sz.a_10 - sz.a_ceil / 2, NULL); //��������� ��������� ��������� �����
		LineTo(hdc, sz.xb2 - sz.a_10, sz.a_ceil* (i + 1) + i + sz.yb1 + sz.a_10 - sz.a_ceil / 2); //�������� ����� �����
	}

	SelectObject(hdc, hOldPen); //������ �������� ����

/*-------------------------------------------------------------
					  ����� "Esc - �����"
--------------------------------------------------------------*/
	text_Esc(hdc);
}


/*-------------------------------------------------------------
				  ����� ������� "Esc - �����"
--------------------------------------------------------------*/
void Objects::text_Esc(HDC hdc) {
	RECT rect;

	rect.left = sz.xb1;
	rect.top = sz.yb6;
	rect.right = sz.xb2;
	rect.bottom = sz.y5;

	FillRect(hdc, &rect, arr_brush[1]); //����� ������� �������������

	HFONT old_hFont = (HFONT)SelectObject(hdc, arr_font[1]); //������������ ����� � �������� ����������
	SetTextColor(hdc, arr_color_brush[8]); //������������ ���� ������
	SetBkColor(hdc, arr_color_brush[1]); //������������ ���� ���� ��� �������

	DrawText(hdc, L"Esc - �����", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //������� ���������
	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 

}


/*-------------------------------------------------------------
					��������� ������� ������
--------------------------------------------------------------*/
void Objects::get_key_state() {
	left();
	right();
}

/*-------------------------------------------------------------
						C������ �����
--------------------------------------------------------------*/
void Objects::left() {
	static short old_state{ 0 };
	static Timer t1; //������ ��������� �������������
	static Timer t2; //������ ������� �������������
	const int n_t1{ 180 }; //����� ��������� �������������
	const int n_t2{ 40 }; //�������� �������

	short state = GetAsyncKeyState(VK_LEFT);
	
	if (state == -32767 && state != old_state) {
		t1.set_time_point();
		t2.set_time_point();
		fig.left();
	}

	if (state && t1.duration() >= n_t1 && t2.duration() >= n_t2) {
		t2.set_time_point();
		fig.left();
	}

	old_state = state;
}


/*-------------------------------------------------------------
						C������ ������
--------------------------------------------------------------*/
void Objects::right() {
	static short old_state{ 0 };
	static Timer t1; //������ ��������� �������������
	static Timer t2; //������ ������� �������������
	const int n_t1{ 180 }; //����� ��������� �������������
	const int n_t2{ 40 }; //�������� �������

	short state = GetAsyncKeyState(VK_RIGHT);

	if (state == -32767 && state != old_state) {
		t1.set_time_point();
		t2.set_time_point();
		fig.right();
	}

	if (state && t1.duration() >= n_t1 && t2.duration() >= n_t2) {
		t2.set_time_point();
		fig.right();
	}

	old_state = state;
}


/*-------------------------------------------------------------
						C������ �����
--------------------------------------------------------------*/
int Objects::up() {

if (((lParam >> 30) & 1) == 0) //���� ����� �������� ������� ���� �� ������!
fig.rotation();			   //������ ������������� ��� ��������� �������
return 0;
}


/*-------------------------------------------------------------
						C������ ����
--------------------------------------------------------------*/
int Objects::down() {

	if (((lParam >> 30) & 1) == 0) //���� ����� �������� ������� ���� �� ������!
		fig.fast_down();			   //������ ������������� ��� ��������� �������
	return 0;
}




/*-------------------------------------------------------------
				  ������� ������ �� �������
--------------------------------------------------------------*/
void Objects::down_figure() {
	fig.down();
}


/*-------------------------------------------------------------
				  ���������� ��������� ������
--------------------------------------------------------------*/
void Objects::update_figure() {
	fig.update_figure();
}

/*-------------------------------------------------------------
						��������� �����
--------------------------------------------------------------*/
void Objects::paint(HDC hdc) {

	field_paint(hdc); //����� ����
	field_fill_rect_ceil(hdc); //����������� ������
	field_fill_side(hdc); //����� ������� ������
	fig.paint(hdc); //����� ������
	fig.paint_next(hdc); //����� ��������� ������
	game.paint(hdc); //��������� ��������� ����
}

/*-------------------------------------------------------------
				  ������� ������ - ����� � �����
---------------------------------------------------------------
					     �������� ������
--------------------------------------------------------------*/
void Objects::brush_generation() {
	for (int i = 0; i < n_color_brush; ++i) {
		arr_brush[i] = CreateSolidBrush(arr_color_brush[i]);
	}
}

/*-------------------------------------------------------------
						�������� ������
--------------------------------------------------------------*/
void Objects::pen_generation() {
	for (int i = 0; i < n_color_pen; ++i) {
		arr_pen[i] = CreatePen(PS_SOLID, arr_color_pen[i].first, arr_color_pen[i].second);
	}
}

/*-------------------------------------------------------------
						�������� �������
--------------------------------------------------------------*/
void Objects::font_generation() {

	for (int i = 0; i < n_font; ++i) {
		arr_font[i] = //������ ����� �����
			CreateFont(size_font[i], 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
	}
}


/*-------------------------------------------------------------
						��������� ����
---------------------------------------------------------------
					  ������ ����� ����
--------------------------------------------------------------*/
void Objects::new_game() {
	game.new_game();
}


/*-------------------------------------------------------------
					��������� ��������� ����
--------------------------------------------------------------*/
void Objects::game_state() {
	game.game_state();
}

/*-------------------------------------------------------------
							�����
--------------------------------------------------------------*/
int Objects::pause() {

		if (((lParam >> 30) & 1) == 0) //���� ����� �������� ������� ���� �� ������!
			game.pause();			   //������ ������������� ��� ��������� �������
		return 0;
}