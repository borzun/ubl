#include <iostream>
#include <string>
#include "ThreadPool.h"
#include "ThreadsafeQueue.h"

class MeMoveClass
{
public:
	explicit MeMoveClass(const std::string& s)
		: str(s)
	{ }

	auto getString() const
	{
		return str;
	}

	std::string str;
};

void my_main()
{
	auto my_func = [](std::string& str) -> int
	{
		MeMoveClass mClass = MeMoveClass(str);
		std::cout << "My_Func: " << mClass.getString() << "\n";

		str = "crook_changed";
		return 0;
	};

	std::string str = "crook";
	const auto asyncReturn = ThreadPool::getInstance().run(my_func, std::ref(str));

	//asyncReturn.get();

	std::cout << "STR after crook: " << str << "\n";

	//std::cout << "source_str: " << str << "; me_class_str: " << mclass.str << "\n";
}

void testThreadPool()
{
	ThreadPool threadPool;

	for (int i = 0; i < 100; ++i) {
		threadPool.run([i]()
		{
			int msecs = i * 20 + rand() % 100;
			std::this_thread::sleep_for(std::chrono::milliseconds(msecs));
			std::cout << "Finished processing job #" << i << " with thread: " << std::this_thread::get_id() << "\n";
		});
	}

	while (!threadPool.isIdle());

	std::cout << "Thread pool is IDLE! \n";
	std::cout << "\n\n\n";
}

int my_func(int a, int b)
{
	int ret = a * b;
	std::cout << "RET = " << ret << "\n";
	return ret;
}

int main()
{

	auto lambda_x = [](int & obj)
	{

	};


	srand(time(NULL));

	for (auto i = 0; i < 10; ++i)
	{
		testThreadPool();



		std::this_thread::sleep_for(std::chrono::milliseconds(5000));


	}

	std::cout << "----------------------------------------------------------------\n";
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "Thread pool test is FINISHED! \n";
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "----------------------------------------------------------------\n";
	std::cout << "----------------------------------------------------------------\n";

	const auto lambda_t =
		[](int a, int b) -> int
	{
		int ret = a * b;
		std::cout << "RET = " << ret << "\n";
		return ret;
	};

	using func_type = std::function<decltype(my_func)>;
	std::function<int(void)> func = std::bind(lambda_t, 5, 10);

	ThreadsafeQueue<std::string> queue;

	std::thread th1([&queue]
	{
		for (int i = 0; i <= 100; ++i) {
			queue.push(std::string("value_") + std::to_string(i));
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});

	std::vector<std::thread> threads;
	for (int i = 1; i < 7; ++i) {
		auto lam_str =
			[&queue, i]
		{
			while (queue.top() != "value_100") {
				std::string str = queue.top();
				auto offset = str.find_first_of("_") + 1;
				auto value = std::stoi(str.substr(offset));
				if (value % i == 0) {
					//std::cout << "Thread #" << i << ": " << str << "\n";
					//std::this_thread::sleep_for(std::chrono::milliseconds(25));
				}
			}
		};

		threads.emplace_back(lam_str);
	}

	th1.join();

	return 0;
}
