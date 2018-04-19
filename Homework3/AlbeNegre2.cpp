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

void useResource(int id, int color)
{
	printf("Thread %d with color %d is using the resource.\n", id, color);
	unique_lock<mutex> lock(counter);
	if (color == WHITE)
		usingWhite++;
	else
		usingBlack++;
	lock.unlock();

	Sleep(2000);

	unique_lock<mutex> lck(counter);
	if (color == WHITE)
	{
		usingWhite--;
		if (usingWhite == 0)
			counterWait.notify_one();
	}
	else
	{
		usingBlack--;
		if (usingBlack == 0)
			counterWait.notify_one();
	}
	lck.unlock();
}

int colorStarving(int color)
{
	if (color == WHITE)
	{
		printf(" %d", (int)time(NULL) - lastUseWhite);
		if ((int)time(NULL) - lastUseWhite <= 5)
			return 0;
		else
		{
			if (waitingWhite == 0)
			{
				printf("Color WHITE is starving but there are no WHITE threads waiting.\n");
				return 0;
			}
			return 1;
		}
	}
	else
	{
		printf(" %d", (int)time(NULL) - lastUseBlack);
		if ((int)time(NULL) - lastUseBlack <= 5)
			return 0;
		else
		{
			if (waitingBlack == 0)
			{
				printf("Color BLACK is starving but there are no BLACK threads waiting.\n");
				return 0;
			}
			return 1;
		}
	}
}

int isOwner(int myColor)
{
	return myColor == owner;
}


void switchOwner(int id, int color)
{
	if (!isOwner(INTERMEDIATE))
	{
		printf("Thread %d with color %d is switching the owner color.\n", id, color);
		if (color == WHITE)
		{
			unique_lock<mutex> lck(counter);
			owner = INTERMEDIATE;
			while (usingWhite != 0)
			{
				counterWait.wait(lck);
				//pthread_cond_wait(&counterWait, &counter);
			}
			printf("All white are now done, giving control to black.\n");
			Sleep(1000);
			lastUseWhite = (int)time(NULL);
			blackWait.notify_all();
			owner = BLACK;
		}
		else
		{
			unique_lock<mutex> lck(counter);
			owner = INTERMEDIATE;
			while (usingBlack != 0)
			{
				counterWait.wait(lck);
			}
			printf("All black are now done, giving control to white.\n");
			Sleep(1000);
			lastUseBlack = (int)time(NULL);
			whiteWait.notify_all();
			owner = WHITE;
		}
	}
}


void getAccess(int id, int color)
{
	if (!(isOwner(color)))
	{
		printf("Thread %d with color %d is waiting\n", id, color);
		unique_lock<mutex> lock(counter);
		if (color == WHITE)
			waitingWhite++;
		else
			waitingBlack++;
		lock.unlock();
	}
	while (!(isOwner(color)))
	{
		unique_lock<mutex> lock(ownerMutex);
		if (color == WHITE)
		{
			whiteWait.wait(lock);
			waitingWhite--;
		}
		else
		{
			blackWait.wait(lock);
			waitingBlack--;
		}

		lock.unlock();
	}

}

struct proces
{
	int id;
	int color;
}temp;

typedef struct proces proces;

void releaseAccess(int id, int color)
{
	if ((color == WHITE && colorStarving(BLACK)) || (color == BLACK && colorStarving(WHITE)))
		switchOwner(id, color);
}

void doRoutine(int id, int color)
{
	getAccess(id, color);

	useResource(id, color);

	releaseAccess(id, color);

	return;
}

int main()
{
	lastUseBlack = (int)time(NULL);
	lastUseWhite = (int)time(NULL);
	for (int i = 0; i<50; i++)
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
