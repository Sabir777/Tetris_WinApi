#include "Scroll_records.h"


Scroll_records::Scroll_records(Scroll* p_rec) 
	: p_rec{p_rec} 
{
	create_window();
	font = CreateFont(n_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
	brush_backgroung = CreateSolidBrush(color_tabl[2]);
	pen = CreatePen(PS_SOLID, d_pen, color_tabl[1]);
}

Scroll_records::~Scroll_records() {
	DeleteObject(font);
	DeleteObject(brush_backgroung);
	DeleteObject(pen);
}


/*-------------------------------------------------------------
				 Статическая callback-функция
--------------------------------------------------------------*/
LRESULT Scroll_records::window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Scroll_records* p_scroll;
	if (uMsg == WM_NCCREATE) {
		p_scroll = static_cast<Scroll_records*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_scroll))) {
			if (GetLastError() != 0)
				return false;
		}
	}
	else {
		//достаю указатель на объект "Window_obj_init" помещенный туда при конструировании объекта
		p_scroll = reinterpret_cast<Scroll_records*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (p_scroll) {

		//записываю данные из процедуры
		p_scroll->m_hwnd = hwnd;
		p_scroll->m_uMsg = uMsg;
		p_scroll->m_wParam = wParam;
		p_scroll->m_lParam = lParam;

		//вызываю объектную процедуру
		return p_scroll->application_proc();
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


/*-------------------------------------------------------------
			  Функция обработки сообщений Windows
--------------------------------------------------------------*/
LRESULT Scroll_records::application_proc() {

	switch (m_uMsg)
	{
	case WM_VSCROLL:
		return wm_vscroll();

	case WM_PAINT:
		return wm_paint();
		
	case WM_CLOSE:
		ShowWindow(m_hwnd, SW_HIDE); //скрыть окно таблицы рекордов
		return 0;

	default: return DefWindowProc(m_hwnd, m_uMsg, m_wParam, m_lParam);
	}	
}

/*-------------------------------------------------------------
			   Создание окна - таблица рекордов
--------------------------------------------------------------*/
void Scroll_records::create_window() {

	try {
		m_hwnd = create_hwnd_window();
	}
	catch (const std::exception& e) {
		std::string expt_data = e.what();
		MessageBox(nullptr, std::wstring(begin(expt_data),
			end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE); //закрыть приложение, если ошибка
	}
}



/*-------------------------------------------------------------
			Создаю окно - возвращаю его дескриптор
--------------------------------------------------------------*/
HWND Scroll_records::create_hwnd_window() {

	HINSTANCE hinst = GetModuleHandle(nullptr);

	//создание класса окна и его заполнение
		WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON3));
		wc.hIconSm = LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON3));
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = window_proc;
		wc.lpszClassName = name_class_window.c_str();
		wc.lpszMenuName = nullptr;
		wc.style = CS_VREDRAW | CS_HREDRAW;

		WSTR message{ L"Error, can't register class: " };
		message += name_class_window;
		STR mess(std::begin(message), end(message));

		if (!RegisterClassEx(&wc)) //регистрация класса окна
			throw std::runtime_error(mess.c_str());

		HWND m_hwnd = CreateWindowEx(0, name_class_window.c_str(),
			name_header_window.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VSCROLL,
			(GetSystemMetrics(SM_CXSCREEN) - width_window) / 2, //расположение окна в центре экрана по горизонтали
			(GetSystemMetrics(SM_CYSCREEN) - height_window) / 2, //расположение окна в центре экрана по вертикали
			width_window, height_window, nullptr, nullptr, nullptr, this);

		message = L"Error, can't create window: ";
		message += name_header_window;
		mess = STR(std::begin(message), end(message));

		if (!m_hwnd)
			throw std::runtime_error(mess.c_str());
		
		return m_hwnd;
}


/*-------------------------------------------------------------
						Показать окно
--------------------------------------------------------------*/
void Scroll_records::show_window() {

	nLines = p_rec->get_table().size(); //обновляю данные о количестве рекордов перед показом
	//заполняю структуру
	SCROLLINFO si{ sizeof(SCROLLINFO) };
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = nLines - 1;

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	si.nPage = nPage = rect.bottom / yChar;

	SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE); //вношу данные о полосе прокрутки

	//показываю окно
	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //показ окна
	UpdateWindow(m_hwnd); //обновление окна
}

