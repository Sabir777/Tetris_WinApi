
//компилирую отдельно файл sound1.exe и вызываю его из Game.cpp
/*
#pragma comment(lib, "winmm.lib")
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	PlaySound(L"row.wav", NULL, SND_SYNC | SND_NODEFAULT);
}
*/

/* компилирую отдельно файл sound2.exe и вызываю его из Game.cpp
*  //Звук тетриса
#pragma comment(lib, "winmm.lib")
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	PlaySound(L"row.wav", NULL, SND_SYNC | SND_NODEFAULT);
	PlaySound(L"tetris.wav", NULL, SND_SYNC | SND_NODEFAULT);
}
*/