/*
 *  2048 for Uzebox 
 *  Version 1.0
 *  Copyright (C) 2013  Hartmut Wendt
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "kernel/uzebox.h"

#include "data/2048_tiles.pic.inc"
#include "data/fonts.pic.inc"
#include "data/patches.inc"
//#include "data/candle_spritetiles.pic.inc"
#include "data/spectrum_midi00.inc"

#include "engine.c"

  

//#define _FadingX
#define FONT_OFFSET	7


/* global definitons */
// program modes
enum {
	PM_Intro,			// program mode intro
	PM_Game,		
	PM_Game_Menu,
	PM_HoF_edit	
};






// 8-bit, 255 period LFSR (for generating pseudo-random numbers)
//#define PRNG_NEXT() (prng = ((u8)((prng>>1) | ((prng^(prng>>2)^(prng>>3)^(prng>>4))<<7))))
#define MAX(x,y) ((x)>(y) ? (x) : (y))




struct EepromBlockStruct ebs;



	

u8 prng; // Pseudo-random number generator

u8 program_mode;	// program mode (intro, 1 player mode, 2 player mode, ....



u8 PosY=120;
u8 PosX=112;
u8 X_pos_sprite=0;
u8 Y_pos_sprite=0;
u8 Match_speed = 1;
//int stop_watch_tmr;
	uint16_t board[SIZE][SIZE];
 

/*** function prototypes *****************************************************************/
void init(void);
void set_PM_mode(u8 mode);
void msg_window(u8 x1, u8 y1, u8 x2, u8 y2);

u8 GetTile(u8 x, u8 y);
void copy_buf(unsigned char *BUFA, unsigned char *BUFB, unsigned char ucANZ);
void fill_buf(u8 *BUFA, u8 content, u8 ucANZ);
void button_handler(u8 PosX, u8 PosY);
u8 set_def_highscore(void);
u8 check_highscore(void);
void copy_highsore(u8 entry_from, u8 entry_to);
void clear_highsore(u8 entry);
u8 view_highscore_entry(u8 x, u8 y, u8 entry, u8 load_data);
void edit_highscore_entry(u8 entry, u8 cursor_pos, u8 b_mode);
void show_highscore_char(u8 entry, u8 position, u8 cursor_on);
void drawBoard(uint16_t board[SIZE][SIZE]);


void init(void)
// init program
{

  // init music	
  InitMusicPlayer(patches);


  // init tile table
  SetTileTable(Tiles2048);
  // init font table
  SetFontTable(fonts);
     		
   // init random generator
  prng = 1;

  //Use block 28
  ebs.id = 28;
  if (!isEepromFormatted())
     return;

  if (EEPROM_ERROR_BLOCK_NOT_FOUND == EepromReadBlock(28,&ebs))
  {
	set_def_highscore();

  }	

  // load into screen
  set_PM_mode(PM_Intro);

}



