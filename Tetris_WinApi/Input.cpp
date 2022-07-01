#include "Input.h"

Input::Input(Scroll* p_rec) 
	: p_rec{ p_rec }
{
	create_window();
	create_controls();
	font = CreateFont(n_font, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
	brush = CreateSolidBrush(arr_color[0]);
}


Input::~Input() {
	DeleteObject(font);
	DeleteObject(brush);
}


/*-------------------------------------------------------------
				 ����������� callback-�������
--------------------------------------------------------------*/
LRESULT Input::window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Input* p_input;
	if (uMsg == WM_NCCREATE) {
		p_input = static_cast<Input*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_input))) {
			if (GetLastError() != 0)
				return false;
		}
	}
	else {
		//������ ��������� �� ������ "Window_obj_init" ���������� ���� ��� ��������������� �������
		p_input = reinterpret_cast<Input*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (p_input) {

		//��������� ������ �� ���������
		p_input->m_hwnd = hwnd;
		p_input->m_uMsg = uMsg;
		p_input->m_wParam = wParam;
		p_input->m_lParam = lParam;

		//������� ��������� ���������
		return p_input->application_proc();
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


/*-------------------------------------------------------------
			  ������� ��������� ��������� Windows
--------------------------------------------------------------*/
LRESULT Input::application_proc() {

	switch (m_uMsg)
	{
	case WM_SETCURSOR:
		return wm_setcursor();
		//SetWindowText(hwnd_edit, L""); //������� ���� ���� �����
		//return 0;

	case WM_COMMAND:
		return wm_command();

	case WM_PAINT:
		return wm_paint();

	case WM_CLOSE:
		return wm_close();

	default: return DefWindowProc(m_hwnd, m_uMsg, m_wParam, m_lParam);
	}
}


/*-------------------------------------------------------------
						�������� ����
--------------------------------------------------------------*/
void Input::create_window() {

	try {
		m_hwnd = create_hwnd_window();
	}
	catch (const std::exception& e) {
		std::string expt_data = e.what();
		MessageBox(nullptr, std::wstring(begin(expt_data),
			end(expt_data)).c_str(), L"������", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE); //������� ����������, ���� ������
	}
}


/*-------------------------------------------------------------
			������ ���� - ��������� ��� ����������
--------------------------------------------------------------*/
HWND Input::create_hwnd_window() {

	HINSTANCE hinst = GetModuleHandle(nullptr);

	//�������� ������ ���� � ��� ����������
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

	if (!RegisterClassEx(&wc)) //����������� ������ ����
		throw std::runtime_error(mess.c_str());

	HWND m_hwnd = CreateWindowEx(0, name_class_window.c_str(),
		name_header_window.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		(GetSystemMetrics(SM_CXSCREEN) - width_window) / 2, //������������ ���� � ������ ������ �� �����������
		(GetSystemMetrics(SM_CYSCREEN) - height_window) / 2, //������������ ���� � ������ ������ �� ���������
		width_window, height_window, nullptr, nullptr, nullptr, this);

	message = L"Error, can't create window: ";
	message += name_header_window;
	mess = STR(std::begin(message), end(message));

	if (!m_hwnd)
		throw std::runtime_error(mess.c_str());

	return m_hwnd;
}


/*-------------------------------------------------------------
						������ ��������
--------------------------------------------------------------*/
void Input::create_controls() {
	try {
		hwnd_button = CreateWindowEx(0, L"BUTTON", L"����������� ����",
			WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, x_pos_btn, y_pos_btn, 
			width_button, height_button, m_hwnd, reinterpret_cast<HMENU>(Controls::BUTTON),
			nullptr, nullptr);

		if (!hwnd_button) {
			throw std::runtime_error("Error, can't create button - ����������� ����!");
		}
		
		hwnd_edit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", messange.c_str(),
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, x_pos_edit, y_pos_edit, width_edit, height_edit, m_hwnd,
			reinterpret_cast<HMENU>(Controls::EDIT), nullptr, nullptr);

		if (!hwnd_edit) {
			throw std::runtime_error("Error, can't create edit - ���� �����!");
		}
		
	}
	catch (const std::exception& e) {
		std::string expt_data = e.what();
		MessageBox(nullptr, std::wstring(begin(expt_data),
			end(expt_data)).c_str(), L"������", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE); //������� ����������, ���� ������
	}
}


/*-------------------------------------------------------------
						�������� ����
--------------------------------------------------------------*/
void Input::show_window() {
	SendMessage(hwnd_edit, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);//������������ ����� � ���� ����� ������
	SetWindowText(hwnd_edit, messange.c_str()); //������� ����� � ���� �����-������
	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //����� ����
	ShowWindow(hwnd_button, SW_SHOWDEFAULT); //����� ������
	ShowWindow(hwnd_edit, SW_SHOWDEFAULT); //����� ������
	UpdateWindow(m_hwnd); //���������� ����
}

/*-------------------------------------------------------------
				��������� ����� �� ���� �����
--------------------------------------------------------------*/
WSTR Input::read() {

	wchar_t buff[50];
	GetWindowText(hwnd_edit, buff, 50);
	
	std::wistringstream ist{ buff };
	
	std::vector<WSTR> arr_str;
	
	WSTR str;
	while (ist >> str) {
		arr_str.push_back(str);
	}

	if (arr_str.size() == 0) {
		return L"�����������";
	}
	else if (arr_str.size() == 1) {
		return arr_str[0];
	}
	else {
		WSTR name;
		size_t size = arr_str.size();
		for (size_t i = 0; i < size; ++i) {
			name += arr_str[i];
			if (i != size - 1) {
				name += L"_";
			}
		}

		if (name != L"�������_����_���_�����...") {
			return name;
		}	
	}
	
	return L"�����������";
}

/*-------------------------------------------------------------
						 WM_COMMAND
--------------------------------------------------------------*/
int Input::wm_command() {

	switch (static_cast<Controls> (LOWORD(m_wParam)))
	{
		case Controls::BUTTON:
		{
			return wm_close(read());
		}
	}
	return 0;
}

/*-------------------------------------------------------------
						  WM_CLOSE
--------------------------------------------------------------*/
int Input::wm_close(WSTR name) {
	flag_focus = false;
	p_rec->add(name); //�������� ������ � ������� ��������
	p_rec->write();		 //�������� ������� � ����
	p_rec->close_picture(); //������ �������
	ShowWindow(hwnd_button, SW_HIDE); //������ ������
	ShowWindow(m_hwnd, SW_HIDE); //������ ����
	return 0;
}

/*-------------------------------------------------------------
						  WM_PAINT
--------------------------------------------------------------*/
int Input::wm_paint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps); //������� �������� ����������
	RECT rect{};
	GetClientRect(m_hwnd, &rect);

	FillRect(hdc, &rect, brush);//������� ���

	HFONT old_hFont = (HFONT)SelectObject(hdc, font); //������������ ����� � �������� ����������

	SetTextColor(hdc, arr_color[1]); //������������ ���� ������
	SetBkColor(hdc, arr_color[0]); //������������ ���� ���� ��� �������


	WSTR intro = L"����������, �� ���������� ����� ������!!!\n"
		L"������� ���� ���, ����� ��������� ��� � ������� ��������.";
	DrawText(hdc, intro.c_str(), -1, &rect,
		DT_PATH_ELLIPSIS | DT_END_ELLIPSIS | DT_WORDBREAK | DT_CENTER);

	SelectObject(hdc, old_hFont); //��������� � �������� �������� ����� 
	EndPaint(m_hwnd, &ps); //���������� �������� ����������
	return 0;
}

/*-------------------------------------------------------------
						  WM_SETFOCUS
--------------------------------------------------------------*/

int Input::wm_setcursor() {
	if (!flag_focus) {
		flag_focus = true;
		SetWindowText(hwnd_edit, L""); //������� ���� ���� �����
	}
	return 0;
}
