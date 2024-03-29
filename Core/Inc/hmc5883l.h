/*
 * hmc5883l.h
 *
 *  Created on: Apr 5, 2022
 *      Author: root
 */

#ifndef INC_HMC5883L_H_
#define INC_HMC5883L_H_

#define HMC5883L_i2c_timeout     1000  // ms

#define HMC5883L_ADDRESS            0x1E // this device only has one address
#define HMC5883L_DEFAULT_ADDRESS    0x1E

#define HMC5883L_RA_CONFIG_A        0x00
#define HMC5883L_RA_CONFIG_B        0x01
#define HMC5883L_RA_MODE            0x02
#define HMC5883L_RA_DATAX_H         0x03
#define HMC5883L_RA_DATAX_L         0x04
#define HMC5883L_RA_DATAZ_H         0x05
#define HMC5883L_RA_DATAZ_L         0x06
#define HMC5883L_RA_DATAY_H         0x07
#define HMC5883L_RA_DATAY_L         0x08
#define HMC5883L_RA_STATUS          0x09
#define HMC5883L_RA_ID_A            0x0A
#define HMC5883L_RA_ID_B            0x0B
#define HMC5883L_RA_ID_C            0x0C

#define HMC5883L_CRA_AVERAGE_BIT    6   //0000 0110
#define HMC5883L_CRA_AVERAGE_LENGTH 2	//0000 0010
#define HMC5883L_CRA_RATE_BIT       4	//0000 0100
#define HMC5883L_CRA_RATE_LENGTH    3	//0000 0011
#define HMC5883L_CRA_BIAS_BIT       1   //0000 0001
#define HMC5883L_CRA_BIAS_LENGTH    2	//0000 0010

#define HMC5883L_AVERAGING_1        0x00
#define HMC5883L_AVERAGING_2        0x01
#define HMC5883L_AVERAGING_4        0x02
#define HMC5883L_AVERAGING_8        0x03

#define HMC5883L_RATE_0P75          0x00
#define HMC5883L_RATE_1P5           0x01
#define HMC5883L_RATE_3             0x02
#define HMC5883L_RATE_7P5           0x03
#define HMC5883L_RATE_15            0x04
#define HMC5883L_RATE_30            0x05
#define HMC5883L_RATE_75            0x06

#define HMC5883L_BIAS_NORMAL        0x00
#define HMC5883L_BIAS_POSITIVE      0x01
#define HMC5883L_BIAS_NEGATIVE      0x02

#define HMC5883L_CRB_GAIN_BIT       7
#define HMC5883L_CRB_GAIN_LENGTH    3

#define HMC5883L_GAIN_1370          0x00
#define HMC5883L_GAIN_1090          0x01
#define HMC5883L_GAIN_820           0x02
#define HMC5883L_GAIN_660           0x03
#define HMC5883L_GAIN_440           0x04
#define HMC5883L_GAIN_390           0x05
#define HMC5883L_GAIN_330           0x06
#define HMC5883L_GAIN_220           0x07

#define HMC5883L_MODEREG_BIT        1
#define HMC5883L_MODEREG_LENGTH     2

#define HMC5883L_MODE_CONTINUOUS    0x00
#define HMC5883L_MODE_SINGLE        0x01
#define HMC5883L_MODE_IDLE          0x02

#define HMC5883L_STATUS_LOCK_BIT    1
#define HMC5883L_STATUS_READY_BIT   0

#define PI		 3.14159265



typedef struct {
	I2C_HandleTypeDef 			*hmc5883l_i2c;
	uint8_t 					Address;



} HMC5883L_t;

int16_t HMC5883L_getHeadingY(HMC5883L_t *hmc , uint8_t Register);
int16_t HMC5883L_getHeadingZ(HMC5883L_t *hmc , uint8_t Register);
int16_t HMC5883L_getHeadingX(HMC5883L_t *hmc , uint8_t Register);
void Uart_Message(char *Message);
uint16_t Read16(HMC5883L_t *hmc , uint8_t Register);
void HMC5883L_OperatingMode(HMC5883L_t *hmc , uint8_t Mode);
uint8_t  HMC5883L_Init(HMC5883L_t *hmc, I2C_HandleTypeDef *i2c, uint8_t Address  ) ;
void HMC5883L_Gain(HMC5883L_t *hmc , uint8_t Mode);
void HMC5883L_SamplesAveraged(HMC5883L_t *hmc , uint8_t Mode);
void HMC5883L_DataOutputRate(HMC5883L_t *hmc , uint8_t Mode);
void HMC5883L_MeasureMode(HMC5883L_t *hmc , uint8_t Mode);
float Direction_Radians(HMC5883L_t *hmc );
float Direction_Degress(HMC5883L_t *hmc );
int16_t Compas_Degress(void);

#endif /* INC_HMC5883L_H_ */
