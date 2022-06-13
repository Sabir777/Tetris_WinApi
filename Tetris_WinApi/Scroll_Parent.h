#pragma once
#include <string>
#include <map>

using WSTR = std::wstring;
using map_int_wstring = std::map<int, WSTR>;

class Scroll {
public:
	virtual map_int_wstring get_table() { return {}; }
	virtual void close_picture() {}
	virtual void add(WSTR) {}
	virtual void write() {}

};