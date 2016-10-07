#include"logic.h"
#include"math.h"

//constructor
block::block(){
	for(int i=0;i<game_size;i++){
		for(int j=0;j<game_size;j++){
			blocks[i][j] = 0;
			prev_blocks[i][j] = 0;
		}
	}
	board = new scoreboard();
	board->read_high(); //read the highest score from the file
};

//destructor
block::~block(){
	board->write_high(); //write the highest score into the file
	delete board;
}

//reset the game
void block::reset(){
	initial();
	add_block(restart,standard);
	add_block(restart,standard);
}

//initial the board(different from reset)
void block::initial(){
	prev_rec();
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			blocks[i][j] = 0;
	board->update_score(restart);
	board->update_steps(restart);
	board->update_time(restart);
}

//return the position of the image in the picture
float block::get_image_pos(int b){
	return block_width * log((float)b)/log((float)2) - block_width;
}

//record the current board
void block::prev_rec(){
	for(int i = 0;i<game_size;i++)
		for(int j = 0;j<game_size;j++)
			prev_blocks[i][j] = blocks[i][j];
	board->update_prev();
}

//back to the previous board
void block::back_to_prev(){
	for(int i = 0;i<game_size;i++)
		for(int j = 0;j<game_size;j++)
			blocks[i][j] = prev_blocks[i][j];
	board->update_score(back);
	board->update_steps(back);
}

//see if 16 blocks have number
bool block::full(){
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			if(!blocks[i][j]) return false;
	return true;
}

//see if the player loses
bool block::lose(){
	//when full and can't move
	if(full()&&!left_move()&&!right_move()&&!up_move()&&!down_move()) return true;
	return false;
}

//see if the player wins
bool block::win(){
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			if(blocks[i][j] == 4096) return true;
	return false;
}

//save
void block::save_game(){
	std::ofstream ost(savegame);
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			ost<<blocks[i][j]<<" ";
	ost<<board->get_score()<<" ";
	ost<<board->get_steps();
}

//load
void block::load_game(){
	prev_rec();
	std::ifstream ist(savegame);
	for(int i=0;i<game_size;i++)
		for(int j=0;j<game_size;j++)
			ist>>blocks[i][j];
	int load_score;
	int load_steps;
	ist>>load_score;
	ist>>load_steps;
	board->update_score(load, load_score);
	board->update_steps(load, load_steps);
}

//when move, first check if it can combines then check if it can move
//left move
void block::left(int curmode)
{
	if(left_move()){
		prev_rec();
		board->update_steps(normal);
		int n;
		for(int i=0;i<game_size;i++){
			for(int j=0;j<game_size-1;j++){
				n=find_right(i,j);
				if( n == -1) 
					break;
				if( blocks[i][j] == blocks[i][n]){
					blocks[i][j] *= 2;
					board->update_score(normal, blocks[i][j], curmode);
					blocks[i][n] = 0;
				}
			}
			for(int j=0;j<game_size-1;j++){
				if( blocks[i][j] ) continue;
				n=find_right(i,j);
				if( n != -1){
					blocks[i][j] = blocks[i][n];
					blocks[i][n] = 0;
				}
			}
		}
		add_block(normal,curmode);
	}
}

//right move
void block::right(int curmode)
{
	if(right_move()){
		prev_rec();
		board->update_steps(normal);
		int n;
		for(int i=0;i<game_size;i++){
			for(int j=game_size-1;j>-1;j--){
				n=find_left(i,j);
				if( n == -1) 
					break;
				if( blocks[i][j] == blocks[i][n]){
					blocks[i][j] *= 2;
					board->update_score(normal,blocks[i][j],curmode);
					blocks[i][n] = 0;
				}
			}
			for(int j=game_size-1;j>-1;j--){
				if( blocks[i][j] ) continue;
				n=find_left(i,j);
				if( n != -1){
					blocks[i][j] = blocks[i][n];
					blocks[i][n] = 0;
				}
			}
		}
		add_block(normal,curmode);
	}
}

