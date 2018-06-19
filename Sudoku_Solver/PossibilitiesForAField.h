#pragma once
class PossibilitiesForAField
{
public:
	PossibilitiesForAField();
	~PossibilitiesForAField();

	bool isValuePossible(int value);
	void removePossibilityForAField(int value);
	void removeManyPossibilitiesForAField(PossibilitiesForAField& usedPossibilities);
private:
	bool possibleValues[9];

};

