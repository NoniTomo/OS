#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

bool flag = false;

condition_variable cond_var;
mutex mtx;

void consumer_function()
{
	while (true)
	{
		unique_lock<mutex> mutex_lock(mtx);
		{
			cond_var.wait(mutex_lock, []() {return flag; });
			flag = false;

			cout << "Событие обработано\n";
		}
	}
}

void producer_function()
{
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(1));
		
		unique_lock<mutex> mutex_lock(mtx);
		flag = true;
		
		cond_var.notify_one();

		cout << "Событие отправлено\n";
	}
}

int main()
{
	setlocale(LC_ALL, "ru");

	thread producer(producer_function);
	thread consumer(consumer_function);

	producer.join();
	consumer.join();

	return 0;
}
