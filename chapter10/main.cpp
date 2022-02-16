/*-------------------------------------------------------------
	
	[main.cpp]
	Author : èoçá„ƒëæ

--------------------------------------------------------------*/
#include"Application.h"
#include<Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	auto& app = Application::Instance();
	if (!app.Begin()) {
		return -1;
	}
	app.Tick();
	app.End();
	return 0;
}