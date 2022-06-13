#pragma once
#include <Windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#include <string>

class MSI {
public:
	MSI() = delete;
	MSI(std::wstring); //конструктор - передача имени файла
	~MSI(); //деструктор

	void play(); //однократно проиграть музыку
	void play_repeat(); //циклически проигрывать музыку
	void pause(); //поставить музыку на паузу
	void resume(); //снять с паузы
	void stop(); //остановить музыку

	static void mute(); //отключить звук

private:
	std::wstring file_name;
	static bool flag_mute;
};

