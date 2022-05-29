#include "Objects.h"

/*-------------------------------------------------------------
						 Конструктор
--------------------------------------------------------------*/
Objects::Objects(HWND& hwnd, UINT& uMsg, WPARAM& wParam, LPARAM& lParam) 
	: hwnd{ hwnd }, uMsg{ uMsg }, wParam{ wParam }, lParam{ lParam }
{
	brush_generation(); //создать кисти
	pen_generation(); //создать перья
	font_generation(); //создать шрифты
}

/*-------------------------------------------------------------
						  Деструктор
--------------------------------------------------------------*/
Objects::~Objects() {
	for (auto& v : arr_brush) { //удаляю кисти
		DeleteObject(v);
	}

	for (auto& v : arr_pen) { //удаляю фломастеры
		DeleteObject(v);
	}
	
	for (auto& v : arr_font) { //удаляю шрифты
		DeleteObject(v);
	}
}

/*-------------------------------------------------------------
						  Методы Field
---------------------------------------------------------------
						Прорисовка поля
--------------------------------------------------------------*/
void Objects::field_paint(HDC hdc) {

	//общая окантовка
	//создаю прозрачную кисть
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(hdc, arr_pen[0]);
	Rectangle(hdc, sz.x1, sz.y1, sz.x2, sz.y2); //рисую окантовку (кисть прозрачная)

	//рисую отделяющую линию от боковой панели
	MoveToEx(hdc, sz.x3, sz.y3, NULL); //перемещаю положение начальной точки
	LineTo(hdc, sz.x3, sz.y4); //указываю конец линии

	//сетка
	SelectObject(hdc, arr_pen[1]);
	
	//рисую вертикальные линии
	for (int i = 0; i < sz.x_n - 1; ++i) {
		MoveToEx( hdc, sz.a_ceil * (i + 1) + i + sz.dx, sz.y3, NULL ); //перемещаю положение начальной точки
		LineTo( hdc, sz.a_ceil * (i + 1) + i + sz.dx, sz.y5 ); //указываю конец линии
	}

	//рисую горизонтальные линии
	for (int i = 0; i < sz.y_n - 1; ++i) {
		MoveToEx(hdc, sz.dx, sz.a_ceil * (i + 1) + i + sz.dy, NULL); //перемещаю положение начальной точки
		LineTo(hdc, sz.x4, sz.a_ceil * (i + 1) + i + sz.dy); //указываю конец линии
	}

	SelectObject(hdc, hOldBrush); //вернул стоковую кисть
	SelectObject(hdc, hOldPen); //вернул стоковое перо
}


/*-------------------------------------------------------------
				Закрашиваю клетки игрового поля
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
			
			if (!fig.check_figure_xy(i, j)) { //не закрашиваю если там фигура

				if (flag_flash == Flash::ON && flash_row.find(j) != flash_row.end()) {
					FillRect(hdc, &rect, arr_brush[0]); //рисую мигающие ряды темным цветом
				}
				else {
					FillRect(hdc, &rect, arr_brush[ceils[j][i]]); //закрашиваю остальные клетки поля
				}
			}
		}
	}
}


/*-------------------------------------------------------------
						Боковая панель
--------------------------------------------------------------*/
void Objects::field_fill_side(HDC hdc) {

/*-------------------------------------------------------------
				  Верхнее окно - следующая фигура
---------------------------------------------------------------
			 Внешняя рамка окна - зеленая(цвет панели)
--------------------------------------------------------------*/
	RECT rect_l;
	rect_l.left = sz.xb1;
	rect_l.top = sz.yb1 + sz.a_10;
	rect_l.right = rect_l.left + sz.a_10;
	rect_l.bottom = sz.yb2 - sz.a_10;;

	FillRect(hdc, &rect_l, arr_brush[1]); //рисую левый прямоугольник

	RECT rect_u;
	rect_u.left = sz.xb1;
	rect_u.top = sz.yb1;
	rect_u.right = sz.xb2;
	rect_u.bottom = rect_u.top + sz.a_10;

	FillRect(hdc, &rect_u, arr_brush[1]); //рисую верхний прямоугольник
	
	RECT rect_r;
	rect_r.left = sz.xb2 - sz.a_10;
	rect_r.top = sz.yb1 + sz.a_10;
	rect_r.right = sz.xb2;
	rect_r.bottom = sz.yb2 - sz.a_10;

	FillRect(hdc, &rect_r, arr_brush[1]); //рисую правый прямоугольник

	RECT rect_d;
	rect_d.left = sz.xb1;
	rect_d.top = sz.yb2 - sz.a_10;
	rect_d.right = sz.xb2;
	rect_d.bottom = sz.yb2;

	FillRect(hdc, &rect_d, arr_brush[1]); //рисую нижний прямоугольник


/*-------------------------------------------------------------
   Внутренняя рамка - цвет поля (поле показа следущей фигуры)
--------------------------------------------------------------*/

	if (fig.get_game_state() != Game_State::GAME_OVER) {
		RECT rect_bl;
		rect_bl.left = sz.xb1 + sz.a_10;
		rect_bl.top = sz.yb1 + sz.a_10;
		rect_bl.right = rect_bl.left + sz.a_ceil / 2 + 1;
		rect_bl.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_bl, arr_brush[2]); //рисую левый черный прямоугольник

		RECT rect_bu;
		rect_bu.left = rect_bl.right;
		rect_bu.top = sz.yb1 + sz.a_10;
		rect_bu.right = sz.xb2 - sz.a_10;
		rect_bu.bottom = rect_bu.top + sz.a_ceil / 2 + 1;

		FillRect(hdc, &rect_bu, arr_brush[2]); //рисую верхний черный прямоугольник

		RECT rect_br;
		rect_br.left = sz.xb2 - sz.a_10 - sz.a_ceil / 2 - 1;
		rect_br.top = rect_bu.bottom;
		rect_br.right = sz.xb2 - sz.a_10;
		rect_br.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_br, arr_brush[2]); //рисую правый черный прямоугольник

		RECT rect_bd;
		rect_bd.left = rect_bl.right;
		rect_bd.top = sz.yb2 - sz.a_10 - sz.a_ceil / 2 - 1;
		rect_bd.right = rect_br.left;
		rect_bd.bottom = sz.yb2 - sz.a_10;

		FillRect(hdc, &rect_bd, arr_brush[2]); //рисую нижний черный прямоугольник
	}

