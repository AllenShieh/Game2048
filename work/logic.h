#ifndef LOGIC_H
#define LOGIC_H

#include"const.h"
#include"score.h"

class block{
public:
	block();
	//move functions
	void left(int curmode);
	void right(int curmode);
	void up(int curmode);
	void down(int curmode);

	//lose or win
	bool lose();
	bool win();

	//board update functions
	void reset();
	void initial();
	void back_to_prev();
	void save_game();
	void load_game();

	//get private objects
	float get_image_pos(int b);
	int* get_blocks(){return blocks[0];}
	int get_score(){return board->get_score();}
	int get_high(){return board->get_high();}
	int get_steps(){return board->get_steps();}
	void set_time(int curstatus, float time=0){board->update_time(curstatus,time);}
	float get_time(){return board->get_time();}
	int get_timescore(){return board->get_timescore();}

	~block();
private:
	scoreboard* board;
	int prev_blocks[game_size][game_size];
	int blocks[game_size][game_size];

	//inside functions
	void prev_rec();
	bool full();
	void add_block(int curstatus, int curmode);

	//support functions
	int find_left(int,int);
	int find_right(int,int);
	int find_up(int,int);
	int find_down(int,int);
	bool left_move();
	bool right_move();
	bool up_move();
	bool down_move();
};

#endif