int main(){
int ibuttons=0,ibuttons_old;
u8 uc1, uc2=0;
u8 ucCNT, ucIDLE_TMR;
bool success;

  // init program
  init();        
  
  // proceed program	
  while(1)
  {
    WaitVsync(1);	  
    // get controller state
    ibuttons_old = ibuttons;
	ibuttons = ReadJoypad(0);


    switch(program_mode)
	{
	  // proceed intro mode	
	  case PM_Intro:
	    prng++; 	    
		// display blinking text:
		if (uc1 >= 14) {
		  Print(9,15,PSTR("PRESS START"));		  
		  uc1 = 0;
	
		} else if (uc1==7) {
		  Print(9,15,PSTR("           "));

		}
		uc1++;
		// check start button
		if (BTN_START & ibuttons) {
          ucIDLE_TMR = 0;
		  set_PM_mode(PM_Game); 	
		}



 		WaitVsync(1);
		#ifdef _FadingX
	 	FadeOut(1, true);
		#endif
		//set_PM_mode(PM_Win8);
  		#ifdef _FadingX
    	FadeIn(1, true);
		#endif	
        break;

	  // proceed game
	  case PM_Game:
		WaitVsync(1);
	    prng++; 	    
		if ((ibuttons & BTN_A) && !(ibuttons_old & BTN_A)) {
			Print(3,25,PSTR("QUIT GAME? (A=NO, B=YES)"));
			program_mode = PM_Game_Menu;
			break;


		// UP button
		}else if ((ibuttons & BTN_UP) && !(ibuttons_old & BTN_UP)) {
			success = moveUp(board);

		// DOWN button
		}else if ((ibuttons & BTN_DOWN) && !(ibuttons_old & BTN_DOWN)) {
			success = moveDown(board);  

		// LEFT button
		}else if ((ibuttons & BTN_LEFT) && !(ibuttons_old & BTN_LEFT)) {
			success = moveLeft(board);			

		// RIGHT button
		}else if ((ibuttons & BTN_RIGHT) && !(ibuttons_old & BTN_RIGHT)) {
			success = moveRight(board);
 		}else success = false;

		if (success) {
			drawBoard(board);
			addRandom(board);
			drawBoard(board);
			if (gameEnded(board)) {
				Print(10,25,PSTR("GAME OVER!"));
				WaitVsync(100);
				if (check_highscore()) set_PM_mode(PM_HoF_edit);
				else set_PM_mode(PM_Intro);					
				break;
            }
		}        
		break;

 	  case PM_Game_Menu:
		if ((ibuttons & BTN_A) && !(ibuttons_old & BTN_A)) {
			Print(3,25,PSTR("                        "));
			program_mode = PM_Game;

		} else if ((ibuttons & BTN_B) && !(ibuttons_old & BTN_B)) {
			set_PM_mode(PM_Intro);
		}

        break;

	  case PM_HoF_edit:				
		// cursor blinking
		WaitVsync(1);
		uc1++;
		if (uc1 >= 10) uc1 = 0;
		// proceed cursor position with left & right button
		if ((ibuttons & BTN_RIGHT) && !(ibuttons_old & BTN_RIGHT)) {
		  if (PosX < 7) {       	   
		  	show_highscore_char(PosY - 1, PosX, 0); 		 
		    PosX++; 			
          }
		}
		if ((ibuttons & BTN_LEFT) && !(ibuttons_old & BTN_LEFT)) {		 
 		  if (PosX) {
		    show_highscore_char(PosY - 1, PosX, 0); 
		    PosX--; 
          }
		}
		// chose character up & down button
		if ((ibuttons & BTN_UP) && !(ibuttons_old & BTN_UP)) {
		  edit_highscore_entry(PosY,PosX,BTN_UP); 
		}
		else if ((ibuttons & BTN_DOWN) && !(ibuttons_old & BTN_DOWN)) {		 
 		  edit_highscore_entry(PosY,PosX,BTN_DOWN); 
		}     
		// show cursor
		show_highscore_char(PosY - 1, PosX, uc1 > 4);

		// store new entry
		if (ibuttons & BTN_A)   
		{
		  // store new highscore 
		  EepromWriteBlock(&ebs);
		  //if (Music_on) fade_out_volume();
		  set_PM_mode(PM_Intro);
		}
		break;




	}
	

  }
  

} 


