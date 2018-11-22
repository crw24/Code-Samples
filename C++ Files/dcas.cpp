//This program was written by Christian Wilson
//This contains the implementation of the Drone class
#include "dcas.h"
#include <iostream>
#include <string>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

char map [40*40] = { 0 };												//The Grid
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;						//initializer for condition variables
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
int threadIdent = 0;
int waiting = 0;
int allDeliveriesCount = 10;

//Drone constructor
Drone::Drone(char name, char destination, int destRow1, int destCol1, int destRow2, int destCol2, int numDeliveries, string startState)
{
	droneName = name;													//sets the droneName to the passed in character
	droneDestination = destination;										//sets the destination character to the passed in character
	currentLocation[0] = 0;												//sets the current location to row 0 column 0, the location of the airport
	currentLocation[1] = 0;									
	deliveriesToMake = numDeliveries;									//sets number of deliveries the drone will make
	state = startState;													//sets start state
	destinationLocation[0] = destRow1;									//sets destinations into array				
	destinationLocation[1] = destCol1;
	destinationLocation[2] = destRow2;
	destinationLocation[3] = destCol2;
}

//Function returns deliveriesToMake variable
int Drone::getDeliveries()
{
	return deliveriesToMake;
}

//function returns state variable
string Drone::getState()
{
	return state;
}

//function determines the destination name
char Drone::getDest()
{
	if (threadIdent == 1)				//if the number of threads so far is 1, returns 'a' for destination name
		return 'a';
	else if (threadIdent == 2)			//if number of threads is 2, returns 'b' for destination name
		return 'b';
	else if (threadIdent == 3)			//if number of threads is 3, returns 'c' for destination name
		return 'c';
	else if (threadIdent == 4)			//if number of threads is 4, returns 'd' for destination name
		return 'd';
	else if (threadIdent == 5)			//if number of threads is 5, returns 'e' for destination name
		return 'e';
	else if (threadIdent == 6)			//if number of threads is 6, returns 'f' for destination name
		return 'f';
	else if (threadIdent == 7)			//if number of threads is 7, returns 'g' for destination name
		return 'g';
	else if (threadIdent == 8)			//if number of threads is 8, returns 'h' for destination name
		return 'h';
	else if (threadIdent == 9)			//if number of threads is 9, returns 'i' for destination name
		return 'i';
	else if (threadIdent == 10)			//if number of threads is 10, returns 'j' for destination name
		return 'j';			
}

//Function determines the name of the drone
char Drone::getName()
{
	if (threadIdent == 1)			//if number of threads is 1, returns 'A' for drone name
		return 'A';
	else if (threadIdent == 2)		//if number of threads is 2, returns 'B' for drone name
		return 'B';
	else if (threadIdent == 3)		//if number of threads is 3, returns 'C' for drone name
		return 'C';
	else if (threadIdent == 4)		//if number of threads is 4, returns 'D' for drone name
		return 'D';
	else if (threadIdent == 5)		//if number of threads is 5, returns 'E' for drone name
		return 'E';
	else if (threadIdent == 6)		//if number of threads is 6, returns 'F' for drone name
		return 'F';
	else if (threadIdent == 7)		//if number of threads is 7, returns 'G' for drone name
		return 'G';
	else if (threadIdent == 8)		//if number of threads is 8, returns 'H' for drone name
		return 'H';
	else if (threadIdent == 9)		//if number of threads is 9, returns 'I' for drone name
		return 'I';
	else if (threadIdent == 10)		//if number of threads is 10, returns 'J' for drone name
		return 'J';
}

//Function to handle taking off and assigning deliveries to a drone for state "TKE"
void Drone::takeoff()
{
//	if (deliveriesToMake == 1)															//if only one delivery is left, loads its location into the 1st 2 elements of the location array
//	{
//		destinationLocation[0] = destinationLocation[2];
//		destinationLocation[1] = destinationLocation[3];
//	}
	if(pthread_mutex_trylock(&mtx[0]) == 0)
	{
		previousNode = 'P';																//sets previous node to 'P' for port
		map[0] = droneName;																//writes droneName character onto the map at landing location
		map[(destinationLocation[0] * 40) + destinationLocation[1]] = droneDestination;	//writes destination name onto map at destination location
		state = "CRU";																	//sets state to CRU for cruise
	}
}

