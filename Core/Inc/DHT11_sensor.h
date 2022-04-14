/*
 * DHT11_sensor.h
 *
 *  Created on: Apr 5, 2022
 *      Author: root
 */

#ifndef INC_DHT11_SENSOR_H_
#define INC_DHT11_SENSOR_H_


typedef struct {
	float Temperature;
	float Humidity;
} DHT_DataTypedef;


void DHT_GetData (DHT_DataTypedef *DHT_Data);
uint8_t Temperature (void);
uint8_t Humidity (void);

#endif /* INC_DHT11_SENSOR_H_ */
