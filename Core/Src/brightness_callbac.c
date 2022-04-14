/*
 * brightness_callbac.c
 *
 *  Created on: Apr 13, 2022
 *      Author: Rafał Mulinski
 */

#include "main.h"
#include <string.h>
#include "stdlib.h"
#include <stdio.h>
#include "menu.h"
#include "HD44780.h"
#include "tim.h"
#include "hmc5883l.h"
#include "DHT11_sensor.h"
#include "GPS.h"

extern void (*key_next_func)(void);
extern void (*key_prev_func)(void);
extern void (*key_enter_func)(void);
extern void (*key_back_func)(void);
UART_HandleTypeDef huart1;

//uint8_t Temperature, Humidity;

void LCD_Brightness_Refresh(void);
extern uint8_t PWM;

void LCD_Brightness_Next (void){


	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,PWM);  // od  1 do 35

	PWM-=10;
	LCD_Brightness_Refresh();
}

void LCD_Brightness_Back(void) {
	key_next_func = menu_next;
	key_prev_func = menu_prev;
	key_enter_func = menu_enter;
	key_back_func = menu_back;

	menu_refresh();

}
void LCD_Brightness_Prev (void){

		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,PWM);  // od  1 do 35
PWM+=10;
LCD_Brightness_Refresh();
}
//
//Brightness setting function
//
void LCD_Brightness_Refresh (void){
		//uint8_t percent = (TIM2->CCR1)/550;
		LCD_Cls();
		LCD_write(0,0, "-- BRIGHTNESS --");
		LCD_Menu_Clear ();

		//memset(LCD_Char,0xff,percent/10);  //LCD_Char   memset(&lcd_buf[2][8],0xff,percent/10)
		//memset(&lcd_buf[2][8+percent/10], '-',10-percent/10);

		LCD_Locate(5,1);
		LCD_Int(PWM);
		LCD_Locate(8,1);
		LCD_Char('%');
}
//
//Test function display
//
void LCD_Display_Test_Refresh(void) {
	//uint8_t percent = (TIM2->CCR1)/550;
	uint8_t x = 0;
	LCD_Cls();
	LCD_write(0, 0, "---- TEST ----");

	LCD_Cls();
	LCD_Locate(0, 0);
	for (x = 0; x <= 15; x++) {
		LCD_Locate(x, 0);
		LCD_Char(0xff);
		HAL_Delay(100);
	}
	LCD_Locate(0, 1);
	for (x = 0; x <= 15; x++) {
		LCD_Locate(x, 1);
		LCD_Char(0xff);
		HAL_Delay(100);
	}

}
//
//Compass value display function
//
void LCD_Display_Compas_Refresh(void) {

	int16_t x ;
	LCD_Cls();
	LCD_write(0, 0, "--- COMPAS ----");

	LCD_Menu_Clear();
	while(HAL_GPIO_ReadPin(GPIOC , key_prev_Pin)){
		HAL_Delay(150);
		x = Compas_Degress();
		if (x>=0 ){
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("PN");
		}
		if (x >= 20 ) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("PN-W");
		}
		if (x >= 70) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("W");
		}
		if (x >= 110 ) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("PD-W");
		}
		if (x >=160 ) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("PD");
		}
		if (x >= 200 ) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("PD-Z");
		}
		if (x >= 250 ) {
			LCD_Menu_Clear();
			LCD_Locate(0, 1);
			LCD_Int(x);
			LCD_Locate(3, 1);
			LCD_Char(0xDF);
			LCD_Locate(6, 1);
			LCD_String("Z");
		}
		if (x >= 290) {
			LCD_Menu_Clear();
				LCD_Locate(0, 1);
				LCD_Int(x);
				LCD_Locate(3, 1);
				LCD_Char(0xDF);
				LCD_Locate(6, 1);
				LCD_String("PN-Z");
			}
		if (x>290 ){
				LCD_Menu_Clear();
				LCD_Locate(0, 1);
				LCD_Int(x);
				LCD_Locate(3, 1);
				LCD_Char(0xDF);
				LCD_Locate(6, 1);
				LCD_String("PN");
			}
	}

	}
