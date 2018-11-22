//This program was written by Christian wilson
//This header contains the declaration of the drone class
#ifndef DCAS_H
#define DCAS_H

#include <string>
#include <iostream>
#include <pthread.h>
#include <condition_variable>
using namespace std; 

extern char map[40 * 40];						//global variable for the grid
extern int threadNum;							//global variable for number of threads
extern pthread_mutex_t mtx[40*40];
extern int allDeliveriesCount;

extern pthread_mutex_t mut;						//global variables for condition variables
extern pthread_cond_t cond;
extern int waiting;

extern pthread_mutex_t mut2;			
extern pthread_cond_t cond2;
extern int threadIdent;

class Drone
{
	char droneName;								//Character for identifying the drone (A,B,C, etc.)
	char droneDestination;						//Character that corresponds to drone name (a,b,c etc.)
	char previousNode;							//What used to be in the previous node
	int currentLocation[2];						//Array for the current location of the drone ([0] for row, [1] for column)
	int destinationLocation[4];					//Same as above but for the destination
	int deliveriesToMake;						//Number of deliveries the drone must make before it is done
	string state;								//the state of the drone (TKE = takeoff, LND = landing, CRU= cruising, DLV = delivering, RTN = return)
	condition_variable cv;

	public:
		Drone(char name, char destination, int destRow1, int destCol1, int destRow2, int destCol2, int numDeliveries, string startState);			//Constructor for drones
		void cruise();																		//Function for CRU state
		int determineSpeed();																//Function that determines if drone moves 1 or 2 spaces
		void land();																		//Function for LND state
		void takeoff();																		//Function for TKE state
		void deliver();																		//Function for DLV state
		void returnToPort();																//Function for RTN state
		static void* runSimulation(void*arg);												//Controls which function gets called, when the drone is done
		static void* printMap(void*arg);													//prints out the map
		int getDeliveries();																//returns deliveriesToMake
		string getState();																	//returns state
		static char getName();																//determines name of drone
		static char getDest();																//determines destination name
		static int getRND();																//returns random numbers
};

#endif