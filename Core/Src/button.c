/*
 * button.c
 *
 *  Created on: Apr 13, 2022
 *      Author: Rafal Mulinski
 */

#include "main.h"
#include "button.h"

//init button
void ButtonEnterInitKey(buttonenter_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce){
	Key -> State = IDLE;
	Key ->  GpioPort = GpioPort;
	Key ->GpioPin = GpioPin;
	Key -> TimerDebounce = TimerDebounce;

}
void ButtonBackInitKey(buttonback_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce){
	Key -> State = IDLE;
	Key ->  GpioPort = GpioPort;
	Key ->GpioPin = GpioPin;
	Key -> TimerDebounce = TimerDebounce;

}
void ButtonPrevInitKey(buttonprev_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce){
	Key -> State = IDLE;
	Key ->  GpioPort = GpioPort;
	Key ->GpioPin = GpioPin;
	Key -> TimerDebounce = TimerDebounce;

}void ButtonNextInitKey(buttonnext_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce){
	Key -> State = IDLE;
	Key ->  GpioPort = GpioPort;
	Key ->GpioPin = GpioPin;
	Key -> TimerDebounce = TimerDebounce;

}

//function time setting

//Register callbacks
void ButtonBackPressCallback(buttonback_t* Key, void *Callback){
	Key->key_back_press = Callback;

}
void ButtonNextPressCallback(buttonnext_t* Key, void *Callback){
	Key->key_next_press = Callback;

}
void ButtonPrevPressCallback(buttonprev_t* Key, void *Callback){
	Key->key_prev_press = Callback;

}
void ButtonEnterPressCallback(buttonenter_t* Key, void *Callback){
	Key->key_enter_press = Callback;

}
//states for next button
void ButtonNextIdleRoutine(buttonnext_t* Key){
if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin)){
	Key->State = DEBOUNCE;
	Key ->LastTick = HAL_GetTick();
}
}
void ButtonNextPressedRoutine(buttonnext_t* Key){
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin)){
		Key->State = IDLE;
	}
}
void ButtonNextDebounceRoutine(buttonnext_t* Key)
{
	if ((HAL_GetTick() - Key ->LastTick) > Key ->TimerDebounce){
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin))
		{
			Key-> State = PRESSED;
			//if (Key->key_next_press != NULL)
			//{
				Key->key_next_press();
			//}

		}
		else {
			Key->State = IDLE;
		}
	}
}
//states Machine for Next Button
void ButtonNextTask(buttonnext_t* Key){

	switch(Key-> State){
	case IDLE:
		ButtonNextIdleRoutine( Key);
		break;
	case DEBOUNCE:
		ButtonNextDebounceRoutine( Key);
		break;
	case PRESSED:
		ButtonNextPressedRoutine( Key);
		break;

	}
}

//states for Enter button
void ButtonEnterIdleRoutine(buttonenter_t *Key) {
	if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}
void ButtonEnterPressedRoutine(buttonenter_t *Key) {
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
		Key->State = IDLE;
	}
}
void ButtonEnterDebounceRoutine(buttonenter_t *Key) {
	if ((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce) {
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
			Key->State = PRESSED;
			//if (Key->key_enter_press != NULL) {
				Key->key_enter_press();
			//}

		} else {
			Key->State = IDLE;
		}
	}
}
//states Machine for Enter Button
void ButtonEnterTask(buttonenter_t *Key) {

	switch (Key->State) {
	case IDLE:
		ButtonEnterIdleRoutine(Key);
		break;
	case DEBOUNCE:
		ButtonEnterDebounceRoutine(Key);
		break;
	case PRESSED:
		ButtonEnterPressedRoutine(Key);
		break;

	}
}

//states for Back button
void ButtonBackIdleRoutine(buttonback_t *Key) {
	if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}
void ButtonBackPressedRoutine(buttonback_t *Key) {
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
		Key->State = IDLE;
	}
}
void ButtonBackDebounceRoutine(buttonback_t *Key) {
	if ((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce) {
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
			Key->State = PRESSED;
			//if (Key->key_back_press != NULL) {
				Key->key_back_press();
			//}

		} else {
			Key->State = IDLE;
		}
	}
}
//states Machine for Back Button
void ButtonBackTask(buttonback_t* Key){

	switch(Key-> State){
	case IDLE:
		ButtonBackIdleRoutine( Key);
		break;
	case DEBOUNCE:
		ButtonBackDebounceRoutine( Key);
		break;
	case PRESSED:
		ButtonBackPressedRoutine( Key);
		break;

	}
}

//states for Prev button
void ButtonPrevIdleRoutine(buttonprev_t* Key){
if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin)){
	Key->State = DEBOUNCE;
	Key ->LastTick = HAL_GetTick();
}
}
void ButtonPrevPressedRoutine(buttonprev_t* Key){
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin)){
		Key->State = IDLE;
	}
}
void ButtonPrevDebounceRoutine(buttonprev_t* Key)
{
	if ((HAL_GetTick() - Key ->LastTick) > Key ->TimerDebounce){
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort , Key->GpioPin))
		{
			Key-> State = PRESSED;
			//if (Key->key_prev_press != NULL)
			//{
				Key->key_prev_press();
			//}

		}
		else {
			Key->State = IDLE;
		}
	}
}
//states Machine for Prev Button
void ButtonPrevTask(buttonprev_t* Key){

	switch(Key-> State){
	case IDLE:
		ButtonPrevIdleRoutine( Key);
		break;
	case DEBOUNCE:
		ButtonPrevDebounceRoutine( Key);
		break;
	case PRESSED:
		ButtonPrevPressedRoutine( Key);
		break;

	}
}
