#include "stdafx.h"
#include "Cell.h"
#include <ctime>
#include <cstdlib>


Cell::Cell()
{
	//srand(time(0));
	myType = rand() % 5; //Range: [0,4] - the indexes of type_record
}
Cell::~Cell() {}

/*
	This will be called at the end of collideWith() and collideFrom()

	Cell will determine if it needs to change it's type.
	If a type in the record array is greater than the threshold,
	then it needs to make a second check. Else wipeRecord() and end.

	the Second check compares the greatest value and the smallest value.
	IF the difference between the two is greater than the DIFFERENCE 
	constant, then the cell is converted into the same type as 
	the min value. 

*/
void Cell::consult_record()
{
	//Declaring variables
	int min_index;//I don't need the max index to determine what type to change to
	int max, min; //Quantity of occurances

	//Finding the max and min, for loop is coded so that the array can change size
	for (int i = 0; i < (sizeof(type_record) / sizeof(type_record[0])); i++)
	{
		if (i == 0) // only runs on first iteration
		{
			max = type_record[i];
			min = type_record[i];
			min_index = i;
		}

		if (type_record[i] > max)
			max = type_record[i];
		if (type_record[i] < min)
		{
			min = type_record[i];
			min_index = i;
		}
	}

	if (max >= THRESHOLD)
	{
		if ((max - min) > DIFFERENCE)
			myType = min_index;
		wipeRecord();
	}

}
//Reset all values of type_record to zero
void Cell::wipeRecord()
{
	for (int i = 0; i < (sizeof(type_record) / sizeof(type_record[0])); i++)
		type_record[i] = 0;
}

void Cell::collision(Cell other)
{
	other.collideWith(myType); //other cell must register collision

	type_record[other.getType()]++;
	consult_record();
}

/*
	This function recieves a collision from another cell.
	This function should only be called within the collision(),
	and is not meant for use from within the main() function.

	The parameter is the type of the cell it recieved a collision
	from.

	if the parameter is not within the range of the index of type_record,
	then it will return a -1. Otherwise, it returns it's own cell type
*/
void Cell::collideWith(int type)
{
	if (type < 0 || type >= (sizeof(type_record) / sizeof(type_record[0])))
		return; //kills function because of invalid input

	type_record[type]++; //Records the collision if it's valid
	consult_record();
}

int Cell::getType() const 
{ 
	return myType; 
}