//up move
void block::up(int curmode)
{
	if(up_move()){
		prev_rec();
		board->update_steps(normal);
		int n;
		for(int j=0;j<game_size;j++){
			for(int i=0;i<game_size-1;i++){
				n=find_down(i,j);
				if( n == -1) 
					break;
				if( blocks[i][j] == blocks[n][j]){
					blocks[i][j] *= 2;
					board->update_score(normal,blocks[i][j],curmode);
					blocks[n][j] = 0;
				}
			}
			for(int i=0;i<game_size-1;i++){
				if( blocks[i][j] ) continue;
				n=find_down(i,j);
				if( n != -1){
					blocks[i][j] = blocks[n][j];
					blocks[n][j] = 0;
				}
			}
		}
		add_block(normal,curmode);
	}
}

//down move
void block::down(int curmode)
{
	if(down_move()){
		prev_rec();
		board->update_steps(normal);
		int n;
		for(int j=0;j<game_size;j++){
			for(int i=game_size-1;i>-1;i--){
				n=find_up(i,j);
				if( n == -1) 
					break;
				if( blocks[i][j] == blocks[n][j]){
					blocks[i][j] *= 2;
					board->update_score(normal,blocks[i][j],curmode);
					blocks[n][j] = 0;
				}
			}
			for(int i=game_size-1;i>-1;i--){
				if( blocks[i][j] ) continue;
				n=find_up(i,j);
				if( n != -1){
					blocks[i][j] = blocks[n][j];
					blocks[n][j] = 0;
				}
			}
		}
		add_block(normal,curmode);
	}
}

//add a block
void block::add_block(int curstatus, int curmode){
	int x = rand()%game_size;
	int y = rand()%game_size;
	if(curstatus == normal && curmode == 1){
		int luck = rand()%4;
		if(!luck) {blocks[x][y] = 2;return ;}
	}
	int is_two = rand()%8;
	while(blocks[x][y]!=0){
		x = rand()%game_size;
		y = rand()%game_size;
	}
	if(is_two || curstatus == restart) blocks[x][y] = 2;
	else blocks[x][y] = 4;
}

/*--------------------------------------------------------------------------------*/
//return being able to do a left move
bool block::left_move(){
	for(int i=0;i<game_size;i++)
		for(int j=1;j<game_size;j++){
			if(blocks[i][j] == blocks[i][j-1] && blocks[i][j] != 0) return true;
			if(blocks[i][j] != 0 && blocks[i][j-1] == 0) return true;
		}
	return false;
}

//return being able to do a right move
bool block::right_move(){
	for(int i=0;i<game_size;i++)
		for(int j=1;j<game_size;j++){
			if(blocks[i][j] == blocks[i][j-1] && blocks[i][j] != 0) return true;
			if(blocks[i][j] == 0 && blocks[i][j-1] != 0) return true;
		}
	return false;
}

//return being able to do a up move
bool block::up_move(){
	for(int j=0;j<game_size;j++)
		for(int i=1;i<game_size;i++){
			if(blocks[i][j] == blocks[i-1][j] && blocks[i][j] != 0) return true;
			if(blocks[i][j] != 0 && blocks[i-1][j] == 0) return true;
		}
	return false;
}

//return being able to do a down move
bool block::down_move(){
	for(int j=0;j<game_size;j++)
		for(int i=1;i<game_size;i++){
			if(blocks[i][j] == blocks[i-1][j] && blocks[i][j] != 0) return true;
			if(blocks[i][j] == 0 && blocks[i-1][j] != 0) return true;
		}
	return false;
}

//find the nearest right block that has number
int block::find_right(int i, int j)
{
	for(int n = j+1;n<game_size;n++){
		if(blocks[i][n]) return n;
	}
	return -1;
}

//find the nearest left block that has number
int block::find_left(int i, int j)
{
	for(int n = j-1;n>-1;n--){
		if(blocks[i][n]) return n;
	}
	return -1;
}

//find the nearest down block that has number
int block::find_down(int i,int j)
{
	for(int n = i+1;n<game_size;n++){
		if(blocks[n][j]) return n;
	}
	return -1;
}

//find the nearest up block that has number
int block::find_up(int i,int j)
{
	for(int n = i-1;n>-1;n--){
		if(blocks[n][j]) return n;
	}
	return -1;
}