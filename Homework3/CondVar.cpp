#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>
using namespace std;
long long nr=0;
condition_variable condProd,condCons;
mutex Mutex;

void producer()
{
	
	while (true)
	{
		unique_lock<mutex> lck(Mutex);
		if (nr == 10)
			condProd.wait(lck);
		
		nr++;
		cout << "Produced. " << nr << "\n";
		
		lck.unlock();
		condCons.notify_one();
	}
}

void consumer()
{
	
	while (true)
	{
		unique_lock<mutex> lck(Mutex);
		if (nr == 0)
			condCons.wait(lck);
		nr--;
		cout << "Consumed. " << nr << "\n";
		lck.unlock();
		condProd.notify_one();
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
