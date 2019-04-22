/*
 * CPPFile1.cpp
 *
 * Created: 16.03.2019 21:34:00
 *  Author: Tomek
 */ 

#include <main.h>
#include <avr/io.h>
#include <Arduino.h>
#include <output.h>

extern uint8_t PIN_BTN0;
extern uint8_t PIN_BTN1;
extern uint8_t PIN_BTN2;
extern uint8_t PIN_BTN3;
extern uint8_t PIN_POT0;
extern uint8_t PIN_POT1;
extern uint8_t PIN_LED_GRN;
extern uint8_t PIN_LED_RED;
extern uint8_t bankNumbers[5];
extern inputStates lastdata;
extern inputStates newdata;
extern inputStates newdata;

void readinputs(struct inputStates * data,uint8_t num_fam,uint8_t num_snd)
{
	uint8_t index = 0;
	uint8_t val = 0;
	uint8_t step1 = (255 / num_fam);
	uint8_t step0 = (255 / num_snd);
	data->button[0] = (digitalRead(PIN_BTN0) == LOW);
	data->button[1] = (digitalRead(PIN_BTN1) == LOW);
	data->button[2] = (digitalRead(PIN_BTN2) == LOW);
	data->button[3] = (digitalRead(PIN_BTN3) == LOW);
	
	for(index = 0; index < (num_fam * step1); index += step1){
		val = analogRead(PIN_POT1) >> 2;
		if(val >= index && val < (index + step1)){
			data->pot[1] = index / step1;
			break;
		}
	}
	for(index = 0; index < (num_snd * step0); index += step0){
		val = analogRead(PIN_POT0) >> 2;
		if(val >= index && val < (index + step0)){
			data->pot[0] = index / step0;
			break;
		}
	}
	
}

uint8_t compareinputs(inputStates * old_p, inputStates * new_p)
{
		
	if(old_p->button[0] != new_p->button[0]){
		old_p->button[0] = new_p->button[0];
		if(new_p->button[0] == HIGH)
			return BUTTON0;
	}
	if(old_p->button[1] != new_p->button[1]){
		old_p->button[1] = new_p->button[1];
		if(new_p->button[1] == HIGH)
			return BUTTON1;
	}		
	if(old_p->button[2] != new_p->button[2]){
		old_p->button[2] = new_p->button[2];
		if(new_p->button[2] == HIGH)
			return BUTTON2;
	}
	if(old_p->button[3] != new_p->button[3]){
		old_p->button[3] = new_p->button[3];
		if(new_p->button[3] == HIGH)
		return BUTTON3;
	}	
	if(old_p->pot[0] != new_p->pot[0])
	{
		old_p->pot[0] = new_p->pot[0];
		return POT0;		
	}
	if(old_p->pot[1] != new_p->pot[1])
	{
		old_p->pot[1] = new_p->pot[1];
		return POT1;
	}
	
	return NONE;
	
}


uint8_t getCurrentButton(inputStates* newdata){
	if(newdata->button[0] == HIGH)
		return BUTTON0;
	if(newdata->button[1] == HIGH)
		return BUTTON1;
	if(newdata->button[2] == HIGH)
		return BUTTON2;
	
	return NONE;
}