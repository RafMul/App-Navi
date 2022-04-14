/*
 * hmc5883l.c
 *
 *  Created on: Apr 5, 2022
 *      Author: Rafał Mulisnki
 */

#include "main.h"
#include "hmc5883l.h"
#include "math.h"


int16_t TmpX;
int16_t TmpY;
int16_t TmpZ;
float TmpXY;
int16_t Degress;
HMC5883L_t Hmc5883l;
/*
 * Function Read 8-bit data register
 */
uint8_t Read8(HMC5883L_t *hmc , uint8_t Register)
{
	uint8_t Value;

	HAL_I2C_Mem_Read(hmc->hmc5883l_i2c, (hmc->Address)<<1, Register, 1, &Value, 1,
			HMC5883L_i2c_timeout);
	return Value;

}
/*
 * Function Read 16-bit data register
 */
uint16_t Read16(HMC5883L_t *hmc, uint8_t Register) {
	uint8_t Value[2];
	HAL_I2C_Mem_Read(hmc->hmc5883l_i2c, (hmc->Address) << 1, Register, 1, Value,
			2,
			HMC5883L_i2c_timeout);
	return ((Value[1] << 8) | Value[0]);
}
/*
 * Reads the X axis value register
 */
int16_t HMC5883L_getHeadingX(HMC5883L_t *hmc, uint8_t Register) {

	uint8_t Value[6];
	HAL_I2C_Mem_Read(hmc->hmc5883l_i2c, (hmc->Address) << 1, Register, 1, Value,
			6, HMC5883L_i2c_timeout);

	return (((int16_t) Value[0]) << 8) | Value[1];
}
/*
 * Reads the Z axis value register
 */
int16_t HMC5883L_getHeadingZ(HMC5883L_t *hmc, uint8_t Register) {
	uint8_t Value[6];
	HAL_I2C_Mem_Read(hmc->hmc5883l_i2c, (hmc->Address) << 1, Register, 1, Value,
			6, HMC5883L_i2c_timeout);

	return (((int16_t) Value[2]) << 8) | Value[3];
}
/*
 * Reads the Y axis value register
 */
int16_t HMC5883L_getHeadingY(HMC5883L_t *hmc, uint8_t Register) {
	uint8_t Value[6];
	HAL_I2C_Mem_Read(hmc->hmc5883l_i2c, (hmc->Address) << 1, Register, 1, Value,
			6, HMC5883L_i2c_timeout);

	return (((int16_t) Value[4]) << 8) | Value[5];
}
/*
 * Writes 8 bit to register
 */
void Write8(HMC5883L_t *hmc , uint8_t Register , uint8_t Value)
{

	HAL_I2C_Mem_Write(hmc->hmc5883l_i2c, (hmc->Address)<<1, Register, 1, &Value, 1,HMC5883L_i2c_timeout);
}
// Configuration register A for HMC5883L

void HMC5883L_MeasureMode(HMC5883L_t *hmc , uint8_t Mode)
{
	uint8_t Tmp;
	if(Mode>3) Mode =3;

	Tmp = Read8(hmc , HMC5883L_RA_CONFIG_A );
	Tmp = Tmp |(Mode  << 0)	;
	//Tmp = Tmp & 0xFC;
	//Tmp |= Mode ;

	Write8(hmc , HMC5883L_RA_CONFIG_A  ,Tmp);
}
/*Used data rate value
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 *
 */
void HMC5883L_DataOutputRate(HMC5883L_t *hmc , uint8_t Mode){
	uint8_t Tmp;
	Tmp = Read8(hmc , HMC5883L_RA_CONFIG_A );
	Tmp = Tmp |(Mode << 2 )	;			// a = a | (1 << 3)

	Write8(hmc , HMC5883L_RA_CONFIG_A  ,Tmp);

}

void HMC5883L_SamplesAveraged(HMC5883L_t *hmc , uint8_t Mode){
	uint8_t Tmp;
	Tmp = Read8(hmc , HMC5883L_RA_CONFIG_A );
	Tmp = Tmp |(Mode << 5 )	;			// a = a | (1 << 3)

	Write8(hmc , HMC5883L_RA_CONFIG_A  ,Tmp);

}
// Configuration register B for HMC5883L
void HMC5883L_Gain(HMC5883L_t *hmc , uint8_t Mode){
	uint8_t Tmp;
	Tmp = Read8(hmc , HMC5883L_RA_CONFIG_B );
	Tmp = Tmp |(Mode << 5 )	;			// a = a | (1 << 3)

	Write8(hmc , HMC5883L_RA_CONFIG_B  ,Tmp);

}
//Configuration Mode Register
//Operating Mode
/*
 * 		HMC5883L_MODE_CONTINUOUS
 	 	HMC5883L_MODE_SINGLE
 	 	HMC5883L_MODE_IDLE
 */
void HMC5883L_OperatingMode(HMC5883L_t *hmc , uint8_t Mode){
	uint8_t Tmp;
	Tmp = Read8(hmc , HMC5883L_RA_MODE  );
	Tmp =  Mode	;			// a = a | (1 << 3)

	Write8(hmc , HMC5883L_RA_MODE   ,Tmp);

}
uint8_t HMC5883L_StatusRegister(HMC5883L_t *hmc , uint8_t Mode){
	uint8_t Tmp;
	Tmp = Read8(hmc , HMC5883L_RA_STATUS   );
	return Tmp;
}

/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */


float Direction_Radians(HMC5883L_t *hmc) {
	float X;
	float Y;
	float XY;
	float declinationAngle = (6.0 + (24.0 / 60.0)) / (180 / PI);
	X = HMC5883L_getHeadingX(hmc, HMC5883L_RA_DATAX_H);
	Y = HMC5883L_getHeadingY(hmc, HMC5883L_RA_DATAX_H);
	XY = atan2(Y, X) ;
	XY += declinationAngle;

	if (XY < 0)
	  {
		XY += 2 * PI;
	  }

	  if (XY > 2 * PI)
	  {
		  XY -= 2 * PI;
	  }
	return XY;
}

float Direction_Degress(HMC5883L_t *hmc) {
	float Y;
	float s;
	Y = Direction_Radians(hmc);
	s = Y * (180 / PI);
	return s;
}

int16_t Compas_Degress(void) {
	TmpX = HMC5883L_getHeadingX(&Hmc5883l, HMC5883L_RA_DATAX_H);
	TmpY = HMC5883L_getHeadingY(&Hmc5883l, HMC5883L_RA_DATAX_H);
	TmpZ = HMC5883L_getHeadingZ(&Hmc5883l, HMC5883L_RA_DATAX_H);
	TmpXY = Direction_Radians(&Hmc5883l);				//angle in radians
	Degress = Direction_Degress(&Hmc5883l);				//angle in degrees


	return Degress;
}




uint8_t HMC5883L_Init(HMC5883L_t *hmc, I2C_HandleTypeDef *i2c, uint8_t Address) {
	hmc->hmc5883l_i2c = i2c;
	hmc->Address = Address;
	HMC5883L_SamplesAveraged(hmc, HMC5883L_AVERAGING_8);
	HMC5883L_DataOutputRate(hmc, HMC5883L_RATE_15);
	HMC5883L_MeasureMode(hmc, HMC5883L_BIAS_NORMAL);
	HMC5883L_Gain(hmc, HMC5883L_GAIN_1370);
	HMC5883L_OperatingMode(hmc, 0x00);
	return 0;
}






