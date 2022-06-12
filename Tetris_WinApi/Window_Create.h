#pragma once
#include <Windows.h>
#include <string>
#include <stdexcept>
#include "resource.h"

/*-------------------------------------------------------------
			 Глобальная функция - создание окна
--------------------------------------------------------------*/
using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);

using WSTR = std::wstring;
using STR = std::string;

template<typename T>
HWND window_create(Func f, WSTR name_class, WSTR header_win, int width, int height, T* p_obj) {

	HINSTANCE hinst = GetModuleHandle(nullptr);

	//создание класса окна и его заполнение
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON2));
	wc.hInstance = hinst;
	wc.lpfnWndProc = f;
	wc.lpszClassName = name_class.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	WSTR message{ L"Error, can't register class: " };
	message += name_class;
	STR mess(std::begin(message), end(message));

	if (!RegisterClassEx(&wc)) //регистрация класса окна
		throw std::runtime_error(mess.c_str());

	HWND m_hwnd = CreateWindowEx(0, name_class.c_str(),
		header_win.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2, //расположение окна в центре экрана по горизонтали
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2, //расположение окна в центре экрана по вертикали
		width, height, nullptr, nullptr, nullptr, p_obj);

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	width += width - (rect.right - rect.left);
	height += height - (rect.bottom - rect.top);

	SetWindowPos(m_hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

	message = L"Error, can't create window: ";
	message += header_win;
	mess = STR(std::begin(message), end(message));

	if (!m_hwnd)
		throw std::runtime_error(mess.c_str());

	return m_hwnd;
}
