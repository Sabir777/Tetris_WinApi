#pragma once
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#include <string>

class MSI {
public:
	MSI() = delete;
	MSI(std::wstring); //����������� - �������� ����� �����
	~MSI(); //����������

	void play(); //���������� ��������� ������
	void play_repeat(); //���������� ����������� ������
	void pause(); //��������� ������ �� �����
	void resume(); //����� � �����
	void stop(); //���������� ������

	static void mute(); //��������� ����

private:
	std::wstring file_name;
	static bool flag_mute;
};

