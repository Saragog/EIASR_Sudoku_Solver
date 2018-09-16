#include "stdafx.h"
#include "Field.h"

#include <iostream>

Field::Field()
{
	initializePossibilities();
	this->value = -1;
	fieldValueSet = false;
}

Field::Field(int value)
{
	initializePossibilities();
	removePossibility(value);
	this->value = value;
	fieldValueSet = true;
}

Field::~Field()
{
}

void Field::initializePossibilities()
{
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
		possibilities[possibilityIndex] = true;
}

bool Field::isFieldValueSet()
{
	return fieldValueSet;
}

void Field::setValue(int value)
{
	this->fieldValueSet = true;
	this->value = value;
}

bool Field::isThereOnePossibility()
{
	bool onePossibilityFound = false;
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
	{
		if (possibilities[possibilityIndex] == true)
		{
			if (!onePossibilityFound) onePossibilityFound = true;
			else return false;
		}
	}
	return onePossibilityFound;
}

bool Field::isThereAnyPossibility()
{
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
	{
		if (possibilities[possibilityIndex] == true) return true;
	}
	return false;
}

int Field::findPossibility()
{
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
		if (possibilities[possibilityIndex] == true) return possibilityIndex+1;
	return 0;
}

void Field::setTheOnlyPossibility()
{
	if (isThereOnePossibility())
		value = findPossibility();
}

void Field::removePossibility(int value)
{
	std::cout << "Value: " << value << "\n";
	possibilities[value-1] = false;
}

int Field::getValue()
{
	return value;
}
