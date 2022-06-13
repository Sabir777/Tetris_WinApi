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


enum class Controls : int { BUTTON = 1001 , EDIT = 2000 };//контролы


using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);
using WSTR = std::wstring;
using STR = std::string;

class Input : public Scroll {

	using Arr_Color = std::array<COLORREF, 2>;

public:
	Input() = delete;
	Input(Scroll*);
	~Input();

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //статическая функция
	LRESULT CALLBACK application_proc(); //объектная функция CALLBACK

	void create_window(); //создаю окно, отлавливаю исключения
	HWND create_hwnd_window(); //создаю окно - возвращаю HWND
	void create_controls(); //создаю контролы
	void show_window(); //показать окно
	WSTR read(); //прочитать текст из окна ввода

	//обработка событий
	int wm_command(); //нажатие кнопки
	int wm_close(WSTR name = L"Неизвестный"); //закрытие окошка
	int wm_paint(); //рисовать окошко
	int wm_setcursor(); //очистить поле ввода от первоначальной надписи

private:
	HWND m_hwnd{}; //дескриптор окна
	UINT m_uMsg{}; //сообщение из оконной процедуры
	WPARAM m_wParam{}; //wParam из оконной процедуры
	LPARAM m_lParam{}; //lParam из оконной процедуры

	WSTR name_class_window{ L"Input" }; //задаю имя класса окна
	WSTR name_header_window{ L"Новый рекорд!!!" }; //задаю заголовок окна
	int width_window{ 520 }; //ширина окна
	int height_window{ 200 }; //высота окна

	HWND hwnd_button{}; //дескриптор кнопки
	int x_pos_btn{ 155 }; //положение x - кнопки
	int y_pos_btn{ 100 }; //положение y - кнопки 
	int width_button{ 200 }; //ширина кнопки
	int height_button{ 30 }; //высота кнопки
	
	HWND hwnd_edit{}; //дескриптор поля ввода
	int x_pos_edit{ 100 }; //положение x - поля ввода
	int y_pos_edit{ 50 }; //положение y - поля ввода
	int width_edit{ 300 }; //ширина поля для ввода
	int height_edit{ 25 }; //высота поля для ввода
	WSTR messange = L"         Введите свое имя здесь...";
	bool flag_focus{ false };

	Arr_Color arr_color = {
		RGB(255, 215, 0), //фон
		RGB(255, 0, 0) //текст
	};
	HBRUSH brush{}; //кисть

	int n_font{ 20 }; //размер шрифта
	HFONT font{};	//шрифт

	Scroll* p_rec; //указатель на базовый класс
};