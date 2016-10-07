#include "score.h"

//update score
void scoreboard::update_score(int curstatus, int newscore, int curmode){
	if(curstatus == 0) score = 0;
	else if(curstatus == 1) score = prev_score;
	else if(curstatus == 2) score = newscore;
	else score += newscore;
	update_high(curmode);
}

//update steps
void scoreboard::update_steps(int curstatus, int newsteps){
	if(curstatus == 0) steps = 0;
	else if(curstatus == 1) steps = prev_steps;
	else if(curstatus == 2) steps = newsteps;
	else steps++;
}

//update time
void scoreboard::update_time(int curstatus, float time){
	if(curstatus == 3) time_left+=time;
	else time_left = 0;
}
	
//record the current stats
void scoreboard::update_prev(){
	prev_score = score;
	prev_steps = steps;
}

//update the highest score
void scoreboard::update_high(int curmode){
	if(score > highscore) highscore = score;
	if(curmode == 2 && score > timescore) timescore = score;
}

//read the highest score from file
void scoreboard::read_high(){
	std::ifstream ist(filename);
	ist >> highscore >> timescore;
	ist.close();
}

//write the highest score from file
void scoreboard::write_high(){
	std::ofstream ost(filename);
	ost << highscore << " " << timescore;
	ost.close();
}