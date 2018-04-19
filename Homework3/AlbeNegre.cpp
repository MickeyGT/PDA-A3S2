#include<iostream>
#include<stdlib.h>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<time.h>
#include<Windows.h>
using namespace std;
#define BLACK 1
#define WHITE 0
#define INTERMEDIATE 2
int owner = WHITE, usingWhite = 0, usingBlack = 0, waitingWhite = 0, waitingBlack = 0;
int lastUseBlack, lastUseWhite;
mutex counter, ownerMutex;
condition_variable whiteWait, blackWait,counterWait;
int colorUsing[10] = { 0,0,0,0,0,0,0,0,0,0 };

void useResource(int id, int color)
{
	if (colorUsing[id] == 0)
	{
		colorUsing[id] = 1;
		if (color == WHITE)
		{
			unique_lock<mutex> lck(counter);
			usingWhite++;
			waitingWhite--;
			lck.unlock();
		}
		else
		{
			unique_lock<mutex> lck(counter);
			usingBlack++;
			waitingBlack--;
			lck.unlock();
		}
	}
	if (color == WHITE)
		lastUseWhite = (int)time(NULL);
	else
		lastUseBlack = (int)time(NULL);
	printf("Thread %d with color %d is using the resource.\n", id, color);
	Sleep(1000);
}

int otherColorNotStarving(int color)
{
	int otherColor;
	if (color == WHITE)
	{
		//printf("Time after switch:%d ",(int)time(NULL)-lastUseBlack);
		if ((int)time(NULL) - lastUseBlack <= 5)
			return 1;
		else
			return 0;
	}
	else
	{
		//printf("Time after switch:%d ",(int)time(NULL)-lastUseWhite);
		if ((int)time(NULL) - lastUseWhite <= 5)
			return 1;
		else
			return 0;
	}
}

int isOwner(int myColor)
{
	return myColor == owner;
}


void stopUsingWait(int id, int color)
{
	if (colorUsing[id] == 1)
	{
		colorUsing[id] = 0;
		if (color == WHITE)
		{
			unique_lock<mutex> lck(counter);
			waitingWhite++;
			usingWhite--;
			lck.unlock();
			printf("Thread %d is now giving his place.\n", id);
			if (usingWhite == 0)
			{
				//pthread_mutex_lock(&ownerMutex);
				printf("All white are now done, giving control to black.\n");
				owner = BLACK;
				Sleep(1000);
				blackWait.notify_all();
				//pthread_mutex_unlock(&ownerMutex); 
			}
		}
		else
		{
			unique_lock<mutex> lck(counter);
			waitingBlack++;
			usingBlack--;
			lck.unlock();
			printf("Thread %d is now giving his place.\n", id);
			if (usingBlack == 0)
			{
				//pthread_mutex_lock(&ownerMutex);
				printf("All black are now done, giving control to white.\n");
				owner = WHITE;
				Sleep(1000);
				whiteWait.notify_all();
				//pthread_mutex_unlock(&ownerMutex); 
			}
		}
	}
}


void doRoutine(int id, int color)
{
	while (1)
	{
		if (isOwner(color) && otherColorNotStarving(color))
			useResource(id, color);
		else
			if (isOwner(color))
				stopUsingWait(id, color);
			else
			{
				while (!(isOwner(color)))
				{
					unique_lock<mutex> lck(ownerMutex);
					if (color == WHITE)
						whiteWait.wait(lck);
					else
						blackWait.wait(lck);
					lck.unlock();
				}
			}
	}
}

int main()
{
	lastUseBlack = (int)time(NULL);
	lastUseWhite = (int)time(NULL);
	for (int i = 0; i<10; i++)
	{
		//color= WHITE;
		srand(time(NULL));
		int clr = rand() % 2;
		thread(doRoutine, i,clr);
		Sleep(1000);
	}
	Sleep(205000);
	return 0;
}
