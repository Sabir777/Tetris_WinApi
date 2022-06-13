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

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //статическая функция
	LRESULT CALLBACK application_proc(); //объектная функция CALLBACK

	void create_window(); //создаю окно, отлавливаю исключения
	HWND create_hwnd_window(); //создаю окно - возвращаю HWND
	void show_window(); //показать окно

	//обработка сообщений Windows
	int wm_vscroll();
	int wm_paint();
	
private:
	HWND m_hwnd{}; //дескриптор окна
	UINT m_uMsg{}; //сообщение из оконной процедуры
	WPARAM m_wParam{}; //wParam из оконной процедуры
	LPARAM m_lParam{}; //lParam из оконной процедуры

	WSTR name_class_window{ L"Table_records" }; //задаю имя класса окна
	WSTR name_header_window{ L"Таблица рекордов" }; //задаю заголовок окна
	int width_window{1000}; //ширина окна
	int height_window{350}; //высота окна

	//данные для полосы прокрутки
	int nPage;		  //количество строк в странице
	int nLines;		  //количество записей в таблице рекордов
	int yChar{ 50 };        // высота строки
	
	int n_font{ yChar }; //размер шрифта
	HFONT font;			 //созданный шрифт
	ARR_COLOR_TABL color_tabl = {
		RGB(50, 205, 50),	  //цвет текста
		RGB(64, 125, 95),	  //цвет линий
		RGB(25, 25, 112)      //цвет фона
	};
	
	HBRUSH brush_backgroung;	  //кисть
	int d_pen{3};				  //толщина пера
	HPEN pen;					  //перо

	Scroll* p_rec; //указатель на базовый класс
};