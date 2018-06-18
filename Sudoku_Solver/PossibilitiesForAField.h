#pragma once
class PossibilitiesForAField
{
public:
	PossibilitiesForAField();
	~PossibilitiesForAField();

	bool isValuePossible(int value);
	void setPossibilityForAField(int value);
private:
	bool possibleValues[9];

};

