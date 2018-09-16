#pragma once
class Field
{
public:
	Field();
	Field(int);
	~Field();
	bool isFieldValueSet();		  // says whether the field is already set
	void setValue(int);			  // sets the value of a field
	bool isThereOnePossibility(); // checks if there is only 1 possibility for the field
	bool isThereAnyPossibility(); // if false is returned then it means that the problem cannot be solved
	void setTheOnlyPossibility(); // sets the value of a field to the only existing possiblity
	void removePossibility(int);  // removes a possibility
	int getValue();
private:
	bool fieldValueSet;
	int value;
	bool possibilities[9];

	void initializePossibilities();
	int findPossibility();
};