/*-------------------------------------------------------------
						   WM_VSCROLL
--------------------------------------------------------------*/
int Scroll_records::wm_vscroll() {
	SCROLLINFO si{ sizeof(SCROLLINFO) };
	si.fMask = SIF_ALL;
	GetScrollInfo(m_hwnd, SB_VERT, &si);
	int nPos = si.nPos;

	switch (LOWORD(m_wParam))
	{
		// кликаю по верхнему флажку полосы
	case SB_LINEUP:
		si.nPos -= 1;
		break;

		// кликаю по нижнему флажку полосы
	case SB_LINEDOWN:
		si.nPos += 1;
		break;

		// перелистываю страницу вверх
	case SB_PAGEUP:
		si.nPos -= si.nPage;
		break;

		// перелистываю страницу вниз
	case SB_PAGEDOWN:
		si.nPos += si.nPage;
		break;

		// тащу ползунок
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;

	default:
		break;
	}


	// ввожу данные в SetScrollInfo
	si.fMask = SIF_POS;
	SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
	// получаю скорректированные данные для ввода в ScrollWindow
	GetScrollInfo(m_hwnd, SB_VERT, &si);

	if (si.nPos != nPos)
	{
		ScrollWindow(m_hwnd, 0, yChar * (nPos - si.nPos), NULL, NULL);
		UpdateWindow(m_hwnd);
	}

	return 0;
}


/*-------------------------------------------------------------
							WM_PAINT
--------------------------------------------------------------*/
int Scroll_records::wm_paint() {

	InvalidateRect(m_hwnd, NULL, TRUE); //чтобы нормально прорисовывались горизонтальные линии вверху

	PAINTSTRUCT ps;
	SCROLLINFO si{ sizeof(SCROLLINFO) };

	HDC hdc = BeginPaint(m_hwnd, &ps);

/*--------------------------------------------------------------*/
							 /*Фон*/
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	FillRect(hdc, &rect, brush_backgroung);//заливаю фон
	

/*--------------------------------------------------------------*/
						   /*Текст*/

	HFONT old_hFont = (HFONT)SelectObject(hdc, font); //устанавливаю шрифт в контекст устройства

	SetTextColor(hdc, color_tabl[0]); //устанавливаю цвет текста
	SetBkColor(hdc, color_tabl[2]); //устанавливаю цвет фона под текстом

	si.fMask = SIF_POS;
	GetScrollInfo(m_hwnd, SB_VERT, &si);
	int nPos = si.nPos;

	//рисую дробную часть нижней строки
	int EndLine = min(nLines - 1, nPos + nPage);

	auto tabl = p_rec->get_table();
	auto it = tabl.end(); --it;
	auto begin = tabl.begin(); --begin;
	std::vector<std::pair<WSTR, int>> vtabl(tabl.size());
	for (size_t i = 0; it != begin; ++i, --it) {
		vtabl[i] = std::pair<WSTR, int>{ (*it).second, (*it).first };
	}

	for (int i = nPos; i <= EndLine; ++i)
	{
		int x = 10;
		int y = yChar * (i - nPos);
		WSTR str{ vtabl[i].first }, name, date, clock;
		std::wistringstream iss{ str };
		iss >> name >> date >> clock;
		date += L" " + clock;
	
		for (int i = 0, size = name.size(); i < size; ++i) { //заменить нижнее подчеркивание пробелами
			if (name[i] == L'_') {
				name[i] = L' ';
			}
		}
		
		auto size = name.size();
		if (size > 15) {
			name = name.substr(0, 15);
			size = 15;
		}
		TextOut(hdc, x, y, name.c_str(), size); //имя

		SetTextAlign(hdc, TA_RIGHT | TA_TOP); //счет
		x = 580;
		WSTR word = std::to_wstring(vtabl[i].second);
		size = word.size();
		TextOut(hdc, x, y, word.c_str(), size);
		
		SetTextAlign(hdc, TA_LEFT | TA_TOP); //дата и время
		x = 600;
		size = date.size();
		TextOut(hdc, x, y, date.c_str(), size);
	}

	SelectObject(hdc, old_hFont); //возвращаю в контекст стоковый шрифт 


/*--------------------------------------------------------------*/
							/*Линии*/

	HPEN hOldPen = (HPEN)SelectObject(hdc, pen);

	for (int i = 1; i <= nLines; ++i) {
		MoveToEx(hdc, 0, i * yChar, NULL);
		LineTo(hdc, rect.right, i * yChar);
	}

	MoveToEx(hdc, 590, 0, NULL);
	LineTo(hdc, 590, nLines * yChar);

	SelectObject(hdc, hOldPen); //вернул стоковое перо
	EndPaint(m_hwnd, &ps);
	return 0;
}