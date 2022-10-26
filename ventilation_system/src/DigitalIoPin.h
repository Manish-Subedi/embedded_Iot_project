/*
 * DigitalIoPin.h
 *
 *  Created on: 7 Sep 2022
 *      Author: Manish Subedi
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

class DigitalIoPin {
	public:
		DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert);
		DigitalIoPin(int port, int pin, bool input);
		DigitalIoPin(const DigitalIoPin &) = delete;
		virtual ~DigitalIoPin();
		bool read();
		void write(bool value);
	private:

		int port, pin;
		bool input, pullup, invert;
};

#endif /* DIGITALIOPIN_H_ */
