#include "Game.h"

/*-------------------------------------------------------------
				   Конструктор с параметрами
--------------------------------------------------------------*/
Game::Game(Parrent* pf, ARRAY_10_20& field, AR_BRUSH& brush, 
			ARR_COLOR& color, AR_FONT& font, Size_Window& sz)
	: Parrent(), p_fig{ pf }, field{ field }, brush{ brush },
	color{ color }, font{ font }, sz{ sz }
{ 
	create_bitmap(); //создаю битмапы
}

/*-------------------------------------------------------------
						  Деструктор
--------------------------------------------------------------*/
Game::~Game() {

	//котик
	DeleteObject(SelectObject(memBit, hOldBitmap)); //возвращаю стоковый и удаляю созданный битмап
	DeleteDC(memBit); //удаляю совместимый контекст

	//череп
	DeleteObject(SelectObject(memBit2, hOldBitmap2)); //возвращаю стоковый и удаляю созданный битмап
	DeleteDC(memBit2); //удаляю совместимый контекст
}


/*-------------------------------------------------------------
		Создание контекста в памяти и загрузка битмапа
--------------------------------------------------------------*/
void Game::create_bitmap() {

	HWND hwnd = FindWindow(L"My_Window", nullptr); //найти главное окно

	//котик
	hBitmap = LoadImage(NULL, L"cat.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //загружаю битмап
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap); //Получаем размерность загруженного bitmap'a

	//череп
	hBitmap2 = LoadImage(NULL, L"skull.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //загружаю битмап
	GetObject(hBitmap2, sizeof(BITMAP), &Bitmap2); //Получаем размерность загруженного bitmap'a

	HDC hdc = GetDC(hwnd); //получаю контекст устройства

	//котик
	memBit = CreateCompatibleDC(hdc); //получаю совместимый контекст устройства в памяти
	hOldBitmap = SelectObject(memBit, hBitmap); //выбираю битмап в совместимый контекст

	//череп
	memBit2 = CreateCompatibleDC(hdc); //получаю совместимый контекст устройства в памяти
	hOldBitmap2 = SelectObject(memBit2, hBitmap2); //выбираю битмап в совместимый контекст

	ReleaseDC(hwnd, hdc); //завершение обработки
}


/*-------------------------------------------------------------
					Установка/снятие паузы
--------------------------------------------------------------*/
void Game::pause() {
	p_fig->pause_figure();
	update_window(); //обновить окно
}

/*-------------------------------------------------------------
					  Начать новую игру
--------------------------------------------------------------*/
void Game::new_game() {
	field = ARRAY_10_20{ {} }; //очистить поле
	score = 0; //обнуляю текущий счет
	count_row = 0; //обнуляю количество сгоревших рядов
	level = 0; //обнуляю уровень сложности
	p_fig->init_new_figure(); //создать новую фигуру
	DestroyWindow(hwnd_btn); //удалить кнопку
	update_window(); //обновить окно
}

/*-------------------------------------------------------------
					Обработка состояния игры
--------------------------------------------------------------*/
void Game::game_state() {

/*-------------------------------------------------------------
				  Создание кнопки новая игра
--------------------------------------------------------------*/
	create_button();

/*-------------------------------------------------------------
	 Выключение мигания так как фигура упала и ряды стерты
--------------------------------------------------------------*/
	flash_off();

/*-------------------------------------------------------------
					  Генерация мигания
--------------------------------------------------------------*/
	flash_generation();

/*-------------------------------------------------------------
			Включение анимации о начисляемых очках
--------------------------------------------------------------*/
	score_animation_on();

/*-------------------------------------------------------------
			 Отключение анимации о начисляемых очках
--------------------------------------------------------------*/
	score_animation_off();

/*-------------------------------------------------------------
					Подсчет заработанных очков
--------------------------------------------------------------*/
	set_score();

/*-------------------------------------------------------------
					Подсчет уровня сложности
--------------------------------------------------------------*/
	set_level();

/*-------------------------------------------------------------
						Включение музыки
--------------------------------------------------------------*/
	play_sound();

}


/*-------------------------------------------------------------
						Создать кнопку
--------------------------------------------------------------*/
void Game::create_button() {

	static Game_State old_gs{ Game_State::PLAY };
	Game_State gs = p_fig->get_game_state(); //получение состояния игры

	if (gs == Game_State::GAME_OVER && gs != old_gs) {

		HWND hwnd = FindWindow(L"My_Window", nullptr); //найти главное окно

		try {
			// создаю кнопку Новая игра
			hwnd_btn = CreateWindowEx(0, L"BUTTON", L"Новая игра",
				WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, sz.x_bt, sz.y_bt, sz.w_bt, sz.h_bt,
				hwnd, reinterpret_cast<HMENU>(Button::NEW_GAME),
				nullptr, nullptr);

			if (!hwnd_btn)
				throw std::runtime_error("Error, can't create button - New Game!");

			ShowWindow(hwnd_btn, SW_SHOWDEFAULT); //показ кнопки
		}
		catch (const std::exception& e) {
			std::string expt_data = e.what();
			MessageBox(nullptr, std::wstring(begin(expt_data),
				end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
			ExitProcess(EXIT_FAILURE); //завершение работы
		}
	}

	old_gs = gs;
}


/*-------------------------------------------------------------
					   Удалить полные ряды
--------------------------------------------------------------*/
void Game::destroy_row(set<int> Y_cont) {

	ARRAY_10_20 temp_field{ {} };
	for (int j = sz.y_n - 1, jx = j; j >= 0; --j) {

		if (Y_cont.find(j) == Y_cont.end()) { //если строку удалять не нужно (ее нет в списке)
			temp_field[jx] = field[j]; //копирую ее
			--jx;
		}
	}
	field = temp_field;	
}


/*-------------------------------------------------------------
	  Установка флага мигания и получение мигающих рядов
---------------------------------------------------------------
				  Включения звука анимации
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
					  Выключение мигания
--------------------------------------------------------------*/
void Game::flash_off() {
	Figure_State fs = p_fig->get_figure_state(); //получение состояния фигуры

	if (flag_flash && fs == Figure_State::DESTROY) {
		flag_flash = false; //отключаю флаг анимации
		flash = Flash::OFF; //убираю мерцание

		update_window();
	}
}


/*-------------------------------------------------------------
					  Генерация мигания
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
	  Включаю режим вывода сообщения о начисляемых очках
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
	Отключаю режим вывода сообщения о начисляемых очках
--------------------------------------------------------------*/
void Game::score_animation_off() {
	if (flag_score_animation && t_score.duration() >= n_t_score) {
		flag_score_animation = false;
		update_window();
	}
}


/*-------------------------------------------------------------
		 Анимация - вывод сообщения о начисленных очках
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

		FillRect(hdc, &rect, brush[2]); //рисую темный прямоугольник

		HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //устанавливаю шрифт в контекст устройства
		SetTextColor(hdc, color[4]); //устанавливаю желтый цвет текста
		SetBkColor(hdc, color[2]); //устанавливаю цвет фона под текстом

		DrawText(hdc, messange().c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //печатаю сообщение
		SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 
	}
}


/*-------------------------------------------------------------
					Начисление очков игры
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
					Вычисление уровня игры
--------------------------------------------------------------*/
void Game::set_level() {

	if (count_row >= 8) {
		count_row = 0;
		++level;

		auto set_time = [&]() {
			switch (level) {
			case 1:
				return 720; //720 милисекунд
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
					Отрисовка текущего счета
--------------------------------------------------------------*/
void Game::paint_score(HDC hdc) {

	RECT rect{ sz.xb1, sz.yb2, sz.xb2, sz.yb3 };
	
	FillRect(hdc, &rect, brush[1]); //рисую фон - цвет панели

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[1]); //устанавливаю шрифт в контекст устройства
	SetTextColor(hdc, color[4]); //устанавливаю желтый цвет текста
	SetBkColor(hdc, color[1]); //устанавливаю цвет фона под текстом

	WSTR str = L"Счет: " + std::to_wstring(score);

	DrawText(hdc, str.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //печатаю сообщение
	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 
}


/*-------------------------------------------------------------
				   Отрисовка уровня сложности
--------------------------------------------------------------*/
void Game::paint_level(HDC hdc) {

	RECT rect{ sz.xb1, sz.yb4, sz.xb2, sz.yb5 };

	FillRect(hdc, &rect, brush[1]); //рисую фон - цвет панели

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[1]); //устанавливаю шрифт в контекст устройства
	SetTextColor(hdc, color[5]); //устанавливаю красный цвет текста
	SetBkColor(hdc, color[1]); //устанавливаю цвет фона под текстом

	WSTR str = L"Уровень: " + std::to_wstring(level);

	DrawText(hdc, str.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //печатаю сообщение
	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 
}



/*-------------------------------------------------------------
					 Отрисовка состояния
--------------------------------------------------------------*/
void Game::paint(HDC hdc) {
	Game_State gs = p_fig->get_game_state();

	paint_score(hdc); //счет
	paint_level(hdc); //уровень

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
						Отрисовка паузы
--------------------------------------------------------------*/
void Game::paint_pause(HDC hdc) {
	RECT rect;
	
	rect.left = sz.xb1 + sz.a_10;
	rect.top = sz.yb3 + sz.a_10;
	rect.right = sz.xb2 - sz.a_10;
	rect.bottom = sz.yb4 - sz.a_10;
	
	FillRect(hdc, &rect, brush[2]); //рисую темный прямоугольник

	HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //устанавливаю шрифт в контекст устройства
	SetTextColor(hdc, RGB(65, 224, 40)); //устанавливаю зеленый цвет текста
	SetBkColor(hdc, color[2]); //устанавливаю цвет фона под текстом

	DrawText(hdc, L"Пауза", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //печатаю сообщение
	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 

}

/*-------------------------------------------------------------
				 Прорисовка состояния - Play
--------------------------------------------------------------*/
void Game::paint_play(HDC hdc) {
	
/*-------------------------------------------------------------
					Рисую картинку - котик
--------------------------------------------------------------*/
	if (!flag_score_animation) {
		StretchBlt(hdc, //дескриптор приемного DC
			sz.xb1 + sz.a_10, //x - левый верхний угол приемника
			sz.yb3 + sz.a_10, //у - левый верхний угол приемника
			sz.a_side - 2 * sz.a_10, // ширина приёмного прямоугольника
			sz.a_side - 2 * sz.a_10, // высота приёмного прямоугольника
			memBit, // дескриптор исходного DC
			0, 0, //координаты левого верхнего угла источника
			Bitmap.bmWidth, //ширина битмапа
			Bitmap.bmHeight, //высота битмапа
			SRCCOPY //код операции - копирование
		);
	}
}


/*-------------------------------------------------------------
				Отрисовываю состояние - Game over
--------------------------------------------------------------*/
void Game::paint_game_over(HDC hdc) {

/*-------------------------------------------------------------
				  Вывожу надпись - Game over
--------------------------------------------------------------*/
	RECT rect;

	rect.left = sz.xb1 + sz.a_10;
	rect.top = sz.yb1 + sz.a_10;
	rect.right = sz.xb2 - sz.a_10;
	rect.bottom = sz.yb2 - sz.a_10;

	FillRect(hdc, &rect, brush[2]); //рисую темный прямоугольник

	rect.top += 15;
	HFONT old_hFont = (HFONT)SelectObject(hdc, font[0]); //устанавливаю шрифт в контекст устройства
	SetTextColor(hdc, color[5]); //устанавливаю красный цвет текста
	SetBkColor(hdc, color[2]); //устанавливаю цвет фона под текстом

	DrawText(hdc, L"Game over", -1, &rect,
		DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_WORDBREAK | DT_CENTER);

	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 

/*-------------------------------------------------------------
					Рисую картинку - череп
--------------------------------------------------------------*/
	StretchBlt(hdc, //дескриптор приемного DC
		sz.xb1 + sz.a_10, //x - левый верхний угол приемника
		sz.yb3 + sz.a_10, //у - левый верхний угол приемника
		sz.a_side - 2 * sz.a_10, // ширина приёмного прямоугольника
		sz.a_side - 2 * sz.a_10, // высота приёмного прямоугольника
		memBit2, // дескриптор исходного DC
		0, 0, //координаты левого верхнего угла источника
		Bitmap2.bmWidth, //ширина битмапа
		Bitmap2.bmHeight, //высота битмапа
		SRCCOPY //код операции - копирование
	);
}

/*-------------------------------------------------------------
				 Перерисовать - обновить окно
--------------------------------------------------------------*/
void Game::update_window() {

	HWND hwnd = FindWindow(L"My_Window", nullptr); //найти главное окно

	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

/*-------------------------------------------------------------
							Музыка
--------------------------------------------------------------*/
void Game::play_sound() {
	static Game_State old_gs = Game_State::GAME_OVER;
	
	Game_State gs = p_fig->get_game_state();

	if (gs != old_gs) {
		if (gs == Game_State::PLAY) {
			
			if (old_gs == Game_State::GAME_OVER) {
				twinpix.stop(); //завершаю трэк - подготавливаюсь к последующему воспроизведению 
				music.play_repeat(); //включаю основной трек (c повтором) - коробейники
			}
			else if (old_gs == Game_State::PAUSE) {
				music.resume();
			}
		}
		else if (gs == Game_State::PAUSE) {

			music.pause();
			game_over.stop();
			game_over.play(); //включаю звук окончания игры
		}
		else if (gs == Game_State::GAME_OVER) {

			game_over.stop();
			game_over.play(); //включаю звук окончания игры
			music.stop(); //отключаю основной трек
			twinpix.play_repeat(); //включаю твин-пикс так как игра закончена
		}
	}
	old_gs = gs;
}