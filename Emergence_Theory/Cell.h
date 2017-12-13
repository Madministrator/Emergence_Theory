#pragma once
class Cell
{
private:
	//Variables needed to determine what type this cell should be
	static const int THRESHOLD = 10;
	static const int DIFFERENCE = 4;
	int type_record[5] = { 0,0,0,0,0 };
	//The type number of THIS cell
	int myType;
	/*
		Ran if one index exceeds the THRESHOLD constant
		will change the type of this cell if needed.
	*/
	void consult_record();
	void wipeRecord();

public:
	//The constructor should give the cell a random type when it is "born"
	Cell();
	~Cell();

	void collision(Cell other);
	void collideWith(int type); //Should only be called from another cell
	int getType() const;
};

