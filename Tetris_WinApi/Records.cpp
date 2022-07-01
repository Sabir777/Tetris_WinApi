#include "Records.h"

Records::Records(std::wstring file_name) : file_name{ file_name } {
	read(); //�������� ������ �� ����� ��� ������� ���������
}


void Records::init_create_table() { //������ ��������� ������ ������������� ������� ��������
	
	table = std::map<int, std::wstring>{
		{100, WSTR{L"�������"} + L" 11-03-2022 12:45"},
		{200, WSTR{L"�������"} + L" 05-04-2022 01:12"},
		{300, WSTR{L"���������"} + L" 23-05-2022 12:30"},
		{400, WSTR{L"�����"} + L" 03-06-2022 14:21"},
		{500, WSTR{L"��������"} + L" 10-06-2022 21:39"}
	};
}

/*-------------------------------------------------------------
				    ����� ������� �� �����
--------------------------------------------------------------*/
void Records::read() {

	std::wifstream ifst(file_name.c_str()); //�������� ����� �� ������

	if (ifst) { //�������� ��� ���� ��������
		ifst.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>)); //������������ ��������� ������ ������

		
		
		while (!ifst.eof()) {
			int key;
			std::wstring name, date, clock;

			ifst >> key >> name >> date >> clock; //�������� ����: ����(���������� �����) - ��� �����������

			if ((ifst.fail() || ifst.bad()) && !ifst.eof()) {
				MessageBox(nullptr, L"������!", L"ifst.fail() || ifst.bad()", MB_OK);
				flag_error_read = true; //��������� ����:������ ������
				break;
			}
			table[key] = name + L" " + date + L" " + clock; //��������� ����-�������� � ��������� ��� ���������� � ����� ����� � ������� ����������
		}
	}
	else {
		flag_error_read = true; //��������� ����:������ ������
	}
	ifst.close();

	if (flag_error_read) {
		init_create_table(); //���� ���� �� ��� ��������, �� ������� ����� ������
	}
}


/*-------------------------------------------------------------
				   ��������� ������� � ����
--------------------------------------------------------------*/
void Records::write() { 
	if (table.size() != 0) {

		std::wofstream ofst(file_name.c_str());
		if (!ofst) return; //�����, ���� �� �������� �����

		auto convert_UTF8 = [](std::wstring wstr) ->std::string //���������� wchar_t � unicode
		{
			const wchar_t* ws = wstr.c_str();
			std::wstring_convert <std::codecvt_utf8 <wchar_t>, wchar_t> convert;
			return convert.to_bytes(ws);
		}; 

		auto end = table.end();
		auto last = end; --last;


		for (auto it{ table.begin() }; it != end; ++it) {

			std::wstring str = std::to_wstring((*it).first) + L" " + (*it).second;
			if (it != last) {
				str += L"\n"; //���� �� ������ ������� �������, �� �� ������������� ��������� ������
			}

			ofst << convert_UTF8(str).c_str();

		}

		ofst.close();
	}
}


void Records::add(WSTR name) {
	Get_time gt;
	table[score] = name + L" " + gt();
}

int Records::max_table() {
	auto it = table.end(); --it;
	return (*it).first;
}


/*-------------------------------------------------------------
		������� ����� ���������� � �������� ��������
--------------------------------------------------------------*/
map_int_wstring Records::get_table() {
	return table;
}

/*-------------------------------------------------------------
				   �������� ������� ��������
--------------------------------------------------------------*/
void Records::show_window() {
	srec.show_window();
}


/*-------------------------------------------------------------
				���������������� ����� ������
--------------------------------------------------------------*/
void Records::new_record(int n) {
	if (n > max_table()) {
		score = n;
		pic.show_window(); //�������� �������
		input.show_window(); //�������� ������ ����� �����
	}
}

/*-------------------------------------------------------------
						������ �������
--------------------------------------------------------------*/
void Records::close_picture() {
	pic.close();
}