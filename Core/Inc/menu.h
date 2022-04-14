/*
 * menu.h
 *
 *  Created on: Mar 31, 2022
 *      Author: root
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_


typedef struct menustruct menu_t;

 struct menustruct{

	 char * name;

menu_t *next;
menu_t *prev;
menu_t *child;
menu_t *parent;
void(*menu_function)(void);



};
menu_t menu1;
	menu_t sub_menu1_1;
		menu_t sub_menu1_1_1;

	menu_t sub_menu1_2;
menu_t menu2;
	menu_t sub_menu2_1;
	menu_t sub_menu2_2;

menu_t menu3;
	menu_t sub_menu3_1;
	menu_t sub_menu3_2;
	menu_t sub_menu3_3;
	menu_t sub_menu3_4;
	menu_t sub_menu3_5;
	menu_t sub_menu3_6;
	menu_t sub_menu3_7;
menu_t menu4;
menu_t menu5;
menu_t menu6;

void menu_refresh(void);
void menu_next(void);
void menu_prev(void);
void menu_enter(void);
void menu_back(void);

void key_next_press(void);
void key_prev_press(void);
void key_enter_press(void);
void key_back_press(void);

#endif /* INC_MENU_H_ */