/*-------------------------------------------------------------
				  Нижнее окно информации - рамка
--------------------------------------------------------------*/
	RECT rect_u2;
	rect_u2.left = sz.xb1;
	rect_u2.top = sz.yb3;
	rect_u2.right = sz.xb2;
	rect_u2.bottom = sz.yb3 + sz.a_10;

	FillRect(hdc, &rect_u2, arr_brush[1]); //рисую верхний прямоугольник

	RECT rect_l2;
	rect_l2.left = sz.xb1;
	rect_l2.top = sz.yb3 + sz.a_10;
	rect_l2.right = sz.xb1 + sz.a_10;
	rect_l2.bottom = sz.yb4;

	FillRect(hdc, &rect_l2, arr_brush[1]); //рисую левый прямоугольник
	
	RECT rect_d2;
	rect_d2.left = sz.xb1 + sz.a_10;
	rect_d2.top = sz.yb4 - sz.a_10;
	rect_d2.right = sz.xb2;
	rect_d2.bottom = sz.yb4;

	FillRect(hdc, &rect_d2, arr_brush[1]); //рисую нижний прямоугольник

	RECT rect_r2;
	rect_r2.left = sz.xb2 - sz.a_10;
	rect_r2.top = sz.yb3 + sz.a_10;
	rect_r2.right = sz.xb2;
	rect_r2.bottom = sz.yb4 - sz.a_10;

	FillRect(hdc, &rect_r2, arr_brush[1]); //рисую правый прямоугольник

/*-------------------------------------------------------------
				  Остаточный прямоугольник
--------------------------------------------------------------*/
	RECT surplus;
	surplus.left = sz.xb1;
	surplus.top = sz.yb5;
	surplus.right = sz.xb2;
	surplus.bottom = sz.yb6;

	FillRect(hdc, &surplus, arr_brush[1]); //рисую большой прямоугольник


/*-------------------------------------------------------------
			Верхнее окно - следующая фигура - сетка
--------------------------------------------------------------*/
	HPEN hOldPen = (HPEN)SelectObject(hdc, arr_pen[1]); //выбираю тонкий карандаш

	
	//рисую вертикальные линии
	for (int i = 0; i < sz.n_side; ++i) {
		MoveToEx(hdc, sz.a_ceil * (i + 1) + i + sz.xb1 + sz.a_10 - sz.a_ceil / 2, sz.yb1 + sz.a_10, NULL); //перемещаю положение начальной точки
		LineTo(hdc, sz.a_ceil * (i + 1) + i + sz.xb1 + sz.a_10 - sz.a_ceil / 2, sz.yb2 - sz.a_10); //указываю конец линии
	}


	//рисую горизонтальные линии
	for (int i = 0; i < sz.n_side; ++i) {
		MoveToEx(hdc, sz.xb1 + sz.a_10, sz.a_ceil * (i + 1) + i + sz.yb1 + sz.a_10 - sz.a_ceil / 2, NULL); //перемещаю положение начальной точки
		LineTo(hdc, sz.xb2 - sz.a_10, sz.a_ceil* (i + 1) + i + sz.yb1 + sz.a_10 - sz.a_ceil / 2); //указываю конец линии
	}

	SelectObject(hdc, hOldPen); //вернул стоковое перо

