#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <map>
#include "Scroll_Parent.h"
#include "Scroll_records.h"
#include "Picture.h"
#include "Input.h"
#include "Get_time.h"

using WSTR = std::wstring;
using map_int_wstring = std::map<int, WSTR>;

class Records : public Scroll {
public:
	Records() = delete;
	Records(std::wstring file_name);

	void init_create_table();
	void read();
	void write() override;
	void add(WSTR) override;
	int max_table();
	map_int_wstring get_table() override;

	void show_window(); //показать таблицу рекордов
	void new_record(int); //новый рекорд - запись имени игрока
	void close_picture() override; //закрыть картину

private:
	std::wstring file_name;
	std::map<int, std::wstring> table{};
	bool flag_error_read{ false };
	int score{};

	Scroll_records srec{ this }; //объект - окно таблицы рекордов
	Picture pic; //окно с картиной
	Input input{ this }; //ввод имени игрока
};
