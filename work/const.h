#ifndef CONST_H
#define CONST_H

enum status{restart=0, back=1, load=2, normal=3}; //game status
enum mode{standard=0, bow=1, timemode=2}; //game mode
const int game_size = 4;
const int block_width = 75;
const char* const savegame = "save.txt"; //contains what you save
const char* const filename = "highscore.txt"; //contains highest score and time limited score
const char* const abouttext = "Use UP/DOWN/LEFT/RIGHT to play\nUse -/+/SPACE to decrease volume, increase volume and pause\nCreated by Allen\n";

#endif