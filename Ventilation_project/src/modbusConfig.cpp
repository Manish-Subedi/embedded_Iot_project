/*
 * modbusConfig.cpp
 *
 *  Created on: 4 Nov 2022
 *      Author: Manish
 */

#include "chip.h"
#include "delay.h"
#include "modbusConfig.h"

modbusConfig::modbusConfig():
	// TODO Auto-generated constructor stub
	node_fan(fan_ID), // Create modbus object that connects to slave id
	node_hmp(HMP60_ID), //rh&temp
	node_gmp(GMP252_ID), //co2

	freq_fan(&node_fan, write_fan_r_ADD),
	pulse_fan(&node_fan, read_fan_r_ADD, false),
	temp_(&node_hmp, read_temp_r_ADD, true),
	rh_(&node_hmp, read_rh_r_ADD, true),
	co2_(&node_gmp, read_co2_r_ADD, true)
	{
		node_fan.begin(TRANSMISSION_RATE);
		node_hmp.begin(TRANSMISSION_RATE);
		node_gmp.begin(TRANSMISSION_RATE);
	}


void modbusConfig::set_speed(const uint16_t& s)
{
	int speed = s;
	freq_fan.write(speed*10);
}
/*
int modbusConfig::get_frequency()
{
	return read_fan_r.read();
}*/
int modbusConfig::get_temp(){
	/*
	bool ready =false;
	int timeout = 0;
	while(!ready && timeout < TIMEOUT_LIMIT ){
		delay_systick(500);
		if (read_fan_r.read()) ready =true;
	}
	*/
	return temp_.read()/10;
	delay_systick(500);
}

int modbusConfig::get_rh(){
	bool ready =false;
	int timeout = 0;
	/*while(!Error_rh_r.read() && !ready){
		delay_systick(500);
		if (read_rh_r.read()) ready =true;
	}*/
    return rh_.read();
    delay_systick(500);
}

int modbusConfig::get_co2(){
	/*bool ready =false;
	int timeout = 0;
	while(!ready && timeout < TIMEOUT_LIMIT ){
		if (read_co2_r.read()) ready =true;
	}*/
   return (co2_.read()/10)*100;
   delay_systick(500);
}

modbusConfig::~modbusConfig() {
	// TODO Auto-generated destructor stub
}