void set_PM_mode(u8 mode) {
// set parameters, tiles, background etc for choosed program mode
//u8 uc1, uc2;

			
	switch (mode)
	{

	  case	PM_Intro:

		StopSong();		
   		 
	    // cursor is invisible now	    
		ClearVram();

		DrawMap(3,1,titel_2048);

		Print(26,7,PSTR("V1.0"));

		Print(8,9,PSTR("FOR E/UZEBOX"));
		Print(6,11,PSTR("WWW.HWHARDSOFT.DE"));

		
		Print(4,19,PSTR("---= TOP PLAYER =---"));
        view_highscore_entry(7,21,1,true);
        view_highscore_entry(7,23,2,true);
        view_highscore_entry(7,25,3,true);  
		
		break;
		

	  case	PM_Game_Menu:
		break;
	 

	  case	PM_Game:
 
		// blank screen - no music

		// StopSong();
        ClearVram();

		WaitVsync(2);

		// init variables
		memset(board,0,sizeof(board));
		score = 0;

		// init board
		addRandom(board);
	    addRandom(board);
		drawBoard(board);

		Print(5,2,PSTR("SCORE:"));

		// start music
		StartSong(midisong);
		break;

      case	PM_HoF_edit:
  		// init tile table
		//SetTileTable(IntroTiles);
  		// init font table
		//SetFontTilesIndex(INTROTILES_SIZE);
 
	    PosY = check_highscore();
	    if (PosY == 2) copy_highsore(1,2);
	    if (PosY == 1) {
		  copy_highsore(1,2);
		  copy_highsore(0,1);
        }
		clear_highsore(PosY - 1);
		// reset cursor to left position
		PosX = 0;
		
		// prepare screen	
	    ClearVram();
	    Print(8,3,PSTR("CONGRATULATION"));	
		Print(8,5,PSTR("NEW HIGHSCORE!"));	
		Print(8,22,PSTR("ENTER YOUR NAME"));	
		Print(6,24,PSTR("AND PRESS BUTTON A"));	

        view_highscore_entry(7,12,1,!(PosY));
        view_highscore_entry(7,14,2,!(PosY));
        view_highscore_entry(7,16,3,!(PosY));  
		break;	

	}



	program_mode = mode;

}


/**** G A M E P L A Y ********************************************************************/



void drawBoard(uint16_t board[SIZE][SIZE]) {
	unsigned char x,y;
	PrintInt(19,2,score,false);
	for (y=0;y<SIZE;y++) {
		for (x=0;x<SIZE;x++) {
			switch (board[x][y]) {

				case 0:
					DrawMap((x*5) + 5,(y*5) + 4,field_empty);
					break;

				case 2:
					DrawMap((x*5) + 5,(y*5) + 4,field_2);
					break;

				case 4:
					DrawMap((x*5) + 5,(y*5) + 4,field_4);
					break;

				case 8:
					DrawMap((x*5) + 5,(y*5) + 4,field_8);
					break;

				case 16:
					DrawMap((x*5) + 5,(y*5) + 4,field_16);
					break;

				case 32:
					DrawMap((x*5) + 5,(y*5) + 4,field_32);
					break;

				case 64:
					DrawMap((x*5) + 5,(y*5) + 4,field_64);
					break;

				case 128:
					DrawMap((x*5) + 5,(y*5) + 4,field_128);
					break;

				case 256:
					DrawMap((x*5) + 5,(y*5) + 4,field_256);
					break;

				case 512:
					DrawMap((x*5) + 5,(y*5) + 4,field_512);
					break;
			
				case 1024:
					DrawMap((x*5) + 5,(y*5) + 4,field_1024);
					break;
			
				case 2048:
					DrawMap((x*5) + 5,(y*5) + 4,field_2048);
					break;
			
			
			}	
		    
		}
	}
}


/**** A N I M A T I O N S ***************************************************************/








/**** S T U F F ********************************************************************/

u8 GetTile(u8 x, u8 y)
// get background tile from vram
{

 return (vram[(y * 30) + x] - RAM_TILES_COUNT);

}



u8 check_highscore(void) {
// check the actual highsore
u8 a;
int i1;
   // read the eeprom block
  if (!isEepromFormatted() || EepromReadBlock(28, &ebs))
        return(0);   
  for(a=0; a<3; a++) {
    i1 = (ebs.data[(a * 10)+8] * 256) + ebs.data[(a * 10)+9];
    if (score > i1) return(a + 1);

  }

  // score is lower as saved highscores 
  return(0);
}



void copy_highsore(u8 entry_from, u8 entry_to) {
// copy a highscore entry to another slot
u8 a;
   // read the eeprom block
  for(a=0; a<10; a++) {
    ebs.data[(entry_to * 10) + a] = ebs.data[(entry_from * 10) + a];
  } 
}


