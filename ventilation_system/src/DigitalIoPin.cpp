/*
 * DigitalIOPin.cpp
 *
 *  Created on: 7 Sep 2022
 *      Author: Manish
 */

#include "DigitalIoPin.h"

#include "chip.h"
#include "board.h"

DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert):
																					port(port),
																					pin(pin),
																					input(input),
																					pullup(pullup),
																					invert(invert)
																					{
	// TODO Auto-generated constructor stub
	if(input == true && pullup == true && invert == true){
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	if(input == true && pullup == true && invert == false){
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
		Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
		}
	if(input == false && pullup == false && invert == false){
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_MODE_INACT | IOCON_DIGMODE_EN);
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}


}

DigitalIoPin::DigitalIoPin(int port, int pin, bool input): port(port), pin(pin), input(input){
	if(input){

	}
}
bool DigitalIoPin::read(){
	bool state = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
	return state;
}
void DigitalIoPin::write(bool state){
	Chip_GPIO_SetPinState(LPC_GPIO, port, pin, state);
}

DigitalIoPin::~DigitalIoPin() {
	// TODO Auto-generated destructor stub

}

