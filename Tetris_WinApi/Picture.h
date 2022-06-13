#pragma once
#include <Windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "resource.h"


using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);
using WSTR = std::wstring;
using STR = std::string;


class Picture {
public:
	Picture();
	~Picture();

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //статическая функция
	LRESULT CALLBACK application_proc(); //объектная функция CALLBACK

	void create_window(); //создаю окно, отлавливаю исключения
	HWND create_hwnd_window(); //создаю окно - возвращаю HWND
	void create_bitmap(); //создаю битмап
	void show_window(); //показать окно
	void close(); //скрыть окно
	int wm_paint(); //рисовать в окне
	
private:
	HWND m_hwnd{}; //дескриптор окна
	UINT m_uMsg{}; //сообщение из оконной процедуры
	WPARAM m_wParam{}; //wParam из оконной процедуры
	LPARAM m_lParam{}; //lParam из оконной процедуры

	WSTR name_class_window{ L"Picture" }; //задаю имя класса окна
	WSTR name_header_window{ L"Новый рекорд!!!" }; //задаю заголовок окна
	int width_window; //ширина окна
	int height_window; //высота окна

	//картина
	HDC memBit; //совместимый контекст в памяти
	HANDLE hBitmap; //загруженный bitmap
	HANDLE hOldBitmap; //стоковый bitmap
	BITMAP Bitmap; //размеры битмапа
};