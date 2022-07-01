#include "Picture.h"


Picture::Picture() {
	
		int x = GetSystemMetrics(SM_CXSCREEN); //������� ������ ������
		int y = GetSystemMetrics(SM_CYSCREEN); //������� ������ ������
		width_window = x;
		height_window = x / 2;
		if (height_window < y) {
			height_window = y;
			width_window = y * 2;
		}
	
	create_window();
	create_bitmap();
}

Picture::~Picture() {

	DeleteObject(SelectObject(memBit, hOldBitmap)); //��������� �������� � ������ ��������� ������
	DeleteDC(memBit); //������ ����������� ��������
}


/*-------------------------------------------------------------
				 ����������� callback-�������
--------------------------------------------------------------*/
LRESULT Picture::window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Picture* p_obj;
	if (uMsg == WM_NCCREATE) {
		p_obj = static_cast<Picture*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_obj))) {
			if (GetLastError() != 0)
				return false;
		}
	}
	else {
		//������ ��������� �� ������ "Window_obj_init" ���������� ���� ��� ��������������� �������
		p_obj = reinterpret_cast<Picture*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (p_obj) {

		//��������� ������ �� ���������
		p_obj->m_hwnd = hwnd;
		p_obj->m_uMsg = uMsg;
		p_obj->m_wParam = wParam;
		p_obj->m_lParam = lParam;

		//������� ��������� ���������
		return p_obj->application_proc();
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}


/*-------------------------------------------------------------
			  ������� ��������� ��������� Windows
--------------------------------------------------------------*/
LRESULT Picture::application_proc() {

	switch (m_uMsg)
	{
	case WM_PAINT:
		return wm_paint();

	case WM_CLOSE:
		ShowWindow(m_hwnd, SW_HIDE); //������ �������
		return 0;

	default: return DefWindowProc(m_hwnd, m_uMsg, m_wParam, m_lParam);
	}
}


/*-------------------------------------------------------------
					�������� ���� - �������
--------------------------------------------------------------*/
void Picture::create_window() {

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
HWND Picture::create_hwnd_window() {

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
		name_header_window.c_str(), WS_MAXIMIZE | WS_POPUP, 0, 0,
		width_window, height_window, nullptr, nullptr, nullptr, this);
	
	message = L"Error, can't create window: ";
	message += name_header_window;
	mess = STR(std::begin(message), end(message));

	if (!m_hwnd)
		throw std::runtime_error(mess.c_str());

	
	return m_hwnd;
}

/*-------------------------------------------------------------
		�������� ��������� � ������ � �������� �������
--------------------------------------------------------------*/
void Picture::create_bitmap() {

	hBitmap = LoadImage(NULL, L"van_gog.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //�������� ������
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap); //�������� ����������� ������������ bitmap'a

	HDC hdc = GetDC(m_hwnd); //������� �������� ����������

	memBit = CreateCompatibleDC(hdc); //������� ����������� �������� ���������� � ������
	hOldBitmap = SelectObject(memBit, hBitmap); //������� ������ � ����������� ��������

	ReleaseDC(m_hwnd, hdc); //���������� ���������
}

/*-------------------------------------------------------------
						�������� ����
--------------------------------------------------------------*/
void Picture::show_window() {

	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //����� ����
	UpdateWindow(m_hwnd); //���������� ����
}

/*-------------------------------------------------------------
						������ ����
--------------------------------------------------------------*/
void Picture::close() {
	ShowWindow(m_hwnd, SW_HIDE); //������ �������
} 


/*-------------------------------------------------------------
						 WM_PAINT
--------------------------------------------------------------*/
int Picture::wm_paint() {

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);

	StretchBlt(hdc, //���������� ��������� DC
		0, //x - ����� ������� ���� ���������
		0, //� - ����� ������� ���� ���������
		width_window, // ������ �������� ��������������
		height_window, // ������ �������� ��������������
		memBit, // ���������� ��������� DC
		0, 0, //���������� ������ �������� ���� ���������
		Bitmap.bmWidth, //������ �������
		Bitmap.bmHeight, //������ �������
		SRCCOPY //��� �������� - �����������
	);

	EndPaint(m_hwnd, &ps);

	return 0;
}