/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 *      Modified: Manish
 */

#include "IntegerEdit.h"
#include <cstdio>

std::atomic<bool> IntegerEdit::saved_{false};

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, std::string editTitle, int minVal, int maxVal, int step): lcd(lcd_), title(editTitle), minVal(minVal), maxVal(maxVal), step(step) {
	value = minVal;
	edit = minVal;
	focus = false;
}

IntegerEdit::~IntegerEdit() {
}

void IntegerEdit::increment() {
	edit += step;
	if(edit > maxVal) edit = maxVal;
}


void IntegerEdit::decrement() {
	edit -= step;
	if(edit < minVal) edit = minVal;
}

void IntegerEdit::accept() {
	save();
	saved_ = true;
}

void IntegerEdit::cancel() {
	edit = value;
}


void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	return this->focus;
}

void IntegerEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%4d]     ", edit);
	}
	else {
		snprintf(s, 17, "      %4d      ", edit);
	}
	lcd->print(s);
}


void IntegerEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int IntegerEdit::getValue() {
	return value;
}

void IntegerEdit::setValue(int value) {
	edit = value;
	save();
}

