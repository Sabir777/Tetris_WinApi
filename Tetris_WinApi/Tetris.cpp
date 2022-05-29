#include "Tetris.h"

/*-------------------------------------------------------------
						  Конструктор
--------------------------------------------------------------*/
Tetris::Tetris() {
	width_window = sz.x_n * sz.a_ceil + sz.x_n - 1 + 3 * sz.a_edging + sz.a_side; //вычисляю ширину окна
	height_window = sz.y_n * sz.a_ceil + sz.y_n - 1 + 2 * sz.a_edging; //вычисляю высоту окна
}


/*-------------------------------------------------------------
				 Статическая функция-процедура
--------------------------------------------------------------*/
LRESULT Tetris::window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Tetris* p_tetris;
	if (uMsg == WM_NCCREATE) {
		p_tetris = static_cast<Tetris*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_tetris))) {
			if (GetLastError() != 0)
				return false;
		}
	}
	else {
		//достаю указатель на объект "Window_obj_init" помещенный туда при конструировании объекта
		p_tetris = reinterpret_cast<Tetris*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (p_tetris) { 
		
		//записываю данные из процедуры
		p_tetris->m_hwnd = hwnd;
		p_tetris->m_uMsg = uMsg;
		p_tetris->m_wParam = wParam;
		p_tetris->m_lParam = lParam;

		//вызываю объектную процедуру
		return p_tetris->application_proc();
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


/*-------------------------------------------------------------
			  Запуск игры - отлавливание исключений
--------------------------------------------------------------*/
int Tetris::run() {
	try {
		return play();
	}
	catch (const std::exception& e) {
		std::string expt_data = e.what();
		MessageBox(nullptr, std::wstring(begin(expt_data),
			end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

/*-------------------------------------------------------------
					   Игра - вечный цикл
--------------------------------------------------------------*/
int Tetris::play() {

	Func f = Tetris::window_proc;
	m_hwnd = window_create<Tetris>(f, name_class_window, name_header_window,
		width_window, height_window, this); //создание окна

	if (!SetTimer(m_hwnd, ID_TIMER, 20, NULL)) //регистрация таймера - 20 мс
		throw std::runtime_error("Error, can't register timer!");

	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //первый показ окна
	UpdateWindow(m_hwnd); //обновление окна

	MSG msg{};

	//вечный цикл
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

/*-------------------------------------------------------------
					   Методы приложения
---------------------------------------------------------------
			 Объектная функция-процедура (приложения)
--------------------------------------------------------------*/
LRESULT Tetris::application_proc() {

	switch (m_uMsg)
	{
	case WM_COMMAND:
		return wm_command();

	case WM_TIMER:
		return wm_timer();

	case WM_PAINT:
		return wm_paint();

	case WM_KEYDOWN:
		return wm_keydown();

	case WM_DESTROY:
		KillTimer(m_hwnd, ID_TIMER); //уничтожение таймера при выходе из программы
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(m_hwnd, m_uMsg, m_wParam, m_lParam);
}

/*-------------------------------------------------------------
				   Методы обработки сообщений
---------------------------------------------------------------
							WM_PAINT
--------------------------------------------------------------*/
int Tetris::wm_paint() {

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps); //получаю контекст устройства

	HDC memBit = CreateCompatibleDC(hdc); //получаю совместимый контекст устройства в памяти
	HANDLE hBitmap = CreateCompatibleBitmap(hdc, sz.a_right, sz.a_down); //размеры битмапа = размерам окна целиком!!!
	HANDLE hOldBitmap = SelectObject(memBit, hBitmap); //выбираю битмап в совместимый контекст

	
	obj.paint(memBit); //рисую в совместимый контекст
	
	BitBlt(hdc, 0, 0, sz.a_right, sz.a_down, memBit, 0, 0, SRCCOPY); //копирую битмап в текущий контекст

	DeleteObject(SelectObject(memBit, hOldBitmap)); //удаляю битмап
	DeleteDC(memBit); //удаляю совместимый контекст

	EndPaint(m_hwnd, &ps); //освобождаю контекст устройства
	return 0;
}

/*-------------------------------------------------------------
						   WM_TIMER
--------------------------------------------------------------*/
int Tetris::wm_timer() {

	obj.get_key_state();
	obj.down_figure();
	obj.update_figure();
	obj.game_state();

	return 0;
}


/*-------------------------------------------------------------
						   WM_COMMAND
--------------------------------------------------------------*/
int Tetris::wm_command() {
	switch (LOWORD(m_wParam)) {
	case static_cast<int> (Button::NEW_GAME):
		obj.new_game();
	}
	return 0;
}


/*-------------------------------------------------------------
 обработка нажатий клавиш "вверх" и "Esc" (против автогенерации)
--------------------------------------------------------------*/
int Tetris::wm_keydown() {

	switch (m_wParam) {
	
	case VK_UP: //стрелка вверх
		return obj.up();

	case VK_DOWN: //стрелка вниз
		return obj.down();

	case VK_ESCAPE:
		return obj.pause();

	default:
		return DefWindowProc(m_hwnd, m_uMsg, m_wParam, m_lParam);
	}
}