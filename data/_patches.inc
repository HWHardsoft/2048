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

//External PCM samples
#include "tambourine.inc"


//INST: Synth Piano
const char patch00[] PROGMEM ={ 
0,PC_WAVE,9,
0,PC_ENV_SPEED,-15,  
3,PC_ENV_SPEED,-5,  
0,PATCH_END
};

//Square Piano ch3
const char patch01[] PROGMEM ={	
0,PC_WAVE,9,
0,PC_ENV_SPEED,-15,  
2,PC_ENV_SPEED,-3,  
0,PC_NOTE_HOLD,0,
3,PC_ENV_SPEED,-8,  
1,PATCH_END
};

const char flute[] PROGMEM ={//Megatris
0,PC_WAVE,8,
0,PC_ENV_VOL,100, 
1,PC_ENV_SPEED,20,
5,PC_ENV_SPEED,-9, 
0,PATCH_END
};

const char organ[] PROGMEM ={//Megatris
0,PC_WAVE,8,
1,PC_ENV_VOL,200, 
1,PC_NOTE_HOLD,0,
1,PC_ENV_SPEED,-25,
0,PATCH_END
};

const char base[] PROGMEM ={	
0,PC_WAVE,1,
0,PC_ENV_SPEED,-15,  
2,PC_ENV_SPEED,-3,  
0,PC_NOTE_HOLD,0,
3,PC_ENV_SPEED,-8,  
1,PATCH_END
};

const char base_drum[] PROGMEM ={	
0,PC_ENV_VOL,180,
0,PC_NOISE_PARAMS,(2<<1),
0,PC_ENV_SPEED,-50, 
1,PC_NOISE_PARAMS,(3<<1),
1,PC_NOISE_PARAMS,(4<<1),
1,PC_NOISE_PARAMS,(5<<1),
1,PC_NOISE_PARAMS,(6<<1),
0,PATCH_END
};


const char hi_hat[] PROGMEM ={
0,PC_NOISE_PARAMS,1,
0,PC_ENV_VOL,0x90,
0,PC_ENV_SPEED,-30, 
8,PC_NOTE_CUT,0,
0,PATCH_END
};

const char crash_cymbal[] PROGMEM ={
0,PC_NOISE_PARAMS,0,
0,PC_ENV_VOL,0xa0,
0,PC_ENV_SPEED,-20, 
1,PC_ENV_SPEED,-5,
0,PC_NOISE_PARAMS,1,
0,PATCH_END
};

const char snare[] PROGMEM ={
0,PC_ENV_SPEED,-19,
0,PC_NOISE_PARAMS,6,
2,PC_NOISE_PARAMS,0x1,
0,PATCH_END 
};

const char clap[] PROGMEM ={
0,PC_ENV_SPEED,-20,
0,PC_NOISE_PARAMS,2,
1,PC_ENV_VOL,0,
2,PC_ENV_VOL,0xff,
0,PC_NOISE_PARAMS,3, 
0,PATCH_END  
};


const struct PatchStruct patches[] PROGMEM = {
{0,NULL,patch00,0,0},
{0,NULL,patch01,0,0},
{0,NULL,flute},
{0,NULL,organ},
{0,NULL,base},
{0,NULL,organ},
{2,tambourine,NULL,sizeof_tambourine,sizeof_tambourine},
{1,NULL,base_drum,0,0},
{1,NULL,hi_hat,0,0},
{1,NULL,crash_cymbal,0,0},
{1,NULL,snare,0,0},
{1,NULL,clap,0,0}
};
