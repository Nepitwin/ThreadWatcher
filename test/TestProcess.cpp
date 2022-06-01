#include "gtest/gtest.h"
#include "Util.h"
#include <ThreadWatcher/IFunction.h>
#include <ThreadWatcher/ThreadProcess.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

TEST(TestProcess, ProcessingStatus)
{
	std::shared_ptr<ThreadWatcher::IFunction> ptrFunction(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> process(new ThreadWatcher::ThreadProcess(ptrFunction));

	ASSERT_TRUE(process->HasStatus(ThreadWatcher::ThreadProcess::Status::INIT));
	std::thread threadStart = std::thread(&ThreadWatcher::ThreadProcess::Start, process);
	std::this_thread::sleep_for(2s);

	ASSERT_TRUE(process->HasStatus(ThreadWatcher::ThreadProcess::Status::RUN));

	std::thread threadStop = std::thread(&ThreadWatcher::ThreadProcess::Stop, process);

	if (threadStart.joinable())
		threadStart.join();

	if (threadStop.joinable())
		threadStop.join();

	ASSERT_TRUE(process->HasStatus(ThreadWatcher::ThreadProcess::Status::FINISHED));
}