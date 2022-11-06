/*
===============================================================================
 Name        : main.c
 Author      : Manish Subedi
 Version     : release 1.0
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "MainHeader.h"


#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
	void SysTick_Handler(void)
	{
		systicks++;
		if(counter > 0) counter--;
	}

	uint32_t get_ticks(void)
	{
		return systicks;
	}

	/* Pin interrupt handler*/
	void PIN_INT0_IRQHandler(void){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));

		// make sure up is only executed once on button debounce
		if (millis() - button_pressed_time < MAX_DEBOUNCE) return;
		button_pressed_time = millis();

		menu.event(MenuItem::up);
	}
	void PIN_INT1_IRQHandler(void){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));

		// make sure up is only executed once on button debounce
		if (millis() - button_pressed_time < MAX_DEBOUNCE) return;
		button_pressed_time = millis();

		if(menu.getPos() == 0) menu.event(MenuItem::ok);
		//check if mode is auto and set pressure is focused
		if(modes->getValue() == 1 && menu.getPos() == 3){
			//allow setting pressure
			menu.event(MenuItem::ok);
		}
		if(modes->getValue() == 0 && menu.getPos() == 4){ //check if mode is manual and set speed is focused
			menu.event(MenuItem::ok); //allow setting speed
		}
	}
	void PIN_INT2_IRQHandler(void){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));

		// make sure up is only executed once on button debounce
		if (millis() - button_pressed_time < MAX_DEBOUNCE) return;
		button_pressed_time = millis();

		menu.event(MenuItem::down);
	}

#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}

