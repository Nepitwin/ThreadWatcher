#include "gtest/gtest.h"
#include "Util.h"
#include <ThreadWatcher/ThreadWatcher.h>
#include <ThreadWatcher/ThreadProcess.h>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

using namespace std::chrono_literals;

/**
 * @brief Test struct for processing.
 */
struct TestProcess
{
	std::string name; // Name from process
	bool firstRun; // First run can be fail because init thread still running
};

/**
 * @brief Stop watcher method to test a delayed Stop execution.
 * 
 * @param watcher Thread watcher to Stop.
 */
void StopWatcher(ThreadWatcher::Watcher &watcher)
{
	std::this_thread::sleep_for(6s);
	watcher.Stop();
}

/**
 * @brief Monitoring watcher test for processing monitoring if watcher is running.
 * 
 * @param watcher Watcher to test by monitoring.
 * @param processingNames Processing list to test if running.
 */
void MonitorWatcher(ThreadWatcher::Watcher &watcher, const std::vector<TestProcess>& processes)
{
	int i = 0;
	
	// Check four times. Tests runs overrall six seconds.
	while (i < 4)
	{
		std::this_thread::sleep_for(1s);
		for (TestProcess process : processes)
		{
			if (watcher.ExistsProcess(process.name))
			{
				bool result = watcher.HasProcessStatus(process.name, ThreadWatcher::ThreadProcess::Status::RUN);
				if (!result && !process.firstRun)
				{
					std::cout << "Process = " << process.name << std::endl;
				}
				EXPECT_TRUE(process.firstRun || result);
                process.firstRun = false;
			}
		}
			
		i++;
	}
}

/**
 * @brief Test to add process by a running watcher.
 * 
 * @param watcher Watcher to test to add a new process.
 */
void addProcessToWatcher(ThreadWatcher::Watcher &watcher)
{
	std::this_thread::sleep_for(1s);
	std::shared_ptr<ThreadWatcher::IFunction> ptrProcessClean(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> processClean(new ThreadWatcher::ThreadProcess(ptrProcessClean));
    watcher.AddProcess("PASS_A37", processClean);
	ASSERT_TRUE(watcher.ExistsProcess("PASS_A37"));
}

TEST(TestThreadWatchers, ExistsProcessNotFound) 
{
	ThreadWatcher::Watcher watcher;
	// https://www.youtube.com/watch?v=lIiUR2gV0xk ;)
	ASSERT_FALSE(watcher.ExistsProcess("PASS_A38"));
}

TEST(TestThreadWatchers, ExistsProcess)
{
    ThreadWatcher::Watcher watcher;
	std::shared_ptr<ThreadWatcher::IFunction> ptrFunction(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> process(new ThreadWatcher::ThreadProcess(ptrFunction));

    watcher.AddProcess("PASS_A38", process);
	ASSERT_TRUE(watcher.ExistsProcess("PASS_A38"));
	ASSERT_TRUE(watcher.HasProcessStatus("PASS_A38", ThreadWatcher::ThreadProcess::Status::INIT));
}

TEST(TestThreadWatchers, RemoveProcess)
{
    ThreadWatcher::Watcher watcher;
	std::shared_ptr<ThreadWatcher::IFunction> ptrFunction(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> process(new ThreadWatcher::ThreadProcess(ptrFunction));

	ASSERT_FALSE(watcher.ExistsProcess("PASS_A38"));
    watcher.AddProcess("PASS_A38", process);
	ASSERT_TRUE(watcher.ExistsProcess("PASS_A38"));
    watcher.RemoveProcess("PASS_A38");
	ASSERT_FALSE(watcher.ExistsProcess("PASS_A38"));
}

TEST(TestThreadWatchers, ProcessMonitoring)
{
    ThreadWatcher::Watcher watcher;
	std::shared_ptr<ThreadWatcher::IFunction> ptrProcessClean(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::IFunction> ptrProcessKill(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> processClean(new ThreadWatcher::ThreadProcess(ptrProcessClean));
	std::shared_ptr<ThreadWatcher::ThreadProcess> processKill(new ThreadWatcher::ThreadProcess(ptrProcessKill));

	std::vector<TestProcess> processing = {{"PASS_A38", true }, {"PASS_A39", true } };

	std::thread stopWatcher = std::thread(&::StopWatcher, std::ref(watcher));
	std::thread monitorWatcher = std::thread(&::MonitorWatcher, std::ref(watcher), processing);

    watcher.AddProcess("PASS_A38", processClean);
    watcher.AddProcess("PASS_A39", processKill);
    watcher.Watch();

	if (monitorWatcher.joinable())
		monitorWatcher.join();

	if (stopWatcher.joinable())
		stopWatcher.join();

	EXPECT_TRUE(watcher.HasProcessStatus("PASS_A38", ThreadWatcher::ThreadProcess::Status::FINISHED));
	EXPECT_TRUE(watcher.HasProcessStatus("PASS_A39", ThreadWatcher::ThreadProcess::Status::FINISHED));
}

TEST(TestThreadWatchers, ProcessMonitoringAddNewProcessByRun)
{
    ThreadWatcher::Watcher watcher;
	std::shared_ptr<ThreadWatcher::IFunction> ptrProcessClean(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::IFunction> ptrProcessKill(new ProcessTesting());
	std::shared_ptr<ThreadWatcher::ThreadProcess> processClean(new ThreadWatcher::ThreadProcess(ptrProcessClean));
	std::shared_ptr<ThreadWatcher::ThreadProcess> processKill(new ThreadWatcher::ThreadProcess(ptrProcessKill));

	std::vector<TestProcess> processing = {{"PASS_A37", true }, {"PASS_A38", true }, {"PASS_A39", true }};

	std::thread stopWatcher = std::thread(&::StopWatcher, std::ref(watcher));
	std::thread monitorWatcher = std::thread(&::MonitorWatcher, std::ref(watcher), processing);
	std::thread includeWatcherProcess = std::thread(&::addProcessToWatcher, std::ref(watcher));

    watcher.AddProcess("PASS_A38", processClean);
    watcher.AddProcess("PASS_A39", processKill);
    watcher.Watch();

	if (includeWatcherProcess.joinable())
		includeWatcherProcess.join();

	if (monitorWatcher.joinable())
		monitorWatcher.join();

	if (stopWatcher.joinable())
		stopWatcher.join();

	EXPECT_TRUE(watcher.HasProcessStatus("PASS_A37", ThreadWatcher::ThreadProcess::Status::FINISHED));
	EXPECT_TRUE(watcher.HasProcessStatus("PASS_A38", ThreadWatcher::ThreadProcess::Status::FINISHED));
	EXPECT_TRUE(watcher.HasProcessStatus("PASS_A39", ThreadWatcher::ThreadProcess::Status::FINISHED));
}