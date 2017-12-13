// EmergenceTheory.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Cell.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

int promptSeed();
void runCollisions(vector<Cell> &colony);
void printData(vector<Cell> &colony, int passes);
bool checkSucceeding(vector<Cell> &colony);
bool cataclysm(vector<Cell> &colony);

int main()
{
	//This constant is the maximum size of the colony, ensures that I can't break the vector class
	const int CARRYING_CAPACITY = 20000; // This will take a lot of CPU anyway
	//Declaring Variables for colony
	srand(time(NULL));
	vector<Cell> cells(promptSeed()); //The cells constructors are called in this statement
	int passes = 0;
	//printing out the initial set up:
	printData(cells, passes);

	bool succeeding = true; //no type is below 5% of the whole colony.

	//Declaring Variables for timer
	clock_t start, end;
	double time_elapsed; // = (end - start) / CLOCKS_PER_SEC;
	start = clock();

	while (succeeding)
	{
		runCollisions(cells);
		//If ten seconds has passed
		end = clock();
		time_elapsed = (end - start) / CLOCKS_PER_SEC;

		if (time_elapsed > 10)
		{
			printData(cells, passes);
			start = clock(); //Resets the timer
		}

		//adds a new cell to the colony:
		if (cells.size() < CARRYING_CAPACITY)
		{
			cells.push_back(Cell());
		}

		succeeding = checkSucceeding(cells);
		if (cataclysm(cells))
			printData(cells, passes);
		passes++;
	}
	
	//Colony has failed
	cout << "Colony failed after " << passes << " passes" << endl << "Final ";
	printData(cells, passes);
	cout << endl;

	system("pause");
    return 0;
}

int promptSeed()
{
	int seed;
	cout << "Create a seed (starting quantity of Cell colony): ";
	cin >> seed;
	while (seed < 1)//input validation
	{
		cout << "Enter a value greater than zero: ";
		cin >> seed;
	}
	system("CLS"); //Clears the screen cause I can
	return seed;
}

///Makes every cell in the vector collide with other cells. This is a separate function to clean up main()
void runCollisions(vector<Cell> &colony)
{
	//Initializing random number engine for collisions
	default_random_engine collide(time(NULL));
	//Generating a uniform int distribution so a cell has an equal chance of colliding with any other cell
	uniform_int_distribution<int> contact(0, colony.size() - 1);
	
	//For loop creates collisions
	for (int i = 0; i < colony.size(); i++)
	{
		//Every cell will collide with three other random cells
		//Consider changing the 3 to see what happens
		for (int x = 0; x < 3; x++)
		{
			int otherCell = contact(collide);
			while (otherCell == i)//Can't collide with itself
				otherCell = contact(collide);
			colony[i].collision(colony[otherCell]); //I hate this syntax, but it works.
		}
	}
}

///Prints a simple statistical analysis of the colony
void printData(vector<Cell> &colony, int passes)
{
	int type_record[5] = { 0,0,0,0,0 };

	for (int i = 0; i < colony.size(); i++)
	{
		int type = colony[i].getType();
		type_record[type]++;
	}

	cout << fixed << showpoint << setprecision(1);
	cout << "Distribution of cell types: " << "Passes = " << passes
		<< endl << "Size of colony: " << colony.size() << endl;
	for (int i = 0; i < 5; i++)
	{
		double percentage = ((static_cast<double>(type_record[i]) / colony.size()) * 100);
		cout << "Type " << i << ":\t" << type_record[i] << "\t" << percentage << "%" << endl;
	}
	//system("pause");
	cout << endl << endl;
}

bool checkSucceeding(vector<Cell> &colony)
{
	bool succeeding = true;

	int type_record[5] = { 0,0,0,0,0 };

	for (int i = 0; i < colony.size(); i++)
	{
		int type = colony[i].getType();
		type_record[type]++;
	}

	for (int i = 0; i < 5; i++)
	{
		double percentage = ((static_cast<double>(type_record[i]) / colony.size()) * 100);
		if (percentage < 5.0)
			succeeding = false;
	}
	return succeeding;
}

///Returns true if colony was changed, false otherwise
bool cataclysm(vector<Cell> &colony)
{
	/*
		When a new cell is added each round, the simulation of the colony 
		creates a system in which the distribution of cell types is relatively
		equal (true equality is impossible in a pseudo-random system).

		So, now I need to throw in a wrench in the system and see what happens.
		To do so, I will have a few steps:
		1. Determine if there will be a cataclysmic event via randomness
		2. Determine which type(s) will be effected
		3. Determine how badly the type(s) will be effected
		3. Find the index of all the cells of a certain type (Stored in vector)
		4. Use the vector.erase(index) method to erase of a percentage of those cells.
		5. call printData() to set a baseline for observations.
	*/

	int probability = rand() % 999 + 1; // Range: (1-1000) The chance of a cataclysm, determined by the following if statement
	if (probability == 1) // 0.1% chance of cataclysm | I might change this to a switch-case for fun later
	{
		int type_to_erase = rand() % 5; //Chooses which type will be reduced
		int severity = rand() % (5 - 2 + 1) + 2; 
		/*
		Here's how severity works:
		severity is a limiter, I can't kill a whole type off, that would end the colony instantly.
		So, the number quantity_to_erase is a limiter, the greater the limiter, the less cells are erased
		if I get a 2, half are erased. If I get a 3, a third are erased. and so on.
		*/

		//Variables to track damage to the colony
		int initial_size = colony.size(); //The initial population of the whole colony
		int type_size = 0; //The initial population of the type affected
		int erased = 0; //counter of successful erasure

		//counting the affected type
		for (int i = 0; i < colony.size(); i++)
			if (colony[i].getType() == type_to_erase)
				type_size++;
				
		//Actually erasing the data - needs the type quantity to work
		for (int index = 0; erased < (type_size / severity) && index < colony.size(); index++)
		{
			if (colony[index].getType() == type_to_erase)
			{
				colony.erase(colony.begin() + index);
				erased++;
				index--; //So I don't skip over values that fill the empty index after erasure
			}
		}

		//Displaying what happened to the data
		cout << "Cataclym occured - Type Affected: " << type_to_erase << endl
			<< "Initial size of colony: " << initial_size << endl
			<< "Initial Quantity of type affected: " << type_size << endl
			<< "Quantity erased of type " << type_to_erase << ": " << erased << endl;

		return true;
	}
	
	return false;
}