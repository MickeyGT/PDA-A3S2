#include<iostream>
#include<mutex>
#include<thread>
using namespace std;
long long nr=0;
mutex  Mutex;

void producer()
{
	
	while (true)
	{
		Mutex.lock();
		if (nr < 10)
		{
			nr++;
			cout << "Produced. " << nr << "\n";
		}
		Mutex.unlock();
	}
}

void consumer()
{
	
	while (true)
	{
		Mutex.lock();
		if (nr != 0)
		{
			nr--;
			cout << "Consumed. " << nr << "\n";
		}
		Mutex.unlock();
	}
}

int main()
{
	thread producer(producer);
	thread consumer(consumer);
	producer.join();
	consumer.join();
	return 0;
}
