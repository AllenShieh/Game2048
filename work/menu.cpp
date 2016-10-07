#include "menu.h"
#include <atlstr.h>

extern bool FrameFunc();
extern bool RenderFunc();

void menuitem::game_start(){
	hge = hgeCreate(HGE_VERSION);

	//hge->System_SetState(HGE_LOGFILE,"info.log");
	hge->System_SetState(HGE_FRAMEFUNC,FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);
	hge->System_SetState(HGE_TITLE,"2048");
	//hge->System_SetState(HGE_FPS,100);
	hge->System_SetState(HGE_WINDOWED,true);
	hge->System_SetState(HGE_SCREENWIDTH,600);
	hge->System_SetState(HGE_SCREENHEIGHT,600);
	hge->System_SetState(HGE_SHOWSPLASH,false);
	hge->System_SetState(HGE_USESOUND,true);

	if(hge->System_Initiate())
	{
		//load materials. if fails, shut down
		if(load_material())
		{
			MessageBox(NULL,"Can't load materials","Error",MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
		}

		//Let's ROOOOOOOOCK!
		else hge->System_Start();
	}
	else MessageBox(NULL,hge->System_GetErrorMessage(),"Error",MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}

//constructor
menuitem::menuitem():hge(0),is_started(0),volume(100),is_paused(0){
	game = new block();
	//other items are created after materials are loaded
}

//destructor
menuitem::~menuitem(){
	delete game;
	delete gui;
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			delete block_image[i][j];
	delete scorefnt;
	delete highscore;
	delete steps;
	delete timefnt;
	delete timescore;
	delete background;
	delete backboard;
	delete mouse;
	hge->Texture_Free(mouse_tex);
	hge->Texture_Free(bg_tex[0]);
	hge->Texture_Free(bg_tex[1]);
	hge->Texture_Free(board_tex);
	hge->Texture_Free(block_tex[0]);
	hge->Texture_Free(block_tex[1]);
	hge->Effect_Free(snd);
	hge->Effect_Free(bgm[0]);
	hge->Effect_Free(bgm[1]);
	hge->Texture_Free(button_tex);

	hge->System_Shutdown();
	hge->Release();
}

//framefunc
bool menuitem::menu_frame(){
	//win or lose
	if(game->lose()){
		MessageBox(NULL,"You lose\n","Game over",MB_OK | MB_ICONASTERISK);
		game->initial();
	}
	if(game->win()){
		MessageBox(NULL,"You win\n","Congratulations",MB_OK | MB_ICONINFORMATION);
		game->initial();
	}
	//deal with having consumed all the time
	if((int)game->get_time() == 60 && is_started && mode_list->GetSelectedItem() == timemode){
		CString str;
		str.Format("Time is up! Your score: %d",game->get_score());
		MessageBox(NULL,str,"Game over",MB_OK | MB_ICONINFORMATION);
		game->set_time(restart);
		game->initial();
		is_started = 0; //make the clock stop
	}

	//get time passed
	float dt = hge->Timer_GetDelta();
	int id;
	static int lastid = 0;
	
	//deal with the pressed keys
	if (hge->Input_KeyDown(HGEK_ESCAPE)) return true;
	if (hge->Input_KeyDown(HGEK_LEFT)){
		game->left(mode_list->GetSelectedItem());
		hge->Effect_Play(snd);
	}
	if (hge->Input_KeyDown(HGEK_RIGHT)){
		game->right(mode_list->GetSelectedItem());
		hge->Effect_Play(snd);
	}
	if (hge->Input_KeyDown(HGEK_UP)){
		game->up(mode_list->GetSelectedItem());
		hge->Effect_Play(snd);
	}
	if (hge->Input_KeyDown(HGEK_DOWN)){
		game->down(mode_list->GetSelectedItem());
		hge->Effect_Play(snd);
	}
	//deal with volume adjustment
	if (hge->Input_GetKey()==HGEK_EQUALS){
		if(volume<100) volume += 10;
		hge->Channel_SetVolume(channel,volume);
	}
	if (hge->Input_GetKey()==HGEK_MINUS){
		if(volume>0) volume -= 10;
		hge->Channel_SetVolume(channel,volume);
	}
	if (hge->Input_GetKey()==HGEK_SPACE){
		if(is_paused==0) {hge->Channel_Pause(channel);is_paused=1;}
		else {hge->Channel_Resume(channel);is_paused=0;}
	}

	id = gui->Update(dt);
	if(id == -1)
	{
		//deal with GUI controls
		if(backbutton->GetState()){
			if(mode_list->GetSelectedItem() == timemode) MessageBox(NULL,"Can't use back in time mode","Note",MB_OK | MB_ICONASTERISK);
			else game->back_to_prev();
			backbutton->SetState(false);
		}
		if(playbutton->GetState()){
			game->reset();
			game->set_time(restart);
			is_started = 1; //make the clock run
			playbutton->SetState(false);
		}
		//time mode forbids save and load function
		if(savebutton->GetState()){
			if(mode_list->GetSelectedItem() == timemode) MessageBox(NULL,"Can't use save in time mode","Note",MB_OK | MB_ICONASTERISK);
			else game->save_game();
			savebutton->SetState(false);
		}
		if(loadbutton->GetState()){
			if(mode_list->GetSelectedItem() == timemode) MessageBox(NULL,"Can't use load in time mode","Note",MB_OK | MB_ICONASTERISK);
			else game->load_game();
			loadbutton->SetState(false);
		}
		if(quitbutton->GetState()) 
			return true;
		if(aboutbutton->GetState()){
			MessageBox(NULL,abouttext,"About",MB_OK | MB_ICONQUESTION);
			aboutbutton->SetState(false);
		}
		switch(lastid)
		{
		case 1:
			gui->SetFocus(1);
			gui->Enter();
			break;
		case 2:
			gui->SetFocus(2);
			gui->Enter();
			break;
		case 3:
			gui->SetFocus(3);
			gui->Enter();
			break;
		case 4:
			gui->SetFocus(4);
			gui->Enter();
			break;
		case 5:
			gui->SetFocus(5);
			gui->Enter();
			break;
		case 6:
			//when changing mode, initialize the game
			gui->SetFocus(6);
			gui->Enter();
			game->initial();
			is_started = 0;
			break;
		case 7:
			gui->SetFocus(7);
			gui->Enter();
			break;
		case 8:
			//change style
			gui->SetFocus(8);
			gui->Enter();
			background->SetTexture(bg_tex[theme_list->GetSelectedItem()]);
			for(int i=0;i<game_size;i++)
				for(int j=0;j<game_size;j++)
					block_image[i][j]->SetTexture(block_tex[theme_list->GetSelectedItem()]);
			hge->Channel_Stop(channel); //stop the current music
			channel = hge->Effect_PlayEx(bgm[theme_list->GetSelectedItem()],volume,0,1,true);
			is_paused = 0;
			break;
		}
	}
	else if(id) {lastid = id; gui->Leave();}

	//if not started, there is no time change
	if(!is_started) dt = 0;
	game->set_time(normal,dt);

	return false;
}

//renderfunc
bool menuitem::menu_render(){
	hge->Gfx_BeginScene();

	//background, backboard, gui
	background->Render(0,0);
	backboard->Render((float)55,(float)200);
	gui->Render();
	
	//blocks
	for(int i=0;i<game_size;i++){
		for(int j=0;j<game_size;j++)
			if(*(game->get_blocks()+4*i+j)) {
				block_image[i][j]->SetTextureRect((float)game->get_image_pos(*(game->get_blocks()+4*i+j)),(float)0,(float)block_width,(float)block_width);
				block_image[i][j]->Render((float)block_width*j+55,(float)block_width*i+200);
			}
	}
	
	//score, highest score, steps
	scorefnt->printf(50,50,HGETEXT_LEFT,"Score: %d",game->get_score());
	highscore->printf(375,50,HGETEXT_LEFT,"Highest: %d",game->get_high());
	steps->printf(225,50,HGETEXT_LEFT,"Steps: %d",game->get_steps());
	//when in time mode, these things show up
	if(mode_list->GetSelectedItem() == timemode){
		timefnt->printf(150,10,HGETEXT_LEFT,"Time left: %.0f",(float)60-game->get_time());
		timescore->printf(325,10,HGETEXT_LEFT,"Timemode score: %d",game->get_timescore());
	}

	hge->Gfx_EndScene();
	return false;
}

//load materials: if it fails, return 1
bool menuitem::load_material(){
	//load materials
	bg_tex[0] = hge->Texture_Load("bg1.png");
	bg_tex[1] = hge->Texture_Load("bg2.png");
	mouse_tex = hge->Texture_Load("cursor.png");
	snd = hge->Effect_Load("menu.wav");
	bgm[0] = hge->Effect_Load("bgm1.mp3");
	bgm[1] = hge->Effect_Load("bgm2.mp3");
	block_tex[0] = hge->Texture_Load("block1.png");
	block_tex[1] = hge->Texture_Load("block2.png");
	board_tex = hge->Texture_Load("board.png");
	button_tex = hge->Texture_Load("button.png");
	channel = hge->Effect_PlayEx(bgm[0],100,0,1,true);

	create_items(); //create items

	return !bg_tex[0]||!bg_tex[1]||!mouse_tex||!snd||!bgm[0]||!bgm[1]||!block_tex[0]||!block_tex[1]||!board_tex||!button_tex;
}

//create items
void menuitem::create_items(){
	for(int i=0;i<game_size;i++){
		for(int j=0;j<game_size;j++){
			block_image[i][j] = new hgeSprite(block_tex[0],(float)game->get_image_pos(*(game->get_blocks()+4*i+j)),(float)0,(float)block_width,(float)block_width);
		}
	}
	//all the fonts
	scorefnt = new hgeFont("font1.fnt");
	highscore = new hgeFont("font1.fnt");
	steps = new hgeFont("font1.fnt");
	timefnt = new hgeFont("font1.fnt");
	timescore = new hgeFont("font1.fnt");
	the_font = new hgeFont("font1.fnt");

	//background, mouse and backboard
	background = new hgeSprite(bg_tex[0],0,0,600,600);
	mouse = new hgeSprite(mouse_tex,0,0,32,32);
	backboard = new hgeSprite(board_tex,0,0,300,300);

	//controls
	backbutton = new hgeGUIButton(1,400,325,100,49,button_tex,0,0);
	backbutton->SetMode(true);
	playbutton = new hgeGUIButton(2,400,100,100,49,button_tex,0,50);
	playbutton->SetMode(true);
	savebutton = new hgeGUIButton(3,400,175,100,49,button_tex,0,100);
	savebutton->SetMode(true);
	loadbutton = new hgeGUIButton(4,400,250,100,49,button_tex,0,150);
	loadbutton->SetMode(true);
	quitbutton = new hgeGUIButton(5,400,475,100,49,button_tex,0,200);
	quitbutton->SetMode(true);
	mode_list = new hgeGUIListbox(6,50,100,250,100,the_font,0xFFffb90f,0xFFff4500,0xFFffff00);
	mode_list->AddItem("Standard Mode");
	mode_list->AddItem("Better or Worse Mode");
	mode_list->AddItem("Time Mode");
	aboutbutton = new hgeGUIButton(7,400,400,100,49,button_tex,0,250);
	aboutbutton->SetMode(true);
	theme_list = new hgeGUIListbox(8,50,525,250,100,the_font,0xFF87ceff,0xFF104e8b,0xFF00ffff);
	theme_list->AddItem("Standard Theme");
	theme_list->AddItem("The World Cup Theme");

	//GUI
	gui = new hgeGUI();
	gui->AddCtrl(backbutton);
	gui->AddCtrl(playbutton);
	gui->AddCtrl(savebutton);
	gui->AddCtrl(loadbutton);
	gui->AddCtrl(quitbutton);
	gui->AddCtrl(mode_list);
	gui->AddCtrl(aboutbutton);
	gui->AddCtrl(theme_list);
	gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	gui->SetCursor(mouse);
	gui->SetFocus(1);
	gui->Enter();
}