//
//Temperature value display function
//
void LCD_Display_Temperature_Refresh(void) {
	//uint8_t percent = (TIM2->CCR1)/550;
	uint8_t x = 0;
	LCD_Cls();
	LCD_write(0, 0, "-- Temperature --");

	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		LCD_Menu_Clear();
		x = Temperature();
		LCD_Locate(0, 1);
		LCD_Int(x);
		LCD_Locate(3, 1);
		LCD_Char(0xDF);
		LCD_Locate(4, 1);
		LCD_Char(0x43);
		HAL_Delay(300);

}
}
//
//Humidity value display function
//
void LCD_Display_Humidity_Refresh(void) {
	uint8_t x = 0;
	LCD_Cls();
	LCD_write(0, 0, "--- Humidity ----");

	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		LCD_Menu_Clear();
		x = Humidity();
		LCD_Locate(0, 1);
		LCD_Int(x);
		LCD_Locate(3, 1);
		LCD_Char(0x25);
		HAL_Delay(300);
}
}
//
//GPS data value display function
//
void GPS_Display_Data_Refresh(void) {
	uint8_t day;
	uint8_t month;
	uint8_t year;
	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		//NEO6_Task(&GpsState);
		LCD_Menu_Clear();
		day = GpsState.Day ;
		month = GpsState.Month ;
		year = GpsState.Year ;
		LCD_Locate(0, 1);
		LCD_String("Data:");
		LCD_Locate(6, 1);
		LCD_Int(day);
		LCD_Locate(9, 1);
		LCD_Int(month);
		LCD_Locate(12, 1);
		LCD_Int(year);
		HAL_Delay(1000);
		//NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS hour value display function
//
void GPS_Display_Hour_Refresh(void) {

	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);
		LCD_Menu_Clear();
		hour = GpsState.Hour ;
		minute = GpsState.Minute ;
		second = GpsState.Second;
		LCD_Locate(0, 1);
		LCD_String("Hour:");
		LCD_Locate(6, 1);
		LCD_Int(hour);

		LCD_Locate(9, 1);
		LCD_Int(minute);
		LCD_Locate(12, 1);
		LCD_Int(second);
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS satelites number value display function
//
void GPS_Display_SatelitesNumber_Refresh(void) {

	uint8_t satelitenumber;

	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);
		LCD_Menu_Clear();
		satelitenumber = GpsState.SatelitesNumber ;

		LCD_Locate(0, 1);
		LCD_String("Satelite:");
		LCD_Locate(10, 1);
		LCD_Int(satelitenumber);
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS altitude value display function
//
void GPS_Display_Altitude_Refresh(void) {

	uint8_t altitude;

	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);
		LCD_Menu_Clear();
		altitude = GpsState.Altitude ;

		LCD_Locate(0, 1);
		LCD_String("Altitude:");
		LCD_Locate(10, 1);
		LCD_Int(altitude);
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS latitude value display function
//
void GPS_Display_Latitude_Refresh(void) {

	double latitude;
	char latitudedirection;
	char  numi [15];

	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);

		LCD_Menu_Clear();
		latitude = GpsState.Latitude ;
		latitudedirection =  GpsState.LatitudeDirection;
		sprintf((char*)numi, "%.2f", latitude);

		LCD_Locate(0, 1);
		LCD_String("Lati:");
		LCD_Locate(6, 1);
		LCD_String(numi);
		LCD_Locate(15, 1);
		LCD_String(&latitudedirection);
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS longitude value display function
//
void GPS_Display_Longitude_Refresh(void) {

	double longitude;
	char longitudedirection;
	char  numi [15];

	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);

		LCD_Menu_Clear();
		longitude = GpsState.Longitude ;
		longitudedirection =  GpsState.LongitudeDirection;
		sprintf((char*)numi, "%.2f", longitude);

		LCD_Locate(0, 1);
		LCD_String("Long:");
		LCD_Locate(6, 1);
		LCD_String(numi);
		LCD_Locate(15, 1);
		LCD_String(&longitudedirection);
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//GPS speed value display function
//
void GPS_Display_Speed_Refresh(void) {

	double speedkilometers;
	//char longitudedirection;
	char  numi [15];

	LCD_Cls();
	LCD_write(0, 0, "---  GPS ----");



	while (HAL_GPIO_ReadPin(GPIOC, key_prev_Pin)) {
		NEO6_Task(&GpsState);

		LCD_Menu_Clear();
		speedkilometers = GpsState.SpeedKilometers ;

		sprintf((char*)numi, "%.2f", speedkilometers);

		LCD_Locate(0, 1);
		LCD_String("Speed:");
		LCD_Locate(6, 1);
		LCD_String(numi);
		LCD_Locate(12, 1);
		LCD_String("km/h");
		HAL_Delay(100);
		NEO6_Recive_Data(&GpsState, &huart1);
}
}
//
//Function that changes the functions of the button
//
void GPS_Display_Speed_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Speed_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_Longitude_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Longitude_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_Latitude_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Latitude_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_Altitude_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Altitude_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_SatelitesNumber_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_SatelitesNumber_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_Hour_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Hour_Refresh();
}
//
//Function that changes the functions of the button
//
void GPS_Display_Data_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	GPS_Display_Data_Refresh();
}
//
//Function that changes the functions of the button
//
void LCD_Display_Himidity_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	LCD_Display_Humidity_Refresh();
}
//
//Function that changes the functions of the button
//
void LCD_Display_Temperature_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	LCD_Display_Temperature_Refresh();
}
//
//Function that changes the functions of the button
//
void LCD_Display_Compas_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	LCD_Display_Compas_Refresh();
}
//
//Function that changes the functions of the button
//
void LCD_Brightness_Callback(void) {
	key_next_func = LCD_Brightness_Next;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = LCD_Brightness_Prev;
	LCD_Brightness_Refresh();
}
//
//Function that changes the functions of the button
//
void LCD_Display_Test_Callback(void) {
	key_next_func = NULL;
	key_back_func = LCD_Brightness_Back;
	key_enter_func = NULL;
	key_prev_func = NULL;
	LCD_Display_Test_Refresh();
}

