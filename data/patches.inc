/*
 *  Uzebox Default Patches
 *  Copyright (C) 2008  Alec Bourque
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

/*
	Patches are made of a command stream made of 3 bytes per command:
		1=delta time
		2=command
		3=command parameter

	Patches must start with 1 byte describing the sound type:
		0=wave channel (i.e.; channel 0,1 and 2)
		1=noise channel (channel 3)
		2=PCM (channel 3)
		->For type=2 two more byte follows in order: sample adress low byte, sample adress hi byte

	It must end with <0,PATCH_END> and this command takes
	only two bytes (no parameter).
*/

//Square Piano
const char patch00[] PROGMEM ={	
0,PC_WAVE,9,
0,PC_ENV_SPEED,-15,  
4,PC_ENV_SPEED,-7,  
0,PATCH_END
};

//Square Piano 2
const char patch01[] PROGMEM ={	
0,PC_WAVE,4,
0,PC_ENV_SPEED,-15,  
4,PC_ENV_SPEED,-7,  
0,PATCH_END
};


//INST: Synth Piano
const char patch02[] PROGMEM ={ 
0,PC_WAVE,6,
0,PC_ENV_SPEED,-5,
0,PATCH_END
};

//INST: organ
const char patch04[] PROGMEM ={	
0,PC_WAVE,8,
1,PC_ENV_VOL,200, 
1,PC_NOTE_HOLD,0,
1,PC_ENV_SPEED,-25,
0,PATCH_END
};

 //INST: hi-hat              Channel 4...
  const char patch03[] PROGMEM ={	
  0,PC_NOISE_PARAMS,1,
  0,PC_ENV_VOL,0x90,
  0,PC_ENV_SPEED,-30, 
  8,PC_NOTE_CUT,0,
  0,PATCH_END
  }; 


//Clap
const char patchx3[] PROGMEM ={
0,PC_ENV_SPEED,-20,
0,PC_NOISE_PARAMS,2,
1,PC_ENV_VOL,0,
2,PC_ENV_VOL,0xff,
0,PC_NOISE_PARAMS,3, 
0,PATCH_END  
};


//FX:  'broken egg'
const char broken_egg[] PROGMEM = {
0,PC_ENV_SPEED,-5,
0,PC_PITCH,60,
2,PC_NOTE_UP,6,  
2,PC_NOTE_UP,6, 
2,PC_NOTE_UP,6,  
4,PC_NOTE_CUT,0,
0,PATCH_END
};

//FX: 'catch object'
const char catch_object[] PROGMEM ={ 
0,PC_WAVE,8,
0,PC_ENV_SPEED,-12,
5,PC_NOTE_UP,12, 
5,PC_NOTE_DOWN,12,
5,PC_NOTE_UP,12, 
5,PC_NOTE_DOWN,12,
5,PC_NOTE_CUT,0,
0,PATCH_END
};

//FX: 'beep'
const char beep[] PROGMEM ={	
0,PC_ENV_SPEED,-25,
2,PC_NOTE_UP,12,  
4,PC_NOTE_CUT,0,								 
0,PATCH_END
};


//FX: 'catch grenade'
const char catch_grenade[] PROGMEM ={	
0,PC_WAVE,4,
0,PC_ENV_SPEED,-5,

0,PC_PITCH,62,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 


1,PC_NOTE_DOWN,10,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 


1,PC_NOTE_DOWN,2,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 


1,PC_NOTE_DOWN,10,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 

1,PC_NOTE_DOWN,5,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 

1,PC_NOTE_DOWN,5,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 

1,PC_NOTE_DOWN,5,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,2, 
1,PC_NOTE_CUT,0,

0,PATCH_END 
};

//FX:  'catch green egg'
const char catch_green_egg[] PROGMEM ={	
0,PC_WAVE,3,
0,PC_PITCH,86,
2,PC_NOTE_DOWN,10, 
2,PC_NOTE_DOWN,8, 
2,PC_NOTE_DOWN,6, 
2,PC_NOTE_DOWN,4, 
2,PC_NOTE_DOWN,2, 
 
0,PC_PITCH,66, 
2,PC_NOTE_UP,5, 
2,PC_NOTE_DOWN,2, 
2,PC_NOTE_UP,5, 

2,PC_NOTE_UP,5, 
2,PC_NOTE_DOWN,2, 
2,PC_NOTE_UP,5, 

0,PC_ENV_SPEED,-8,  

4,PC_NOTE_UP,5, 
4,PC_NOTE_DOWN,5, 
4,PC_NOTE_UP,5, 
4,PC_NOTE_DOWN,5, 
4,PC_NOTE_UP,5,
4,PC_NOTE_DOWN,5, 
4,PC_NOTE_UP,5,
1,PC_NOTE_CUT,0,
0,PATCH_END 

};

//FX: 'catch carrot'
const char catch_carrot[] PROGMEM ={	
0,PC_ENV_VOL,0,

4,PC_WAVE,4,
0,PC_ENV_VOL,0xff,
0,PC_TREMOLO_LEVEL,0x90,     
0,PC_TREMOLO_RATE,60, 
0,PC_ENV_SPEED,-5,
1,PC_PITCH,80,
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_DOWN,5, 
3,PC_NOTE_UP,5, 
3,PC_NOTE_CUT,0,
0,PATCH_END 
};


//empty 
const char empty[] PROGMEM ={
0,PATCH_END
};



//FX:  'game over'
const char gameover[] PROGMEM ={	
0,PC_WAVE,4,
0,PC_ENV_SPEED,-5,

0,PC_PITCH,52,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,4, 
1,PC_NOTE_UP,6, 

1,PC_NOTE_DOWN,10,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,4, 
1,PC_NOTE_UP,6, 


1,PC_NOTE_DOWN,10,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,4, 
1,PC_NOTE_UP,6, 

1,PC_NOTE_DOWN,5,
1,PC_NOTE_UP,2, 
1,PC_NOTE_UP,4, 
1,PC_NOTE_UP,6, 
 
2,PC_NOTE_UP,2, 
2,PC_NOTE_UP,4, 
2,PC_NOTE_UP,6, 

3,PC_NOTE_DOWN,5,
3,PC_NOTE_UP,2, 
3,PC_NOTE_UP,4, 
3,PC_NOTE_UP,6, 
 
4,PC_NOTE_DOWN,5,
4,PC_NOTE_UP,2, 
4,PC_NOTE_UP,4, 
4,PC_NOTE_UP,6, 
1,PC_NOTE_CUT,0,
0,PATCH_END 
};

/*
const struct PatchStruct patches[] PROGMEM = {
{0,NULL,patch00,0,0},
{0,NULL,patch01,0,0},
{0,NULL,patch02,0,0},
{0,NULL,patch01,0,0},
{0,NULL,beep,0,0},
{0,NULL,catch_object,0,0},
{0,NULL,broken_egg,0,0},
{0,NULL,catch_green_egg,0,0},
{0,NULL,gameover,0,0},
};*/


const struct PatchStruct patches[] PROGMEM = {
{0,NULL,patch02,0,0},
{0,NULL,patch02,0,0},
{0,NULL,patch02,0,0},
{0,NULL,patch00,0,0},

};
