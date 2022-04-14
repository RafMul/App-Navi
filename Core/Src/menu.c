/*
 * menu.c
 *
 *  Created on: Mar 31, 2022
 *      Author: Rafal Mulinski
 */


#include "main.h"
#include "menu.h"
#include <string.h>
#include "HD44780.h"
#include "brightness_callbac.h"


// definition of menu's components: (*name, *next, *prev, *child, *parent, (*menu_function))
menu_t menu1 = { "DISPLAY", &menu2, &menu6, &sub_menu1_1, NULL, NULL };
	menu_t sub_menu1_1 = { "Contrast", &sub_menu1_2, &sub_menu1_2, &sub_menu1_1_1, &menu1, NULL };
		menu_t sub_menu1_1_1 = { "Test", NULL, 		  &sub_menu1_1_1, NULL, &sub_menu1_1, LCD_Display_Test_Callback };
	menu_t sub_menu1_2 = { "Brightness", NULL, 		  &sub_menu1_1, NULL, &menu1, LCD_Brightness_Callback };
menu_t menu2 = { "COPMPAS",             &menu3, 	  &menu1, 		&sub_menu2_1, NULL, NULL };
	menu_t sub_menu2_1 = { "DEGRESS",   &sub_menu2_2, &sub_menu2_2, NULL, &menu2, LCD_Display_Compas_Callback };
	menu_t sub_menu2_2 = { "RADIANS",   NULL,		  &sub_menu2_1, NULL, &menu2, NULL };
menu_t menu3 = { "GPS", &menu4, &menu2, &sub_menu3_1, 		  NULL, 		NULL };
	menu_t sub_menu3_1 = { "UTC Time",  &sub_menu3_2, &sub_menu3_7, NULL, &menu3, GPS_Display_Hour_Callback };
	menu_t sub_menu3_2 = { "Date",      &sub_menu3_3, &sub_menu3_1, NULL, &menu3, GPS_Display_Data_Callback };
	menu_t sub_menu3_3 = { "Latitude",  &sub_menu3_4, &sub_menu3_2, NULL, &menu3, GPS_Display_Latitude_Callback };
	menu_t sub_menu3_4 = { "Longitude", &sub_menu3_5, &sub_menu3_3, NULL, &menu3, GPS_Display_Longitude_Callback  };
	menu_t sub_menu3_5 = { "Speed",     &sub_menu3_6, &sub_menu3_4, NULL, &menu3, GPS_Display_Speed_Refresh };
	menu_t sub_menu3_6 = { "Satellites", &sub_menu3_7, &sub_menu3_5, NULL, &menu3, GPS_Display_SatelitesNumber_Callback };
	menu_t sub_menu3_7 = { "Altitude", 		NULL, &sub_menu3_6, NULL, &menu3, GPS_Display_Altitude_Callback };
menu_t menu4 = { "TEMPERATURE", &menu5, &menu3, NULL, NULL, LCD_Display_Temperature_Callback };
menu_t menu5 = { "HUMIDITY", &menu6, &menu4, NULL, NULL, LCD_Display_Himidity_Callback };
menu_t menu6 = { "ELEMENT 6", NULL, &menu5, NULL, NULL, NULL };

menu_t *currentPointer = &menu1;
uint8_t menu_index;
uint8_t lcd_row_pos;

uint8_t lcd_row_pos_level_1, lcd_row_pos_level_2;

void (*key_next_func)(void) = &menu_next;
void (*key_prev_func)(void) = &menu_prev;
void (*key_enter_func)(void) = &menu_enter;
void (*key_back_func)(void) = &menu_back;

void menu_refresh(void) {

	menu_t *temp;
	uint8_t i;
	uint8_t center;


	LCD_Cls();

	//LCD_write(0, 0, "-   NAVI APP   -");

	if (currentPointer->parent){
		temp = (currentPointer->parent)->child;
		center = (LCD_X >>1 )-(strlen((temp->parent)->name)>>1);
		LCD_Locate(center-1, 0);
		LCD_Char(' ');
		LCD_String((temp->parent)->name);
		LCD_Char(' ');
	}

	else
		temp = &menu1;

	for (i = 0; i != menu_index - lcd_row_pos; i++) {
		temp = temp->next;
	}
	//LCD_Cls();
	LCD_Menu_Clear ();
	for (i = 1; i < LCD_Y ; i++) {
		LCD_Locate(0, i);
		if (temp == currentPointer) LCD_Char(0x7E);
		else LCD_Char(' ');

		LCD_Locate(2, i);
		LCD_String(temp->name);

		temp = temp->next;
		if (!temp) break;

	}



}

	// lcd_refresh();



uint8_t menu_get_index(menu_t *q) {

	menu_t *temp;
	uint8_t i = 0;

	if (q->parent) temp = (q->parent)->child;
	else temp = &menu1;

	while (temp != q) {
		temp = temp->next;
		i++;
	}

	return i;
}

uint8_t menu_get_level(menu_t *q) {

	menu_t *temp = q;
	uint8_t i = 0;

	if (!q->parent) return 0;

	while (temp->parent != NULL) {
		temp = temp->parent;
		i++;
	}

	return i;
}
void key_next_press(void) {

	//menu_prev();
	if(key_next_func) (*key_next_func)();
}
void key_prev_press(void) {

	//menu_next();
	if(key_prev_func) (*key_prev_func)();
}
void key_enter_press(void) {

	//menu_enter();
	if(key_enter_func) (*key_enter_func)();
}
void key_back_press(void) {

	//menu_back();
	if(key_back_func) (*key_back_func)();
}

void menu_back(void) {

	if (currentPointer->parent) {

		switch (menu_get_level(currentPointer)) {
		case 1:
			lcd_row_pos = lcd_row_pos_level_1;
			break;

		case 2:
			lcd_row_pos = lcd_row_pos_level_2;
			break;
		}

		currentPointer = currentPointer->parent;
		menu_index = menu_get_index(currentPointer);

		menu_refresh();

	}
}


void menu_enter(void) {

	if (currentPointer->menu_function) currentPointer->menu_function();

	if (currentPointer->child)
	{

		switch (menu_get_level(currentPointer)) {
			case 0:
				lcd_row_pos_level_1 = lcd_row_pos;
				break;

			case 1:
				lcd_row_pos_level_2 = lcd_row_pos;
				break;
		}

		// switch...case can be replaced by:
		// lcd_row_pos_level[ menu_get_level(currentPointer) ] = lcd_row_pos;

		menu_index = 0;
		lcd_row_pos = 0;

		currentPointer = currentPointer->child;

		menu_refresh();
	}
}

void menu_next(void) {

	if (currentPointer->next)
	{
		currentPointer = currentPointer->next;
		menu_index++;
		if (++lcd_row_pos > LCD_Y - 2) lcd_row_pos = LCD_Y - 2;
	}
	else
	{
		menu_index = 0;
		lcd_row_pos = 0;

		if (currentPointer->parent) currentPointer = (currentPointer->parent)->child;
		else currentPointer = &menu1;
	}

	menu_refresh();

}
void menu_prev(void){
	currentPointer = currentPointer -> prev;
	if (menu_index){
		menu_index--;
		if (lcd_row_pos > 0) lcd_row_pos--;
	}
	else{
		menu_index = menu_get_index(currentPointer);
		if (menu_index >= LCD_Y - 2) lcd_row_pos = LCD_Y - 2;
		else lcd_row_pos = menu_index;
	}
	menu_refresh();
}
