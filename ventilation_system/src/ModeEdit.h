/*
 * ModeEdit.h
 *
 *  Created on: 20 Oct 2022
 *      Author: Manish
 */

#ifndef SRC_MODEEDIT_H_
#define SRC_MODEEDIT_H_
#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>
#include <atomic>

class ModeEdit: public PropertyEdit {
public:
	ModeEdit(LiquidCrystal *lcd_, std::string editTitle, std::string* modes, int k);
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	int getValue();
	void setValue(int index);
	virtual ~ModeEdit();
	static std::atomic<bool> saved_;
private:
	void save();
	void displayEditValue();

	LiquidCrystal *lcd;
	std::string title;
	std::string* modes;
	int value;
	int length;
	int edit;
	bool focus;
};

#endif /* SRC_MODEEDIT_H_ */
