#include "gtest/gtest.h"
#include "Util.h"
#include <ThreadWatcher/IFunction.h>
#include <ThreadWatcher/Process.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

TEST(TestProcess, ProcessingStatusClean) 
{
	std::shared_ptr<IFunction> ptrFunction(new CleanProcessTesting());
	std::shared_ptr<Process> process(new Process(ptrFunction));

	ASSERT_TRUE(process->hasStatus(Process::Status::INIT));
	std::thread threadStart = std::thread(&Process::start, process);
	std::this_thread::sleep_for(2s);

	ASSERT_TRUE(process->hasStatus(Process::Status::RUN));

	std::thread threadStop = std::thread(&Process::stop, process);

	if (threadStart.joinable())
		threadStart.join();

	if (threadStop.joinable())
		threadStop.join();

	ASSERT_TRUE(process->hasStatus(Process::Status::FINISHED));
}

TEST(TestProcess, ProcessingStatusKill)
{
	std::shared_ptr<IFunction> ptrFunction(new KillProcessTesting());
	std::shared_ptr<Process> process(new Process(ptrFunction));

	ASSERT_TRUE(process->hasStatus(Process::Status::INIT));
	std::thread threadStart = std::thread(&Process::start, process);
	std::this_thread::sleep_for(2s);

	ASSERT_TRUE(process->hasStatus(Process::Status::RUN));

	std::thread threadStop = std::thread(&Process::stop, process);

	if (threadStart.joinable())
		threadStart.join();

	if (threadStop.joinable())
		threadStop.join();

	ASSERT_TRUE(process->hasStatus(Process::Status::KILLED));
}