void clear_highsore(u8 entry) {
// clear the name in actual entry and set the score to highscore
u8 a;
  // clear name 
  for(a=0; a<8; a++) {
    ebs.data[(entry * 10) + a] = 0x20;
  }   
  // set score
  ebs.data[(entry * 10) + 8] = score / 256;
  ebs.data[(entry * 10) + 9] = score % 256;
}



u8 set_def_highscore(void) {
// write the default highscore list in the EEPROM
  // entry 1
  ebs.data[0] = 'U';
  ebs.data[1] = 'Z';
  ebs.data[2] = 'E';
  ebs.data[3] = ' ';
  ebs.data[4] = ' ';
  ebs.data[5] = ' ';
  ebs.data[6] = ' ';
  ebs.data[7] = ' ';
  ebs.data[8] = 0x01;
  ebs.data[9] = 0xF4;
  // entry 2
  ebs.data[10] = 'H';
  ebs.data[11] = 'A';
  ebs.data[12] = 'R';
  ebs.data[13] = 'T';
  ebs.data[14] = 'M';
  ebs.data[15] = 'U';
  ebs.data[16] = 'T';
  ebs.data[17] = ' ';
  ebs.data[18] = 0x01;
  ebs.data[19] = 0x90;
  // entry 3
  ebs.data[20] = 'C';
  ebs.data[21] = 'A';
  ebs.data[22] = 'R';
  ebs.data[23] = 'S';
  ebs.data[24] = 'T';
  ebs.data[25] = 'E';
  ebs.data[26] = 'N';
  ebs.data[27] = ' ';
  ebs.data[28] = 0x01;
  ebs.data[29] = 0x2C;
  return(EepromWriteBlock(&ebs));
}


u8 view_highscore_entry(u8 x, u8 y, u8 entry, u8 load_data) {
// shows an entry of the higscore
u8 a,c;

  // read the eeprom block
  if (load_data)
  {
    if (!isEepromFormatted() || EepromReadBlock(28, &ebs))
        return(1);   
  }
  entry--;

  PrintInt(x + 13, y,(ebs.data[(entry * 10)+8] * 256) + ebs.data[(entry * 10)+9],false);
  for(a = 0; a < 8;a++) {
	c = ebs.data[a + (entry * 10)];  
	PrintChar(x + a, y, c);  
  }
  return(0);
}



void edit_highscore_entry(u8 entry, u8 cursor_pos, u8 b_mode) {
// edit and view and char in the name of choosed entry    
entry--;
u8 c = ebs.data[(entry * 10) + cursor_pos];
  // proceed up & down button
  if (b_mode == BTN_UP) {
     c++;
     if (c > 'Z') c = ' '; 
     else if (c == '!') c = 'A';
  }
  if (b_mode == BTN_DOWN) {		 
     c--;      
     if (c == 0x1F) c = 'Z';
	 else if (c < 'A') c = ' ';
  }
  ebs.data[(entry * 10) + cursor_pos] = c;

}


void show_highscore_char(u8 entry, u8 position, u8 cursor_on) {
// shows a char of edited name
u8 c = ebs.data[(entry * 10) + position];
    if (cursor_on) PrintChar(7 + position, (entry * 2) + 12, '_');   // show '_'
    else if (c == ' ') PrintChar(7 + position, (entry * 2) + 12, ' ');	// space
    else PrintChar(7 + position, (entry * 2) + 12, c); 	
}




/**
copy a buffer into another buffer 
@param source buffer
@param target buffer
@param count of copied bytes
@return none
*/
void copy_buf(unsigned char *BUFA, unsigned char *BUFB, unsigned char ucANZ)
{
 for(;ucANZ>0 ; ucANZ--)
 {
  *(BUFB++) = *(BUFA++);
 }   
}


/**
fill a buffer 
@param target buffer
@param byte to fill
@param count of copied bytes
@return none
*/
void fill_buf(u8 *BUFA, u8 content, u8 ucANZ)
{
 for(;ucANZ>0 ; ucANZ--)
 {
  *(BUFA++) = content;
 }   
}