/* function declarations */
uint16_t SDP_read();
void message_handler(MessageData*);
std::string sample_json(int sample_number, int speed, int setpoint,int pressure, const char* mode, const char* error, int co2, int rh, int temp);

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
#endif
#endif
    /* check watchdog status flag */
	uint32_t wdStatus = Chip_WWDT_GetStatus(LPC_WWDT);
	if(wdStatus) {
		Board_LED_Set(2, true); //set blue led if reset caused by watchdog
	}
	else Board_LED_Set(1, true); //else green led
    uint32_t wdtFreq;
    /* Enable the WDT oscillator */
    Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_WDTOSC_PD);
    /* The WDT divides the input frequency into it by 4 */
    wdtFreq = Chip_Clock_GetWDTOSCRate() / 4;
    /* Initialize WWDT (also enables WWDT clock) */
    Chip_WWDT_Init(LPC_WWDT);
    /* Set watchdog feed time constant to approximately 60s */
    Chip_WWDT_SetTimeOut(LPC_WWDT, wdtFreq * 60);
	/* Configure WWDT to reset on timeout */
	Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);
	/* Start watchdog */
	Chip_WWDT_Start(LPC_WWDT);


    // this call initializes debug uart for stdout redirection
	retarget_init();
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	Chip_RIT_Init(LPC_RITIMER);
    /* Enable and setup SysTick Timer at a periodic rate */
    SysTick_Config(SystemCoreClock / TICKRATE_HZ);

    /* configure MQTT and subscribe */
    MQTT mqtt(message_handler);
    mqtt.connect(SSID, PASSWORD, BROKER_IP, BROKER_PORT);
    mqtt.subscribe(MQTT_TOPIC_SETTING);


    /* configure LCD pinouts */
    DigitalIoPin *rs = new DigitalIoPin(0, 29, DigitalIoPin::output);
	DigitalIoPin *en = new DigitalIoPin(0, 9, DigitalIoPin::output);
	DigitalIoPin *d4 = new DigitalIoPin(0, 10, DigitalIoPin::output);
	DigitalIoPin *d5 = new DigitalIoPin(0, 16, DigitalIoPin::output);
	DigitalIoPin *d6 = new DigitalIoPin(1, 3, DigitalIoPin::output);
	DigitalIoPin *d7 = new DigitalIoPin(0, 0, DigitalIoPin::output);
	LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

	/* Configure buttons */
	DigitalIoPin sw1(1, 8, DigitalIoPin::pullup, true);
	DigitalIoPin sw2(0, 5, DigitalIoPin::pullup, true);
	DigitalIoPin sw3(0, 6, DigitalIoPin::pullup, true);
	DigitalIoPin sw4(0, 7, DigitalIoPin::pullup, true);

	// configure display geometry
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("LCD Ready!");

	/* GPIO interrupts and button init */
	DigitalIoPin::GPIO_Interrupt_init();
	sw1.enable_interrupt(0);
	sw2.enable_interrupt(1);
	sw3.enable_interrupt(2);

	/* modes for the control on LCD */
	std::string options[2] = { "Manual", "Auto" };

	modes = new StringEdit(&lcd, std::string("Mode"), options, 2);
	IntegerEdit *freq = new IntegerEdit(&lcd, std::string("Speed"), 0, 100, 2);
	IntegerEdit *pres = new IntegerEdit(&lcd, std::string("Pressure"), 0, 120, 2);
	IntegerEdit *t_pres = new IntegerEdit(&lcd, std::string("Target Pressure"), 0, 120, 2);
	IntegerEdit *t_freq = new IntegerEdit(&lcd, std::string("Target Speed"), 0, 100, 2);
	IntegerEdit *temp = new IntegerEdit(&lcd, std::string("Temperature"), 0, 100, 1);
	IntegerEdit *rh = new IntegerEdit(&lcd, std::string("RH"), 0, 100, 1);
	IntegerEdit *co2 = new IntegerEdit(&lcd, std::string("CO2"), 0, 1000, 10);

	/* Menu item objects */
	menu.addItem(new MenuItem(modes));
	menu.addItem(new MenuItem(freq));
	menu.addItem(new MenuItem(pres));
	menu.addItem(new MenuItem(t_pres));
	menu.addItem(new MenuItem(t_freq));
	menu.addItem(new MenuItem(temp));
	menu.addItem(new MenuItem(rh));
	menu.addItem(new MenuItem(co2));

	/*configure modbus transmission and initalize */
	/* this initializes comms with HMP60, GMP252, and the Produal MIO (fan) */
	modbusConfig modbus;

	/* LCD UI readings and control system config */
	modes->setValue(0);
	freq->setValue(0);
	modbus.set_speed(0);
	pres->setValue(0);
	t_pres->setValue(0);
	temp->setValue(0);
	rh->setValue(0);
	co2->setValue(0);

	int mqtt_status = 0;

	/* variables in publish */
	int nr = 0;
	int current_pressure = 0;
	int current_freq = 0;
	int spt_updated = 0;
	const char *m_sta[2] = { "false", "true" };
	const char *m_sta_mode[2] = { "false", "true" };

	/* jsmn-JSON config */
	jsmn_parser p;
	jsmntok_t tokens[256];

	/* main loop */
    while(1) {
		temp->setValue(modbus.get_temp());
		Sleep(100);
		pres->setValue((int) SDP_read());
		Sleep(100);
		co2->setValue(modbus.get_co2());
		Sleep(200);
		rh->setValue(modbus.get_rh());
		
    	current_pressure = pres->getValue();
    	current_freq = freq->getValue();

		if (IntegerEdit::saved_ == true) {
			if((menu.getPos() == 4) && (current_freq != t_freq->getValue())){
				modbus.set_speed(t_freq->getValue());
				Sleep(50);
				freq->setValue(t_freq->getValue());
			}
			IntegerEdit::saved_ = false;
		}

		/* read the mode setting and the set-value in this function */
		if(mqtt_message_arrived){
			jsmn_init(&p); 
			mqtt_message_arrived = false;
			//printf((mqtt_message + "\r\n").c_str());
			const char *mqtt_message_ = mqtt_message.c_str();
			jsmn_parse(&p, mqtt_message_, 100, tokens, 256);

			/* check for the mode from Web UI */
			jsmntok_t key = tokens[2]; /* the second token holds the mode (true/false) */
			unsigned int length = key.end - key.start;
			char set_mode[length + 1];
			memcpy(set_mode, &mqtt_message_[key.start], length);
			set_mode[length] = '\0';
			//printf("Key: %s\n", set_mode);

			/* read the set value */
			jsmntok_t key_ = tokens[4]; /* the 4th token holds the setpoint integer as a string */
			unsigned int length_ = key_.end - key_.start;
			char set_point[length_ + 1];
			memcpy(set_point, &mqtt_message_[key_.start], length_);
			set_point[length_] = '\0';
			//printf("Key: %s\n", set_point);

			/* convert the string into integer */
			spt_updated = std::stoi(set_point);

			if (strncmp("false", set_mode, 5) == 0) {
				modbus.set_speed(spt_updated*10);
				freq->setValue(spt_updated);
				t_freq->setValue(spt_updated);
				modes->setValue(0);
			}
			else if (strncmp("true", set_mode, 4) == 0) {
				t_pres->setValue(spt_updated);
				modes->setValue(1);
			}
			freq->setValue(current_freq*10);
			menu.event(MenuItem::show);
			memset(tokens, 0 , 256);
		}
#endif
		/* if in auto mode */
		if(modes->getValue() == 1){
			int tolerance = 2;
			if(abs(t_pres->getValue() - current_pressure) > tolerance){
				double diff = (t_pres->getValue() - current_pressure) / (double)120;
					signed int diff_ = (signed int)(diff * 50);				
					current_freq += diff_;
					
					/* limiting the values within the range */
					if(current_freq > 100) current_freq = 100;
					else if(current_freq < 0) current_freq = 0;
				}
			modbus.set_speed(current_freq*10);
			Sleep(500);
			freq->setValue(current_freq);
		}
		/* create a json object for publishing */ 
		std::string sample = sample_json(nr, freq->getValue(), spt_updated, pres->getValue(), m_sta_mode[modes->getValue()], m_sta[mqtt_status], modbus.get_co2(), modbus.get_rh(), modbus.get_temp());
		mqtt_status = mqtt.publish(MQTT_TOPIC_SEND, sample, sample.length());

		nr++;
		menu.event(MenuItem::show);
		//mqtt.yield(200);

		/* feed the WatchDog timer */
		Chip_WWDT_Feed(LPC_WWDT);
    }

    return 0 ;
}

