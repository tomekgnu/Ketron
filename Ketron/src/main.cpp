/*
 * Ketron.cpp
 *
 * Created: 16.03.2019 09:21:01
 * Author : Tomek
 */ 

#include <util/delay.h>
#include <main.h>
#include <input.h>
#include <output.h>
#include <MIDI.h>
#include <TFT.h>
#include <SPI.h>
#include <OSFS.h>

uint8_t PIN_BTN0 = 2;
uint8_t PIN_BTN1 = 3;
uint8_t PIN_BTN2 = 4;
uint8_t PIN_BTN3 = PIN_A5;
uint8_t PIN_POT0 = 0;
uint8_t PIN_POT1 = 1;
uint8_t PIN_LED_GRN = 6;
uint8_t PIN_LED_RED = 7;
uint8_t bankNumbers[5] = {0,1,10,11,67};
inputStates lastdata;
inputStates newdata;
byte data1;
byte data2;
byte channel;


// how many sounds there are in each family
uint8_t family_sounds[NUM_OF_FAMILIES] = {35,25,35,18,78,64,36,48,52,35,39,57,47};
TFT tft = TFT(TFT_CS, TFT_DC, TFT_RST);
//DataFlash dataflash;
MIDI_CREATE_DEFAULT_INSTANCE();


int main(void)
{
    init();		// initialize Arduino wiring
    setup();	// local setup	
	
	if(OSFS::checkLibVersion() == OSFS::result::UNFORMATTED){
		OSFS::format();
		tft.text("Not formatted",0,0);
		while(1)
			continue;
	}
	showCurrentMode(NONE_SELECT,RIGHT,false);	
	showFamilyName(0,RIGHT,NONE_SELECT);
	showInstrumentName(0,0,RIGHT,NONE_SELECT);	
	
	MIDI.sendControlChange(midi::BankSelect,0,1);
	MIDI.sendProgramChange(0,1);
    while (1) 
    {
		processEvents();
    }
	
}


void setup()
{
	SPI.begin();	
	
	pinMode(PIN_BTN0, INPUT_PULLUP);
	pinMode(PIN_BTN1, INPUT_PULLUP);
	pinMode(PIN_BTN2, INPUT_PULLUP);
	pinMode(PIN_BTN3,INPUT_PULLUP);
	pinMode(PIN_LED_GRN, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);
		
	MIDI.begin(MIDI_CHANNEL_OMNI);
	MIDI.turnThruOff();
	
	tft.begin();
	tft.fillScreen(ST7735_MAGENTA);
	tft.stroke(0,0,0);
	tft.textSize(2);	
}

void getSplitNote(byte &splitNote, bool &split)
{
	while(digitalRead(PIN_BTN1) == HIGH){
	if(MIDI.read() && MIDI.getType() == midi::NoteOn){
			splitNote = MIDI.getData1();							
			split = true;
			break;
		}
	}	
}

void loadSounds(uint8_t * currentBank, uint8_t * currentProgram,uint8_t hand,byte channel)
{
	MIDI.sendControlChange(midi::BankSelect,currentBank[hand],channel);
	MIDI.sendProgramChange(currentProgram[hand],channel);
}

