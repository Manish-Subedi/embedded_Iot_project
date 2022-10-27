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

#include "DigitalIoPin.h"
#include "LpcUart.h"
#include "LiquidCrystal.h"
#include "I2C.h"
#include "./modbus/ModbusMaster.h"
#include "./modbus/ModbusRegister.h"
#include "SimpleMenu.h"
#include "IntegerEdit.h"
#include "ModeEdit.h"
#include "retarget_uart.h"
#include "../src/mqtt/esp8266_socket.h"
#include "../src/paho_mqtt/MQTTClient-C/src/MQTTClient.h"

//#include "MqttController.h"

//#include "jsmn.h"

#define SDP_CMD_READ (0xF1)
#define SDP_ADDRESS (0x40)

#define SSID	    "SmartIotMQTT"
#define PASSWORD    "SmartIot"
#define BROKER_IP   "192.168.1.254"
#define BROKER_PORT  1883
#define MQTT_TIMEOUT 30000
#define MQTT_TOPIC_RECEIVE   (const char *)"Group5/controller/settings"
#define MQTT_TOPIC_SEND      (const char *)"Group5/controller/status"
#define TICKRATE_HZ (1000)
#define JSMN_TOKENS 20

#define PININT_IRQ_HANDLER  PIN_INT0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */

static volatile int counter;
static volatile uint32_t systicks;

#define SW1_PORT 1
#define SW1 PIN 8
#define SW2_PORT 1
#define SW2 PIN
#define SW3_PORT 1
#define SW3 PIN
#define MAX_DEBOUNCE             30

static volatile int button_pressed_time(0);
static SimpleMenu menu;
static ModeEdit *modes;

#endif /* SRC_MAINHEADER_H_ */
