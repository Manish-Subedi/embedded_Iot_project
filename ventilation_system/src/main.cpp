/*
===============================================================================
 Name        : main.c
 Author      : Manish Subedi
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "MainHeader.h"

// TODO: insert other include files here

// TODO: insert other definitions and declarations here


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
#if 1
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

		printf((std::string("mode value: ") + std::to_string(modes->getValue()) + "\n").c_str());
		if(menu.getPos() == 0) menu.event(MenuItem::ok);
		//check if mode is auto
		if(modes->getValue() == 0){
			//allow setting pressure
			if(menu.getPos() == 3) menu.event(MenuItem::ok);
		}
		if(modes->getValue() == 1){ //check if mode is manual
			if(menu.getPos() == 4) menu.event(MenuItem::ok); //allow only set-frequency
		}
	}
	void PIN_INT2_IRQHandler(void){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));

		// make sure up is only executed once on button debounce
		if (millis() - button_pressed_time < MAX_DEBOUNCE) return;
		button_pressed_time = millis();

		menu.event(MenuItem::down);
	}
	void PIN_INT3_IRQHandler(void){
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(3));

		// make sure up is only executed once on button debounce
		if (millis() - button_pressed_time < MAX_DEBOUNCE) return;
		button_pressed_time = millis();

		menu.event(MenuItem::back);
	}
#endif


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
//void message_handler(MessageData*);


int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // this call initializes debug uart for stdout redirection
	retarget_init();
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

    /* Enable and setup SysTick Timer at a periodic rate */
    SysTick_Config(SystemCoreClock / TICKRATE_HZ);

    printf("\nBoot\n");
    // TODO: insert code here

    Chip_RIT_Init(LPC_RITIMER);

    /* configure LCD pinouts */
	DigitalIoPin rs(0, 29, false, false, false);
	DigitalIoPin en(0, 9, false, false, false);
	DigitalIoPin d4(0, 10, false, false, false);
	DigitalIoPin d5(0, 16, false, false, false);
	DigitalIoPin d6(1, 3, false, false, false);
	DigitalIoPin d7(0, 0, false, false, false);

	LiquidCrystal *lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);

	// configure display geometry
	lcd->begin(16, 2);
	lcd->setCursor(0, 0);
	lcd->print("LCD Ready!");

#if 0

	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg_d = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };

	LpcUart dbgu(cfg_d);

	dbgu.write("\n\rHello World!\r\n"); //testing debug port
