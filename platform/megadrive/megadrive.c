/*
    (c) 2018 Affonso Amendola
    (c) 2018 Felipe Sanches <juca@members.fsf.org>
    
    This file is part of WumpusVGA.

    WumpusVGA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WumpusVGA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WumpusVGA.  If not, see <https://www.gnu.org/licenses/>
*/

#include "wumpus.h"
#include "hw_md.h"

extern int boardPosX;
extern int boardPosY;
extern int playerPosX;
extern int playerPosY;
extern int wumpusPosX;
extern int wumpusPosY;
extern int arrowPosX;
extern int arrowPosY;
extern int hasArrow;
extern int secretExitX;
extern int secretExitY;
//extern int goldPosX;
//extern int goldPosY;
extern int escaping;
extern int showWumpus;
extern int showWumpus;
//extern int showGold;
//extern int showHoles;
extern int gameRunning;


enum {
	BLANK_CHAR,
	PLAYER,
	GOLD,
	WUMPUS_AWAKEN,
	WUMPUS_SLEEPING1,
	WUMPUS_SLEEPING2,
	OPEN_FLOOR,
	HIDDEN_FLOOR
};

//routine for placing a character on screen
void set_char(int x, int y, char char_code){
	char* c[] = {
		" ", // blank
		"P", // player
		"G", // gold
		"W", // wumpus awaken
		"z", //        sleeping 1
		"Z", //        sleeping 2
		".", // open floor
		"x"  // hidden floor
	};
	put_str(c[(int) char_code], 0x2000, x, y);
}

// Routine to print a line of text at a
// given screen coordinate
void print_line(const char* str, int x, int y){
	put_str("                               ", 0x2000, 0, y);
	put_str((char *) str, 0x2000, x, y);
}

void print_score(char score)
{
	//TODO: Implement-me!
}

void init_video(){
	//TODO: Implement-me!

	clear_screen();
}

void init_system(){
	init_video();
	set_game_level(3);//max level by default
	gameRunning = 1;
}

void integer_to_string(int value, char string[8])
{
	//TODO: Implement-me!
}

int read_user_input()
{
	int input = 0;
	short buttons = SEGA_CTRL_A;

	while(buttons & (SEGA_CTRL_A |
	                 SEGA_CTRL_B |
	                 SEGA_CTRL_UP |
	                 SEGA_CTRL_DOWN |
	                 SEGA_CTRL_LEFT |
	                 SEGA_CTRL_RIGHT)) {
		buttons = get_pad(0);
	}

	while(!(buttons & (SEGA_CTRL_A |
	                   SEGA_CTRL_B |
	                   SEGA_CTRL_UP |
	                   SEGA_CTRL_DOWN |
	                   SEGA_CTRL_LEFT |
	                   SEGA_CTRL_RIGHT))) {
		buttons = get_pad(0);
	}

	if (buttons & SEGA_CTRL_RIGHT) input |= INPUT_JOYSTICK_RIGHT;
	if (buttons & SEGA_CTRL_LEFT) input |= INPUT_JOYSTICK_LEFT;
	if (buttons & SEGA_CTRL_DOWN) input |= INPUT_JOYSTICK_DOWN;
	if (buttons & SEGA_CTRL_UP) input |= INPUT_JOYSTICK_UP;
	if (buttons & SEGA_CTRL_A) input |= INPUT_ACTION_FIRE;
	if (buttons & SEGA_CTRL_B) input |= INPUT_ACTION_GET;

	input |= INPUT_LEVEL_ULTRA;
	return input;
}

void wait_for_user_input()
{
	//This is a bit silly...
	read_user_input();
}

int main()
{
	init_system();
	wumpus_main();
	return 0;
}

void draw_message_box(const char* line1,
                      const char* line2)
{
	print_line(line1, 4, 4);
	print_line(line2, 4, 5);
}

void display_title_screen()
{
	clear_screen();
	draw_message_box("TITLE SCREEN", "---------------");
}

void display_game_over_screen()
{
	clear_screen();
	draw_message_box("GAME OVER SCREEN", "");
}

void fill_screen(char color)
{
	//TODO: Implement-me!
}

int abs(int value)
{
	return (value > 0) ? value : -value;
}

void after_gameover()
{
	//TODO: Implement-me!
	draw_message_box("THIS IS AFTER", "THE GAME OVER SCREEN");
}

void drawScreen(int visited[boardSizeX][boardSizeY])
{
	clear_screen();
	int x, y;
	int borderX = 3, borderY = 6;
	for(x = 0; x <= boardSizeX; x++)
	{
		for(y = 0; y <= boardSizeY; y++)
		{
			set_char(borderX + x, borderY + y, OPEN_FLOOR);
			if(x == 0 || x == boardSizeX ||
			   y == 0 || y == boardSizeY)
				set_char(borderX + x, borderY + y, HIDDEN_FLOOR);

			if(x == playerPosX && y == playerPosY)
				set_char(borderX + x, borderY + y, PLAYER);

			if(visited[x][y] == 0 &&
			   x > 0 && x < boardSizeX &&
			   y > 0 && y < boardSizeY)
				set_char(borderX + x, borderY + y, HIDDEN_FLOOR);

//			if(hole[x][y]==1 && showHoles==1)
//				set_char(borderX + x, borderY + y, HOLE);
//
//			if(x==goldPosX && y==goldPosY && showGold==1)
//				set_char(borderX + x, borderY + y, GOLD);

			if((x == wumpusPosX && y == wumpusPosY) &&
			   (showWumpus == 1 || visited[x][y] == 1))
				set_char(borderX + x, borderY + y, WUMPUS_AWAKEN); //TODO: animate AWAKEN / SLEEPING1 / SLEEPING2
		}
	}

	if(escaping==1)
		set_char(borderX + secretExitX, borderY + secretExitY, HIDDEN_FLOOR);
}
