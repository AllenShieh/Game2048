#ifndef MENU_H
#define MENU_H

#include "hge.h"
#include "hgesprite.h"
#include "hgeparticle.h"
#include "hgefont.h"
#include "hgeguictrls.h"
#include "logic.h"
#include "const.h"

class menuitem{
public:
	menuitem();
	bool menu_frame();
	bool menu_render();
	bool load_material();
	void game_start();
	~menuitem();
private:
	HGE* hge;
	block* game;
	void create_items();
	bool is_started;
	int volume;
	bool is_paused;

	hgeGUI* gui;
	hgeSprite* block_image[game_size][game_size];
	hgeFont* scorefnt;
	hgeFont* highscore;
	hgeFont* steps;
	hgeFont* the_font;
	hgeFont* timefnt;
	hgeFont* timescore;
	hgeSprite* mouse;
	hgeSprite* backboard;
	hgeSprite* background;
	hgeGUIButton* backbutton;
	hgeGUIButton* playbutton;
	hgeGUIButton* savebutton;
	hgeGUIButton* loadbutton;
	hgeGUIButton* quitbutton;
	hgeGUIListbox* mode_list;
	hgeGUIButton* aboutbutton;
	hgeGUIListbox* theme_list;

	HTEXTURE bg_tex[2];
	HTEXTURE board_tex;
	HTEXTURE mouse_tex;	
	HTEXTURE block_tex[2];
	HEFFECT snd;
	HEFFECT bgm[2];
	HTEXTURE button_tex;
	HCHANNEL channel;
};

#endif