void processEvents()
{
	static byte splitNote = 0;
	static uint8_t currentFamily[2] = {0,0};
	static uint8_t currentBank[2] = {0,0};
	static uint8_t currentProgram[2] = {0,0};
	static uint8_t currentFamilyIndex[2] = {0,0};
	uint16_t delayCount = 0;	
	uint8_t currentInput;
	static uint8_t currentMode = NONE_SELECT;
	static uint8_t currentHand = RIGHT;
	static bool split = false;
	static bool midiThru = false;
	static uint8_t currentPreset = 0;
	
	readinputs(&newdata,NUM_OF_FAMILIES,family_sounds[currentFamily[currentHand]]);
	currentInput = compareinputs(&lastdata,&newdata);
	
	if(currentInput != NONE){
				
		switch(currentInput){
			case POT1: 
					if(currentMode == FAMILY_SELECT_RIGHT)
						currentFamily[RIGHT] = newdata.pot[1];
					else if(currentMode == FAMILY_SELECT_LEFT)
						currentFamily[LEFT] = newdata.pot[1];
					break;
			case POT0:  
					if(currentMode == SOUND_SELECT_RIGHT)
						currentFamilyIndex[RIGHT] = newdata.pot[0];
					else if(currentMode == SOUND_SELECT_LEFT)
						currentFamilyIndex[LEFT] = newdata.pot[0];
					break;
			case BUTTON0:
					if(currentMode == PRESET_SELECT){
						currentPreset = PREV_PRESET(currentPreset);
						loadPreset(currentPreset,currentFamily,currentFamilyIndex,currentBank,currentProgram,&split,&splitNote);						
						showPresetList(currentPreset,BUTTON0);						
						break;
					}
					currentMode = getNextMode(currentMode,split);
					showScreenPart(currentFamily,currentProgram,currentBank,currentMode,RIGHT,midiThru);
					if(split == true)
						showScreenPart(currentFamily,currentProgram,currentBank,currentMode,LEFT,midiThru);							
					break;				
			case BUTTON1:
					tft.background(ST7735_MAGENTA);
					if(currentMode == PRESET_SELECT){
						currentMode = NONE_SELECT;
						showScreenPart(currentFamily,currentProgram,currentBank,currentMode,RIGHT,midiThru);
						if(split == true)
							showScreenPart(currentFamily,currentProgram,currentBank,currentMode,LEFT,midiThru);
						break;
					}
					split = false;
					splitNote = 0;					
					tft.text("Press note:",0,20);
					_delay_ms(500);
					getSplitNote(splitNote, split);
					currentMode = NONE_SELECT;				
					showScreenPart(currentFamily,currentProgram,currentBank,currentMode,RIGHT,midiThru);
					if(split == true)
						showScreenPart(currentFamily,currentProgram,currentBank,currentMode,LEFT,midiThru);					
					break;
			case BUTTON2:
					if(currentMode == PRESET_SELECT){
						currentPreset = NEXT_PRESET(currentPreset);
						loadPreset(currentPreset,currentFamily,currentFamilyIndex,currentBank,currentProgram,&split,&splitNote);						
						showPresetList(currentPreset,BUTTON2);
						break;
					}
					if(midiThru == false){
						MIDI.sendControlChange(midi::BankSelect,0,1);
						MIDI.turnThruOn();
						midiThru = true;																
					}
					else{
						MIDI.turnThruOff();
						midiThru = false;						
						loadSounds(currentBank,currentProgram,RIGHT,1);
						if(split == true)
							loadSounds(currentBank, currentProgram,LEFT,2);	
										
					}	
					showCurrentMode(currentMode,RIGHT,midiThru);					
					break;
			case BUTTON3:
					tft.background(ST7735_MAGENTA);
					for(delayCount = 0; delayCount < 4; delayCount++){
						if(digitalRead(PIN_BTN3) == HIGH)
							break;
						_delay_ms(500);
					}
					
					if(delayCount == TWO_SECONDS){											
						if(savePreset(currentPreset,currentFamily,currentFamilyIndex,split,splitNote) == true)
							tft.text("Saved",0,0);
						else
							tft.text("Save failed",0,0);						
					}
					else{
						tft.background(ST7735_MAGENTA);
						currentMode = PRESET_SELECT;
						loadPreset(currentPreset,currentFamily,currentFamilyIndex,currentBank,currentProgram,&split,&splitNote);						
						showPresetList(currentPreset,NONE);
					}
					break;
				
			}	// end of switch			
		
		
		if(midiThru == false && (currentInput == POT0 || currentInput == POT1)){			
			getBankProgram(currentFamily,currentFamilyIndex,currentBank,currentProgram);
			switch(currentMode){
				case FAMILY_SELECT_RIGHT:	showFamilyName(currentFamily,RIGHT,currentMode); 
											showInstrumentName(currentBank,currentProgram,RIGHT,currentMode);
											loadSounds(currentBank, currentProgram,RIGHT,1);
											break;
				case SOUND_SELECT_RIGHT:	showInstrumentName(currentBank,currentProgram,RIGHT,currentMode);
											loadSounds(currentBank, currentProgram,RIGHT,1);										 
											break;
				case FAMILY_SELECT_LEFT:	showFamilyName(currentFamily,LEFT,currentMode);
											showInstrumentName(currentBank,currentProgram,LEFT,currentMode);
											loadSounds(currentBank, currentProgram,LEFT,2);
											break;
				case SOUND_SELECT_LEFT:		showInstrumentName(currentBank,currentProgram,LEFT,currentMode);
											loadSounds(currentBank, currentProgram,LEFT,2);
											break;
				
			}			
						
		}
		
	}	// end of input		
	
			
	if(MIDI.read()){
		if(midiThru == true)
			return;
		data1 = MIDI.getData1();
		data2 = MIDI.getData2();
		channel = MIDI.getChannel();
		switch(MIDI.getType()){
			case midi::ControlChange: 
				MIDI.sendControlChange(data1,data2,1);
				if(split == true)
					MIDI.sendControlChange(data1,data2,2);
				break;
			case midi::NoteOn: 			
				MIDI.sendNoteOn(data1, data2,OUT_CHANNEL);
				break;
			case midi::NoteOff: 
				MIDI.sendNoteOff(data1, data2,OUT_CHANNEL);			
				break;
			
		}
	
	}
	updateLEDs(&newdata);
	
}

void loadPreset(uint8_t preset,uint8_t *family,uint8_t *famIndex,uint8_t *bank,uint8_t *prog,bool *sp,uint8_t *note){
	if(readPreset(preset,family,famIndex,sp,note) == true){		
		getBankProgram(family,famIndex,bank,prog);
		loadSounds(bank,prog,RIGHT,1);
		if((*sp) == true)
			loadSounds(bank,prog,LEFT,2);	
	}	
}