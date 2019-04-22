/*
 * output.h
 *
 * Created: 16.03.2019 21:49:30
 *  Author: Tomek
 */ 


#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <main.h>
#include <midi_Defs.h>
void updateLEDs(inputStates * data);
void updateLCD(inputStates * data);
void showFamilyName(uint8_t * ind,uint8_t hand,uint8_t currentMode);
void showInstrumentName(uint8_t *bank,uint8_t *prog,uint8_t hand,uint8_t currentMode);
void showCurrentMode(uint8_t currentMode,uint8_t hand);
void getBankProgram(uint8_t *family,uint8_t *index,uint8_t *currentBank, uint8_t *currentProgram);
void showSplitPoint(byte data);
void showScreenPart(uint8_t * family,uint8_t * program,uint8_t * bank,uint8_t mod,uint8_t hand);
uint8_t getNextMode(uint8_t mod,uint8_t split = false);
bool savePreset(uint8_t number,uint8_t * family,uint8_t *familyIndex,bool split,uint8_t splitNote);
bool readPreset(uint8_t number,uint8_t * family,uint8_t *familyIndex,bool *split,byte *splitNote);
void showPresetList(uint8_t selected,uint8_t but);
#endif /* OUTPUT_H_ */