#include "menu.h"
#include "time.h"

menuitem* menu = 0;

bool FrameFunc()
{
	return menu->menu_frame();
}

bool RenderFunc()
{
	return menu->menu_render();
}

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	//time seed
	srand(time(0));
	menu = new menuitem();
	menu->game_start();
	delete menu;
	return 0;
}