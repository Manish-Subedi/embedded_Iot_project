/*
 * ModeEdit.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: Manish
 */

#include "ModeEdit.h"
#include <cstdio>
#include <string>
#include <atomic>

std::atomic<bool> ModeEdit::saved_{false};

ModeEdit::ModeEdit(LiquidCrystal *lcd_, std::string editTitle, std::string* modes, int k):
lcd(lcd_), title(editTitle), modes(modes), length(k)
{
	value = 0;
	edit = 0;
	focus = false;
}

ModeEdit::~ModeEdit() {
	// TODO Auto-generated destructor stub
}

void ModeEdit::increment() {
	edit++;
	if(edit > length -1) edit = 0;
}

void ModeEdit::decrement() {
	edit--;
	if(edit < 0) edit = length -1;
}

void ModeEdit::accept() {
	save();
	saved_ = true;
}

void ModeEdit::cancel() {
	edit = value;
}


void ModeEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool ModeEdit::getFocus() {
	return this->focus;
}

void ModeEdit::display() {
	lcd->clear();
	lcd->print(title);
	char s[17];
	if(focus) {
		snprintf(s, 17, "  [%6s]   ", &(modes[edit][0]));
	}
	else {
		snprintf(s, 17, "   %6s    ", &(modes[edit][0]));
	}
	lcd->print(s);
}


void ModeEdit::save() {
	// set current value to be same as edit value
	value = edit;
	// todo: save current value for example to EEPROM for permanent storage
}

int ModeEdit::getValue() {
	return value;
}

void ModeEdit::setValue(int value) {
	edit = value;
	save();
}

