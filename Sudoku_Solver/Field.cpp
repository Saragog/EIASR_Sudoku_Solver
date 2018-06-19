#include "stdafx.h"
#include "Field.h"


Field::Field()
{
	initializePossibilities();
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
	this->isFieldValueSet = true;
	this->value = value;
}

bool Field::isThereOnePossibility()
{
	bool onePossibilityFound = false;
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
	{
		if (possibilities[possibilityIndex])
			if (!onePossibilityFound) onePossibilityFound = true;
			return false;
	}
	return onePossibilityFound;
}

bool Field::isThereAnyPossibility()
{
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
	{
		if (possibilities[possibilityIndex]) return true;
	}
	return false;
}

int Field::findPossibility()
{
	for (int possibilityIndex = 0; possibilityIndex < 9; possibilityIndex++)
		if (possibilities[possibilityIndex]) return possibilityIndex+1;
	return 0;
}

void Field::setTheOnlyPossibility()
{
	if (isThereOnePossibility())
		value = findPossibility();
}

void Field::removePossibility(int value)
{
	possibilities[value - 1] = false;
}