#endif

	std::string options[2] = { "Auto", "Manual" };
	modes = new ModeEdit(lcd, std::string("Mode"), options, 2);
	IntegerEdit *freq = new IntegerEdit(lcd, std::string("Frequency"), 0, 1000, 50);
	IntegerEdit *pres = new IntegerEdit(lcd, std::string("Pressure"), 0, 120, 5);
	IntegerEdit *t_pres = new IntegerEdit(lcd, std::string("Target Pressure"), 0, 120, 5 );
	IntegerEdit *t_freq = new IntegerEdit(lcd, std::string("Target Freq"), 0, 1000, 50);
	IntegerEdit *temp = new IntegerEdit(lcd, std::string("Temperature"), 0, 0, 0);
	IntegerEdit *rh = new IntegerEdit(lcd, std::string("RH"), 0, 0, 0);
	IntegerEdit *co2 = new IntegerEdit(lcd, std::string("CO2"), 0, 0, 0);


	menu.addItem(new MenuItem(modes));
	menu.addItem(new MenuItem(freq));
	menu.addItem(new MenuItem(pres));
	menu.addItem(new MenuItem(t_pres));
	menu.addItem(new MenuItem(t_freq));
	menu.addItem(new MenuItem(temp));
	menu.addItem(new MenuItem(rh));
	menu.addItem(new MenuItem(co2));


	//MenuItem items_[MENU_SIZE] = { modes, freq, pres, t_fr, t_pr };

	menu.event(MenuItem::show);

	/* Modbus node for fan */
	ModbusMaster node_fan(1); // Create modbus object that connects to slave id 1
	node_fan.begin(9600); // set transmission rate - other parameters are set inside the object and can't be changed here
	ModbusRegister freq_fan(&node_fan, 0);
	ModbusRegister pulse_fan(&node_fan, 4, false);


	/* Modbus node for GMP252 co2 sensor */
	ModbusMaster node_gmp(240);
	node_gmp.begin(9600);
	ModbusRegister co2_(&node_gmp, 0x0101, false);


	/* Modbus node for HMP60 temperature & humidity sensor */
	ModbusMaster node_hmp(241);
	node_hmp.begin(9600);
	ModbusRegister rh_(&node_hmp, 0x0100, false);
	ModbusRegister temp_(&node_hmp, 0x0101, false);

	/* LCD UI readings and control system config */
	modes->setValue(0);
	freq->setValue(0);
	freq_fan.write((freq->getValue())); //instantly set fan speed
	Sleep(1000); // let read the pressure
	pres->setValue((int) SDP_read());
	t_pres->setValue(0);
	temp->setValue(temp_.read());
	rh->setValue(rh_.read());
	co2->setValue(co2_.read());


	char buffer[130];

	//MqttController mqtt(message_handler);
	//mqtt.Mqtt_start(SSID, PASSWORD, BROKER_IP, BROKER_PORT);


	/* variables in publish */
	int nr = 0;
	int current_pressure = 0;
	int current_t_freq = 0;
	int current_freq = 0;

    // Enter an infinite loop, just incrementing a counter
    while(1) {
		pres->setValue((int) SDP_read());

		temp->setValue(temp_.read());
		rh->setValue(rh_.read());
		co2->setValue(co2_.read());

    	current_pressure = (int) SDP_read();
    	current_freq = freq->getValue();
    	current_t_freq = t_freq->getValue();

		if (IntegerEdit::saved_ == true || ModeEdit::saved_ == true) {
			//if set pressure
			if(menu.getPos() == 4){
				if(current_freq != t_freq->getValue()){
					freq->setValue(t_freq->getValue());
					freq_fan.write(t_freq->getValue());
					Sleep(2000);
					pres->setValue((int) SDP_read());
				}
			}
			else if(menu.getPos() == 3)  {
				if(t_pres->getValue() < current_pressure){
						while(((int) SDP_read()) > t_pres->getValue()){
							current_freq-=50;
							freq_fan.write(current_freq);
							Sleep(400);
						}
						freq->setValue(current_freq);
					}
					//if
				else if(t_pres->getValue() > current_pressure){
						while(((int) SDP_read()) < t_pres->getValue()){
							current_freq+=50;
							freq_fan.write(current_freq);
							Sleep(400);
						}
						freq->setValue(current_freq);
					}
			}
			Sleep(1000);

			//snprintf(buffer, 100, "\nMode(0 - auto, 1 - manual): %d\n\rFreq: %d\r\nPressure: %d\r\n", modes->getValue(), freq->getValue(), SDP_read());
			//dbgu.write(buffer);
			printf("Mode(0 - auto, 1 - manual): %d\nFreq: %d\nPressure: %d\n", modes->getValue(), freq->getValue(), SDP_read());
			//snprintf(buffer, 100, "\n\rtemp %d, rh %d, co2 %d, pulse %d\r\n", temp.read(), rh.read(), co2.read(), pulse_fan.read());
			//dbgu.write(buffer);
			//snprintf(buffer, 150, "{\r\n\"nr\": %d,\r\n\"speed\": %d,\r\n\"setpoint\": %d,\r\n\"pressure\": %d,\r\n\"auto\": %s,\r\n\"error\": %s,\r\n\"co2\": %d,\r\n\"rh\": %d,\r\n\"temp\": %d\r\n}", nr, freq->getValue(), t_fr->getValue(), pres->getValue(), "false", "false", co2.read(), rh.read(), temp.read());
			//dbgu.write(buffer);
			//snprintf(buffer, 150, "{\n\"nr\": %d,\n\"speed\": %d,\n\"setpoint\": %d,\n\"pressure\": %d,\n\"auto\": %s,\n\"error\": %s,\n\"co2\": %d,\n\"rh\": %d,\n\"temp\": %d\n}", nr, freq->getValue(), t_fr->getValue(), pres->getValue(), "false", "false", co2.read(), rh.read(), temp.read());

			IntegerEdit::saved_ = false;
			ModeEdit::saved_ = false;
		}
		//snprintf(buffer, 130, "{\n\"nr\": %d,\n\"speed\": %d,\n\"setpoint\": %d,\n\"pressure\": %d,\n\"auto\": %s,\n\"error\": %s,\n\"co2\": %d,\n\"rh\": %d,\n\"temp\": %d\n}", nr, freq->getValue(), t_fr->getValue(), pres->getValue(), "false", "false", co2.read(), rh.read(), temp.read());

		//mqtt.publish(MQTT_TOPIC_SEND, buffer);
		nr++;
		//if(t_count >= 100) menu.event(MenuItem::back);
		Sleep(100);
		menu.event(MenuItem::show);
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

	// uint containing two's complement -> int16
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
/*
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
*/
