#pragma once
#include <string>
#include <sstream>
#include <ctime>

class Get_time {

public:
	std::wstring operator()() {
		tm tm_obj;
		time_t time_obj = time(nullptr);
		localtime_s(&tm_obj, &time_obj);
		
		std::wostringstream wostr;
		if (tm_obj.tm_mday < 10) wostr << L"0";
		wostr << tm_obj.tm_mday << L"-";			//�����
		if (tm_obj.tm_mon + 1 < 10) wostr << L"0";
		wostr << tm_obj.tm_mon + 1 << L"-";			//�����
		wostr << tm_obj.tm_year + 1900 << L" ";		//���
		
		if (tm_obj.tm_hour < 10) wostr << L"0";
		wostr << tm_obj.tm_hour << L":";			//����
		if (tm_obj.tm_min < 10) wostr << L"0";
		wostr << tm_obj.tm_min;						//������
		return wostr.str();
	}
};