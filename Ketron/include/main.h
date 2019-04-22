/*
 * main.h
 *
 * Created: 16.03.2019 21:57:38
 *  Author: Tomek
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>

typedef struct {
	uint8_t family[2];
	uint8_t familyIndex[2];
}Preset;

typedef struct inputStates
{
	uint8_t button[4];
	uint8_t pot[2];
}inputStates;

// Number of presets
#define NUM_OF_PRESETS		6
#define PREV_PRESET(x)		(x > 0?(x - 1):0)
#define NEXT_PRESET(x)		(x < (NUM_OF_PRESETS - 1)?(x + 1):x)

// Keypress timeout	(multiply by 500ms)
#define TWO_SECONDS		4
// TFT pin definitions
#define TFT_CS			10
#define TFT_DC			9
#define TFT_RST			8

// left-right constants
#define LEFT				0
#define RIGHT				1
#define BOTH				2
// input names
#define BUTTON0				0
#define BUTTON1				1
#define BUTTON2				2
#define BUTTON3				5
#define POT0				3
#define POT1				4
#define NONE				255


// modes for button 0 and 3
#define NONE_SELECT				0
#define FAMILY_SELECT_RIGHT		1
#define SOUND_SELECT_RIGHT		2
#define FAMILY_SELECT_LEFT		3
#define SOUND_SELECT_LEFT		4
#define PRESET_SELECT			5
#define SOUND_SELECT			6
// bank numbers
#define NUM_OF_BANKS		5
#define BANKA				0
#define BANKB				1
#define BANKC				10
#define BANKD				11
#define BANKE				4

// sound family constants
#define NUM_OF_FAMILIES		13
#define PIANO				0
#define CHROMATIC			1
#define ORGAN				2
#define ACCORDION			3
#define GUITAR				4
#define STRINGS_CHOIR		5
#define BRASS				6
#define SAX_FLUTE			7
#define PAD					8
#define SYNTH				9
#define ETHNIC				10
#define BASS_SFX			11
#define DRUMS				12

void processEvents();
void loadPreset(uint8_t preset,uint8_t *family,uint8_t *famIndex,uint8_t *bank,uint8_t *prog,bool *sp,uint8_t *note);

#endif /* MAIN_H_ */