//Function handles drone cruising to delivery for state "CRU"
void Drone::cruise()
{
	if (destinationLocation[0] > currentLocation[0] && pthread_mutex_trylock(&mtx[((currentLocation[0]+1)*40) + currentLocation[1]]) == 0)				//if current row is lower than destination row, moves drone up (numerically) a row
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;										//places previous node character back on map
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);
		currentLocation[0]++;																					//increments current row
	

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];										//sets previousNode to character at current location
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;										//sets node at current location to droneName
		
		if (currentLocation[0] == destinationLocation[0] && currentLocation[1] == destinationLocation[1])		
			state = "DLV";																						//if drone at destination, sets state to DLV for delivering
	}

	else if (destinationLocation[1] > currentLocation[1] && pthread_mutex_trylock(&mtx[(currentLocation[0]*40) + (currentLocation[1] + 1)]) == 0)		//if current column is lower than destination row, moves drone right a column
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);		
		currentLocation[1]++;																					//increments current column

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];									
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;

		if (currentLocation[0] == destinationLocation[0] && currentLocation[1] == destinationLocation[1])
			state = "DLV";
	}

	else if (destinationLocation[0] < currentLocation[0] && pthread_mutex_trylock(&mtx[((currentLocation[0]-1)*40) + currentLocation[1]]) == 0)			//if current row is higher than destination row, moves drone down (numerically) a row
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);		
		currentLocation[0]--;																					//decrements current row

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;

		if (currentLocation[0] == destinationLocation[0] && currentLocation[1] == destinationLocation[1])
			state = "DLV";
	}

	else if (destinationLocation[1] < currentLocation[1] && pthread_mutex_trylock(&mtx[(currentLocation[0]*40) + (currentLocation[1]-1)]) == 0)			//if current column is higher than destination row, moves drone left a column
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);		
		currentLocation[1]--;																					//decrements current column

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;

		if (currentLocation[0] == destinationLocation[0] && currentLocation[1] == destinationLocation[1])
			state = "DLV";
	}
}

//Function handles delivering package for state "DLV"
void Drone::deliver()
{
	previousNode = '*';					//sets previous node to '*' from the destination character
	state = "RTN";						//sets state to "RTN" for returnToPort
}

//Function handles returning to the port for state "RTN"
void Drone::returnToPort()
{
	if (currentLocation[0] > 0 && pthread_mutex_trylock(&mtx[((currentLocation[0] - 1)*40) + currentLocation[1]]) == 0)			//if current row is higher (numerically) than 0 (port row)
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;														//replace previousNode character on map
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);		
		currentLocation[0]--;																									//decrement current row

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];														//set previousNode to new currentLocation node character 
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;														//put droneName character on map at currentLocation

		if (currentLocation[0] == 0 && currentLocation[1] == 0)																	//if current location is 0,0 (Port)
			state = "LND";																										//sets state to "LND" for landing
	}

	else if (currentLocation[1] > 0 && pthread_mutex_trylock(&mtx[(currentLocation[0]*40) + (currentLocation[1]-1)]) == 0)		//if current column is higher (numerically) than 0 (port column)
	{
		map[(currentLocation[0] * 40) + currentLocation[1]] = previousNode;		
		pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);		
		currentLocation[1]--;																									//decrements current column

		previousNode = map[(currentLocation[0] * 40) + currentLocation[1]];
		map[(currentLocation[0] * 40) + currentLocation[1]] = droneName;

		if (currentLocation[0] == 0 && currentLocation[1] == 0)
			state = "LND";
	}
}

//Function for landing the drone back at port for state "LND"
void Drone::land()
{
	map[0] = 'P';																		//sets port location node to 'P'
	pthread_mutex_unlock(&mtx[(currentLocation[0] * 40) + currentLocation[1]]);
	deliveriesToMake--;																	//decrements deliveriesToMake
	allDeliveriesCount--;
	if (deliveriesToMake > 0)															//if deliveriesToMake is not 0
		state = "TKE";																	//sets state to "TKE" for takeoff
}

//Function returns a random number between 1-40 for the destinations of the drones deliveries
int Drone::getRND()
{
	int rnd;
	rnd = rand() % 40 + 1;						
	return rnd;
}

