/*
 * brightness_callbac.h
 *
 *  Created on: Apr 4, 2022
 *      Author: Rafał Mulinski
 */

#ifndef INC_BRIGHTNESS_CALLBAC_H_
#define INC_BRIGHTNESS_CALLBAC_H_


void LCD_Brightness_Callback(void);
void LCD_Display_Test_Callback(void);
void LCD_Display_Compas_Callback(void);
void LCD_Display_Temperature_Callback(void) ;
void LCD_Display_Himidity_Callback(void) ;
void GPS_Display_Data_Callback(void);
void GPS_Display_Hour_Callback(void);
void GPS_Display_SatelitesNumber_Callback(void) ;
void GPS_Display_Altitude_Callback(void);
void GPS_Display_Latitude_Callback(void);
void GPS_Display_Longitude_Callback(void);
void GPS_Display_Speed_Refresh(void);

#endif /* INC_BRIGHTNESS_CALLBAC_H_ */