/*-------------------------------------------------------------
					  Текст "Esc - пауза"
--------------------------------------------------------------*/
	text_Esc(hdc);
}


/*-------------------------------------------------------------
				  Рисую надпись "Esc - пауза"
--------------------------------------------------------------*/
void Objects::text_Esc(HDC hdc) {
	RECT rect;

	rect.left = sz.xb1;
	rect.top = sz.yb6;
	rect.right = sz.xb2;
	rect.bottom = sz.y5;

	FillRect(hdc, &rect, arr_brush[1]); //рисую фоновый прямоугольник

	HFONT old_hFont = (HFONT)SelectObject(hdc, arr_font[1]); //устанавливаю шрифт в контекст устройства
	SetTextColor(hdc, arr_color_brush[8]); //устанавливаю цвет текста
	SetBkColor(hdc, arr_color_brush[1]); //устанавливаю цвет фона под текстом

	DrawText(hdc, L"Esc - пауза", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //печатаю сообщение
	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 

}


/*-------------------------------------------------------------
					Обработка нажатия клавиш
--------------------------------------------------------------*/
void Objects::get_key_state() {
	left();
	right();
}

/*-------------------------------------------------------------
						Cтрелка влево
--------------------------------------------------------------*/
void Objects::left() {
	static short old_state{ 0 };
	static Timer t1; //таймер включения автогенерации
	static Timer t2; //таймер периода автогенерации
	const int n_t1{ 180 }; //время включения автогенерации
	const int n_t2{ 40 }; //скорость повтора

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
						Cтрелка вправо
--------------------------------------------------------------*/
void Objects::right() {
	static short old_state{ 0 };
	static Timer t1; //таймер включения автогенерации
	static Timer t2; //таймер периода автогенерации
	const int n_t1{ 180 }; //время включения автогенерации
	const int n_t2{ 40 }; //скорость повтора

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
						Cтрелка вверх
--------------------------------------------------------------*/
int Objects::up() {

if (((lParam >> 30) & 1) == 0) //если перед нажатием клавиша была не нажата!
fig.rotation();			   //против автогенерации при удержании клавиши
return 0;
}


/*-------------------------------------------------------------
						Cтрелка вниз
--------------------------------------------------------------*/
int Objects::down() {

	if (((lParam >> 30) & 1) == 0) //если перед нажатием клавиша была не нажата!
		fig.fast_down();			   //против автогенерации при удержании клавиши
	return 0;
}




/*-------------------------------------------------------------
				  Падение фигуры по таймеру
--------------------------------------------------------------*/
void Objects::down_figure() {
	fig.down();
}


/*-------------------------------------------------------------
				  Обновление состояния фигуры
--------------------------------------------------------------*/
void Objects::update_figure() {
	fig.update_figure();
}

/*-------------------------------------------------------------
						Рисование общее
--------------------------------------------------------------*/
void Objects::paint(HDC hdc) {

	field_paint(hdc); //рисую поле
	field_fill_rect_ceil(hdc); //раскрашиваю клетки
	field_fill_side(hdc); //рисую боковую панель
	fig.paint(hdc); //рисую фигуру
	fig.paint_next(hdc); //рисую следующую фигуру
	game.paint(hdc); //отрисовка состояния игры
}

/*-------------------------------------------------------------
				  Палитра цветов - кисти и перья
---------------------------------------------------------------
					     Создание кистей
--------------------------------------------------------------*/
void Objects::brush_generation() {
	for (int i = 0; i < n_color_brush; ++i) {
		arr_brush[i] = CreateSolidBrush(arr_color_brush[i]);
	}
}

/*-------------------------------------------------------------
						Создание перьев
--------------------------------------------------------------*/
void Objects::pen_generation() {
	for (int i = 0; i < n_color_pen; ++i) {
		arr_pen[i] = CreatePen(PS_SOLID, arr_color_pen[i].first, arr_color_pen[i].second);
	}
}

/*-------------------------------------------------------------
						Создание шрифтов
--------------------------------------------------------------*/
void Objects::font_generation() {

	for (int i = 0; i < n_font; ++i) {
		arr_font[i] = //создаю новый шрифт
			CreateFont(size_font[i], 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
	}
}


/*-------------------------------------------------------------
						Состояние игры
---------------------------------------------------------------
					  Начать новую игру
--------------------------------------------------------------*/
void Objects::new_game() {
	game.new_game();
}


/*-------------------------------------------------------------
					Обработка состояния игры
--------------------------------------------------------------*/
void Objects::game_state() {
	game.game_state();
}

/*-------------------------------------------------------------
							Пауза
--------------------------------------------------------------*/
int Objects::pause() {

		if (((lParam >> 30) & 1) == 0) //если перед нажатием клавиша была не нажата!
			game.pause();			   //против автогенерации при удержании клавиши
		return 0;
}