/* function implementations */

/* read data from Sensirion SDP610 – 120Pa pressure sensor */
uint16_t SDP_read(){

	/* I2C objects and config */
	I2C_config cfg_i2c ;
	I2C i2c(cfg_i2c);

	uint8_t tx[1];
	uint8_t rx[3];
	int16_t pressure;

	tx[0]=SDP_CMD_READ;

	i2c.transaction(SDP_ADDRESS,tx,1,rx,3);

	// Combine read bytes
	uint16_t raw = (rx[0] << 8) | rx[1];

	// uint containing two's complement
	if (raw & 0x8000) {
		raw = -~(raw - 1);
	}
	pressure = raw;

	// Scaling
	pressure /= 240;

	// altitude correction
	pressure *= 0.95;

	return pressure;
}
void delay_systick(const int ticks){
	int delay = ticks;
	while(delay > 0){
		__WFI();
	}
}


void message_handler(MessageData* data)
{
	mqtt_message_arrived = true;
	mqtt_message = "";

	// Parse message from payload
	char payload_parsed[READ_BUF_LENGTH];
	snprintf(
			payload_parsed,
			data->message->payloadlen + 1,
			"%.*s",
			data->message->payloadlen,
			(char *)data->message->payload
	);
	mqtt_message = payload_parsed;
}

std::string sample_json(int sample_number, int speed, int setpoint,int pressure, const char* mode, const char* error, int co2, int rh, int temp)
{

	nlohmann::json sample;
	sample["nr"] = sample_number;
	sample["speed"] = speed;
	sample["setpoint"] = setpoint;
	sample["pressure"] = pressure;
	sample["auto"] = mode;
	sample["error"] = error;
	sample["co2"] = co2;
	sample["rh"] = rh;
	sample["temp"] = temp;


	return sample.dump();
}
