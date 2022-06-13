#include "Records.h"

Records::Records(std::wstring file_name) : file_name{ file_name } {
	read(); //загружаю данные из файла при запуске программы
}


void Records::init_create_table() { //создаю начальный список инициализации таблицы рекордов
	
	table = std::map<int, std::wstring>{
		{100, WSTR{L"Никотин"} + L" 11-03-2022 12:45"},
		{200, WSTR{L"Опасный"} + L" 05-04-2022 01:12"},
		{300, WSTR{L"Скудоумец"} + L" 23-05-2022 12:30"},
		{400, WSTR{L"Голем"} + L" 03-06-2022 14:21"},
		{500, WSTR{L"Афонасий"} + L" 10-06-2022 21:39"}
	};
}

/*-------------------------------------------------------------
				    Читаю рекорды из файла
--------------------------------------------------------------*/
void Records::read() {

	std::wifstream ifst(file_name.c_str()); //открываю поток на чтение

	if (ifst) { //проверяю что файл открылся
		ifst.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>)); //устанавливаю кодировку потока юникод

		
		
		while (!ifst.eof()) {
			int key;
			std::wstring name, date, clock;

			ifst >> key >> name >> date >> clock; //считываю пару: ключ(количество очков) - имя рекордсмена

			if ((ifst.fail() || ifst.bad()) && !ifst.eof()) {
				MessageBox(nullptr, L"Ошибка!", L"ifst.fail() || ifst.bad()", MB_OK);
				flag_error_read = true; //выставляю флаг:ошибка чтения
				break;
			}
			table[key] = name + L" " + date + L" " + clock; //записываю ключ-значение в контейнер все хранящиеся в файле имена и рекорды поочередно
		}
	}
	else {
		flag_error_read = true; //выставляю флаг:ошибка чтения
	}
	ifst.close();

	if (flag_error_read) {
		init_create_table(); //если файл не был прочитан, то создать новый список
	}
}


/*-------------------------------------------------------------
				   Записываю рекорды в файл
--------------------------------------------------------------*/
void Records::write() { 
	if (table.size() != 0) {

		std::wofstream ofst(file_name.c_str());
		if (!ofst) return; //ухожу, если не открылся поток

		auto convert_UTF8 = [](std::wstring wstr) ->std::string //преобразую wchar_t в unicode
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
				str += L"\n"; //если не делать данного условия, то не прочитывается последняя строка
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
		Вернуть копию контейнера с записями рекордов
--------------------------------------------------------------*/
map_int_wstring Records::get_table() {
	return table;
}

/*-------------------------------------------------------------
				   Показать таблицу рекордов
--------------------------------------------------------------*/
void Records::show_window() {
	srec.show_window();
}


/*-------------------------------------------------------------
				Зарегистрировать новый рекорд
--------------------------------------------------------------*/
void Records::new_record(int n) {
	if (n > max_table()) {
		score = n;
		pic.show_window(); //показать картину
		input.show_window(); //показать окошко ввода имени
	}
}

/*-------------------------------------------------------------
						Скрыть картину
--------------------------------------------------------------*/
void Records::close_picture() {
	pic.close();
}