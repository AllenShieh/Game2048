#ifndef SCORE_H
#define SCORE_H

#include<iostream>
#include<fstream>
#include"const.h"

class scoreboard{
public:
	scoreboard():score(0),highscore(0),steps(0),prev_score(0),prev_steps(0),time_left(0),timescore(0){};

	//get private objects
	int get_high(){return highscore;}
	int get_score(){return score;}
	int get_steps(){return steps;}
	float get_time(){return time_left;}
	int get_timescore(){return timescore;}

	//update stats
	void update_score(int curstatus, int newscore=0, int curmode=0);
	void update_steps(int curstatus, int newsteps=0);
	void update_high(int curmode);
	void read_high();
	void write_high();
	void update_prev();
	void update_time(int curstatus, float time=0);

	//~scoreboard()
private:
	int score;
	int highscore;
	int prev_score;
	int steps;
	int prev_steps;
	float time_left;
	int timescore;
};

#endif