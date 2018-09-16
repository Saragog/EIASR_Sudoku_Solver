#include "stdafx.h"
#include "PossibilitiesForAField.h"

#include <iostream>

PossibilitiesForAField::PossibilitiesForAField()
{
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		possibleValues[valueIndex] = true;
}


PossibilitiesForAField::~PossibilitiesForAField()
{

}

bool PossibilitiesForAField::isValuePossible(int value)
{
	return possibleValues[value - 1];
}

void PossibilitiesForAField::removePossibilityForAField(int value)
{
	possibleValues[value - 1] = false;
}

void PossibilitiesForAField::removeManyPossibilitiesForAField(PossibilitiesForAField & usedPossibilities)
{
	for (int possibility = 1; possibility <= 9; possibility++)
	{
		if (usedPossibilities.isValuePossible(possibility) == false)
			removePossibilityForAField(possibility);
	}
}

void PossibilitiesForAField::printPossibilities()
{
	std::cout << "Wypisuje mozliwosci:\n";
	for (int possibility = 1; possibility <= 9; possibility++)
	{
		if (isValuePossible(possibility))
			std::cout << possibility;
	}
}

void PossibilitiesForAField::resetPossibilities()
{
	std::cout << "Resetuje mozliwosci\n";
	for (int possibility = 0; possibility < 9; possibility++)
	{
		possibleValues[possibility] = true;
	}
}