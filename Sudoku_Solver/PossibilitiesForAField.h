#pragma once
class PossibilitiesForAField
{
public:
	PossibilitiesForAField();
	~PossibilitiesForAField();

	bool isValuePossible(int value);
	void removePossibilityForAField(int value);
	void removeManyPossibilitiesForAField(PossibilitiesForAField& usedPossibilities);
	void resetPossibilities();

	// FOR TESTING 
	void printPossibilities();

private:
	bool possibleValues[9];

};

