#include "stdafx.h"
#include "PossibilitiesForAField.h"


PossibilitiesForAField::PossibilitiesForAField()
{
	for (int valueIndex = 0; valueIndex < 9; valueIndex++)
		possibleValues[valueIndex] = false;
}


PossibilitiesForAField::~PossibilitiesForAField()
{

}



bool PossibilitiesForAField::isValuePossible(int value)
{
	if (possibleValues[value - 1] == true) return true;
	return false;
}

void PossibilitiesForAField::setPossibilityForAField(int value)
{
	possibleValues[value - 1] = true;
}