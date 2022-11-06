/*
 * MainHeader.h
 *
 *  Created on: 20 Oct 2022
 *      Author: Manish
 */

#ifndef SRC_MAINHEADER_H_
#define SRC_MAINHEADER_H_

#include "chip.h"
#include "board.h"
#include <cr_section_macros.h>
#include "ritimer_15xx.h"
#include <atomic>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <string>
#include <cstdlib>

#include "DigitalIoPin.h"
#include "LpcUart.h"
#include "LiquidCrystal.h"
#include "I2C.h"
#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"
#include "SimpleMenu.h"
#include "IntegerEdit.h"
#include "StringEdit.h"
#include "retarget_uart.h"
#include "../src/mqtt/esp8266_socket.h"
#include "paho.mqtt/MQTTClient-C/MQTTClient.h"
#include "jsmn.h"
#include "MQTT.h"
#include "modbusConfig.h"
#include "json.hpp"

#define SDP_CMD_READ (0xF1)
#define SDP_ADDRESS (0x40)

#define SSID	    "SmartIotMQTT"
#define PASSWORD    "SmartIot"
#define BROKER_IP   (char *)"192.168.1.254"
#define BROKER_PORT  1883
#define MQTT_TIMEOUT 30000
#define MQTT_TOPIC_SETTING   (const char *)"Group5/controller/settings"
#define MQTT_TOPIC_SEND      (const char *)"Group5/controller/status"
#define TICKRATE_HZ (1000)
#define JSMN_TOKENS 20

static volatile int counter;
static volatile uint32_t systicks(0);

#define MAX_DEBOUNCE 30


static volatile int button_pressed_time(0);
static std::string mqtt_message("");
static bool mqtt_message_arrived(false);
void message_handler(MessageData* data);
static SimpleMenu menu;
static StringEdit *modes;

#endif /* SRC_MAINHEADER_H_ */
