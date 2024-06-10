#include "ina226.h"
#include "i2c.h"
#define INA226_ASUKIAAA_LSB_BUS   1.25
#define INA226_ASUKIAAA_LSB_POWER 25

#define INA226_ASUKIAAA_REGISTER_CONFIG  0x00
#define INA226_ASUKIAAA_REGISTER_SHUNTV  0x01
#define INA226_ASUKIAAA_REGISTER_BUSV    0x02
#define INA226_ASUKIAAA_REGISTER_POWER   0x03
#define INA226_ASUKIAAA_REGISTER_CURRENT 0x04
#define INA226_ASUKIAAA_REGISTER_CALIB   0x05
#define INA226_ASUKIAAA_REGISTER_MASK    0x06
#define INA226_ASUKIAAA_REGISTER_ALERTL  0x07
#define INA226_ASUKIAAA_REGISTER_DIE_ID  0xff

typedef struct INA226_CONFIG {
	int address;
  uint16_t config;
  uint16_t calib;
	I2C_HandleTypeDef *hi2c;
} INA226_CONFIG;

INA226_CONFIG ina226_config;

void INA226_config(I2C_HandleTypeDef* wire, int address, uint16_t calib, uint16_t config) 
{
	ina226_config.hi2c = wire;
  ina226_config.address = address;
  ina226_config.calib = calib;
  ina226_config.config = config;
  //ina226_config.hi2c = 0;
}

//void INA226_setWire(I2C_HandleTypeDef* wire) 
//{
//  ina226_config.hi2c = wire;
//}

uint16_t calcCalibByResistorMicroOhm(uint32_t resistorMilliOhm) 
{
  //return (uint32_t) 5120 * 1000 / resistorMilliOhm;
	return (uint32_t) 5120  / resistorMilliOhm;
}

int INA226_begin() 
{
  if (ina226_config.hi2c == 0) 
	{
//    wire = &Wire;
//    wire->begin();
//    delay(10);
		return -1;
  }
	
  int result = INA226_write(INA226_ASUKIAAA_REGISTER_CONFIG, ina226_config.config);
  if (result != 0) return result;
  return INA226_write(INA226_ASUKIAAA_REGISTER_CALIB, ina226_config.calib);
}

int INA226_readMV(int16_t* volt) 
{
  uint16_t busVoltage;
  int result = INA226_read(INA226_ASUKIAAA_REGISTER_BUSV, (uint16_t*) &busVoltage);
  if (result != 0) return result;
  *volt = busVoltage * INA226_ASUKIAAA_LSB_BUS;
  return 0;
}

int INA226_readV(float* volt) 
{
  int16_t mv;
  int result = INA226_readMV(&mv);
  if (result != 0) return result;
  *volt = (float) mv / 1000;
  return 0;
}

int INA226_readMA(int16_t* current) 
{
  return INA226_read(INA226_ASUKIAAA_REGISTER_CURRENT, (uint16_t*) current);
}

int INA226_readA(float* current) 
{
  int16_t ma;
  int result = INA226_readMA(&ma);
  if (result != 0) return result;
  *current = (float) ma / 1000;
  return 0;
}

int INA226_readMW(int16_t* watt) 
{
  uint16_t powerVoltage;
  int result = INA226_read(INA226_ASUKIAAA_REGISTER_POWER, (uint16_t*) &powerVoltage);
  if (result != 0) return result;
  *watt = powerVoltage * INA226_ASUKIAAA_LSB_POWER;
  return 0;
}

int INA226_readW(float* watt) 
{
  int16_t mw;
  int result = INA226_readMW(&mw);
  if (result != 0) return result;
  *watt = (float) mw / 1000;
  return 0;
}

int INA226_write(uint8_t reg, uint16_t val) 
{
//  wire->beginTransmission(address);
//  wire->write(reg);
//  wire->write(val >> 8);
//  wire->write(val & 0xff);
//  return wire->endTransmission();
	
//	uint8_t arr[2] = { reg, cmd };
	
	uint8_t arr[3] = {0};
	arr[0] = reg;
	arr[1] = val >> 8;
	arr[2] = val & 0xff;
	
	HAL_I2C_Master_Transmit(ina226_config.hi2c, ina226_config.address << 1, arr, 3, I2C_TIMEOUT);
	
	return 0;
}

int INA226_read(uint8_t reg, uint16_t* val) 
{
  uint16_t ret = 0;

	uint8_t result[2] = {0};
	HAL_I2C_Master_Transmit(ina226_config.hi2c, ina226_config.address <<1 , &reg, 1, I2C_TIMEOUT);
	
	HAL_I2C_Master_Receive(ina226_config.hi2c, (ina226_config.address << 1) + 1, result, 2, I2C_TIMEOUT);
	
	ret  = result[0];
	ret = (ret <<8) | result[1];
	
  *val = ret;
  return 0;
}
