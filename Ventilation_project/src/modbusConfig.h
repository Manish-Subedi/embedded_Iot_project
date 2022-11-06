/*
 * modbusConfig.h
 *
 *  Created on: 4 Nov 2022
 *      Author: Manish
 */

#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"

#define fan_ID               1
#define HMP60_ID             241
#define GMP252_ID            240

#define TRANSMISSION_RATE    9600

#define write_fan_r_ADD          0
#define read_fan_r_ADD           4

#define read_temp_r_ADD          0x0101
#define read_rh_r_ADD            0x0110

#define read_co2_r_ADD           0x0101

#define Error_rh_r_ADD           0x0200

#define TIMEOUT_LIMIT        20
#define SUCCESS_MASK         0x0100
#define BREAK_TIME           1000
#ifndef SRC_MODBUSCONFIG_H_
#define SRC_MODBUSCONFIG_H_

class modbusConfig {
public:
	modbusConfig();
	void set_speed(const uint16_t& freq);
	//int get_speed();
	int get_temp();
	int get_rh();
	int get_co2();
	virtual ~modbusConfig();
private:
	ModbusMaster node_fan; // Create modbus object that connects to slave id 1
	ModbusMaster node_hmp; //rh&temp
	ModbusMaster node_gmp; //co2

	ModbusRegister freq_fan;
	ModbusRegister pulse_fan;
	ModbusRegister temp_;
	ModbusRegister rh_;
	ModbusRegister co2_;
	//ModbusRegister Error_rh_r;
};

#endif /* SRC_MODBUSCONFIG_H_ */