//Function controls the drone simulation
void* Drone::runSimulation(void*arg)
{		
	int destLoc[4];
	destLoc[0] = Drone::getRND();						//getting rows (0,2) and columns (1,3) for delivery locations
	destLoc[1] = Drone::getRND();			
	destLoc[2] = Drone::getRND();								
	destLoc[3] = Drone::getRND();

	if (destLoc[0] == 1 && destLoc[1] == 1)				//if the delivery location is the airport, redoes the column
		destLoc[1] = Drone::getRND();

	if (destLoc[2] == 1 && destLoc[3] == 1)
		destLoc[2] = Drone::getRND();

	threadIdent++; 										//increment threadIdent for droneName and destName
	
	Drone drone(Drone::getName(), Drone::getDest(),	destLoc[0], destLoc[1], destLoc[2], destLoc[3],  1, "TKE");	//create drone

	int speed;											//variable holds speed of drone (if it moves 1 or 2 spaces)
	while (drone.getDeliveries() != 0)					//while there are still deliveries to make
	{
		if (drone.getState() == "TKE")					//if in takeoff state
			drone.takeoff();							//calls takeoff function

		else if (drone.getState() == "CRU")				//if in cruise state
		{
			speed = drone.determineSpeed();				//calls determineSpeed() to determine the speed of the drone
			if (speed > 50)								//if speed returned is > 50, drone moves twice
			{
				drone.cruise();							//calls cruise function
				if (drone.getState() == "CRU")			//if still in cruise state
					drone.cruise();						//calls cruise function again
			}
			else										//drone moves once
				drone.cruise();							//calls cruise function
		}

		else if (drone.getState() == "DLV")				//if in delivering state
			drone.deliver();							//calls delivering function

		else if (drone.getState() == "RTN")				//if in returning to port state
		{
			speed = drone.determineSpeed();				//determine speed as above
			if (speed > 50)								//moves twice
			{
				drone.returnToPort();					//first move
				if (drone.getState() == "RTN")			//if still in returning to port state
					drone.returnToPort();				//moves again
			}
			else										//moves once
				drone.returnToPort();
		}

		else if (drone.getState() == "LND")				//if in landing state
			drone.land();								//calls landing function
					
		waiting++;										//increments waiting

		pthread_cond_signal(&cond);						//signal CV

		pthread_cond_wait(&cond2, &mut2);				//wait for release				
	}

	while (allDeliveriesCount != 0)						//loop waits until all threads have made their deliveries until before any threads exit
	{
		waiting++;										//increments waiting

		pthread_cond_signal(&cond);						//signal CV

		pthread_cond_wait(&cond2, &mut2);
	}

	pthread_exit(0);									//exit thread
}

//Function for determining drone speed
int Drone::determineSpeed()
{
	int speed;									//variable for speed

	speed = rand() % 100 + 1;					//sets speed to a random number between 1 and 100, <50 = 1 move, >50 = 2 moves

	return speed;								//returns speed
}

//Function for printing the 40x40 grid map
void* Drone::printMap(void*arg)
{
	while(allDeliveriesCount != 0)
	{
		while(waiting < threadNum)
			pthread_cond_wait(&cond, &mut);

		for (int i = 0; i < 40; i++)					//outer for loop runs through the rows
		{	
			for (int j = 0; j < 40; j++)				//inner for loop runs through the columns
				cout << map[i*40 + j] << "  ";			//ouputs the element of map array at specified row and column, followed by spaces
			cout << endl;								//ends line at the end of the row
		}
		cout << endl << endl;							//skips down several lines for next printing to begin
	
		waiting = 0;									//reset waiting counter
		pthread_cond_broadcast(&cond2);					//broadcast to all waiting threads
	}
	for (int i = 0; i < 40; i++)						//outer for loop runs through the rows
	{	
		for (int j = 0; j < 40; j++)					//inner for loop runs through the columns
			cout << map[i*40 + j] << "  ";				//ouputs the element of map array at specified row and column, followed by spaces
		cout << endl;									//ends line at the end of the row
	}
	pthread_cond_broadcast(&cond2);						//broadcast to all waiting threadds
	
	pthread_exit(0);									//exit thread
}