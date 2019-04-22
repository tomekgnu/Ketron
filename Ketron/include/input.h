/*
 * input.h
 *
 * Created: 16.03.2019 21:38:39
 *  Author: Tomek
 */ 


#ifndef INPUT_H_
#define INPUT_H_

#include <main.h>

void readinputs(struct inputStates * data,uint8_t num_fam,uint8_t num_snd);
uint8_t compareinputs(inputStates * old_p, inputStates * new_p);
uint8_t getCurrentButton(inputStates* newdata);
#endif /* INPUT_H_ */