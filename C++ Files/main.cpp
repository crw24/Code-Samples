//This program was written by Christian Wilson
//This program simulates a Drone Collision Avoidance System
#include "dcas.h"
#include <iostream>
#include <string>
#include <pthread.h>
#include <time.h>

using namespace std;

int threadNum = 0;												//Number of threads
pthread_mutex_t mtx[40*40] = PTHREAD_MUTEX_INITIALIZER;			//initialize map

int main()
{
	srand(time(NULL));											//seeding the random generator
	map[0] = 'P';												//initialize the map, sets first node to 'P' for p rest to '*' for empty
	for (int i = 1; i < 1600; i++)
		map[i] = '*';

	for (int i = 0; i < 1600; i++)
		mtx[i] = PTHREAD_MUTEX_INITIALIZER;

	for (int i = 0; i < 40; i++)					//outer for loop runs through the rows
	{	
		for (int j = 0; j < 40; j++)				//inner for loop runs through the columns
			cout << map[i*40 + j] << "  ";			//ouputs the element of map array at specified row and column, followed by spaces
		cout << endl;								//ends line at the end of the row
	}
	cout << endl << endl;							//skips down several lines for next printing to begin

	pthread_t tids[10];
	pthread_t prt;
	pthread_create(&prt, NULL, Drone::printMap, NULL);

	for (int i = 0; i < 10; i++)										//for loop for creating threads
	{
		pthread_create(&tids[i], NULL, Drone::runSimulation, NULL);	
		threadNum++;
	}

	for(int i = 0; i < 10; i++)											//for loop for joining to main
	{
		pthread_join(tids[i], NULL);
	}

	for (int i = 0; i < 40; i++)					//outer for loop runs through the rows
	{	
		for (int j = 0; j < 40; j++)				//inner for loop runs through the columns
			cout << map[i*40 + j] << "  ";			//ouputs the element of map array at specified row and column, followed by spaces
		cout << endl;								//ends line at the end of the row
	}
	cout << endl << endl;							//skips down several lines for next printing to begin

	return 0;
}