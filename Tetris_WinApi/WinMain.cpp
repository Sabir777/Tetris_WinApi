#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//манифест

#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include "Tetris.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	srand((unsigned int)time(NULL));
	Tetris app;
	app.run();
}