/*
 * button.h
 *
 *  Created on: Mar 31, 2022
 *      Author: root
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

//states for state button
typedef enum {

	IDLE = 0,
	DEBOUNCE,
	PRESSED

} button_state;

//struct for button Back
typedef struct {
	button_state 		State;
	GPIO_TypeDef* 		GpioPort;
	uint16_t 			GpioPin;
	uint32_t 			TimerDebounce;
	uint32_t 			LastTick;
	void (*key_back_press)(void);

} buttonback_t;
//struct for button Enter
typedef struct {
	button_state 		State;
	GPIO_TypeDef* 		GpioPort;
	uint16_t 			GpioPin;
	uint32_t 			TimerDebounce;
	uint32_t 			LastTick;
	void (*key_enter_press)(void);
} buttonenter_t;
//struct for button Prev
typedef struct {
	button_state 		State;
	GPIO_TypeDef* 		GpioPort;
	uint16_t 			GpioPin;
	uint32_t 			TimerDebounce;
	uint32_t 			LastTick;
	void (*key_prev_press)(void);
} buttonprev_t;
//struct for button Next
typedef struct {
	button_state 		State;
	GPIO_TypeDef* 		GpioPort;
	uint16_t 			GpioPin;
	uint32_t 			TimerDebounce;
	uint32_t 			LastTick;
	void (*key_next_press)(void);
} buttonnext_t;
//public functions
void ButtonBackInitKey(buttonback_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce);
void ButtonEnterInitKey(buttonenter_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce);
void ButtonPrevInitKey(buttonprev_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce);
void ButtonNextInitKey(buttonnext_t* Key, GPIO_TypeDef 	*GpioPort, uint16_t  GpioPin, uint32_t 	TimerDebounce);

void ButtonBackTask(buttonback_t* Key);
void ButtonNextTask(buttonnext_t* Key);
void ButtonPrevTask(buttonprev_t* Key);
void ButtonEnterTask(buttonenter_t* Key);

void ButtonBackPressCallback(buttonback_t* Key, void *Callback);
void ButtonNextPressCallback(buttonnext_t* Key, void *Callback);
void ButtonPrevPressCallback(buttonprev_t* Key, void *Callback);
void ButtonEnterPressCallback(buttonenter_t* Key, void *Callback);


#endif /* INC_BUTTON_H_ */
