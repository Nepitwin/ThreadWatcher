#include "gtest/gtest.h"
#include "Util.h"
#include <ThreadWatcher/ThreadWatcher.h>
#include <ThreadWatcher/Process.h>
#include <thread>
#include <chrono>
#include <vector>

using namespace std::chrono_literals;

/**
 * @brief Stop watcher method to test a delayed stop execution.
 * 
 * @param watcher Thread watcher to stop.
 */
void stopWatcher(ThreadWatcher &watcher)
{
	std::this_thread::sleep_for(6s);
	watcher.stop();
}

/**
 * @brief Monitoring watcher test for processing monitoring if watcher is running.
 * 
 * @param watcher Watcher to test by monitoring.
 * @param processingNames Processing list to test if running.
 */
void monitorWatcher(ThreadWatcher &watcher, std::vector<std::string> processingNames)
{
	int i = 0;

	// Check four times. Tests runs overrall six seconds.
	while (i < 4)
	{
		std::this_thread::sleep_for(1s);
		for (std::string processingName : processingNames)
		{
			if (watcher.existsProcess(processingName))
			{
				bool result = watcher.hasProcessStatus(processingName, Process::Status::RUN);
				if (!result)
				{
					// Wait 2 seconds because process can be added dynamic by an running watcher
					std::this_thread::sleep_for(2s);
					result = watcher.hasProcessStatus(processingName, Process::Status::RUN);
				}
				EXPECT_TRUE(result);
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
void addProcessToWatcher(ThreadWatcher &watcher)
{
	std::this_thread::sleep_for(2s);
	std::shared_ptr<IFunction> ptrProcessClean(new CleanProcessTesting());
	std::shared_ptr<Process> processClean(new Process(ptrProcessClean));
	watcher.addProcess("PASS_A37", processClean);
	ASSERT_TRUE(watcher.existsProcess("PASS_A37"));
	std::this_thread::sleep_for(2s);
	// Watcher is running added process will be executed immediately
	ASSERT_TRUE(watcher.hasProcessStatus("PASS_A37", Process::Status::RUN));
}

TEST(TestThreadWatchers, ExistsProcessNotFound) 
{
	ThreadWatcher watcher;
	// https://www.youtube.com/watch?v=lIiUR2gV0xk ;)
	ASSERT_FALSE(watcher.existsProcess("PASS_A38"));
}

TEST(TestThreadWatchers, ExistsProcess)
{
	ThreadWatcher watcher;
	std::shared_ptr<IFunction> ptrFunction(new CleanProcessTesting());
	std::shared_ptr<Process> process(new Process(ptrFunction));

	watcher.addProcess("PASS_A38", process);
	ASSERT_TRUE(watcher.existsProcess("PASS_A38"));
	ASSERT_TRUE(watcher.hasProcessStatus("PASS_A38", Process::Status::INIT));
}

TEST(TestThreadWatchers, RemoveProcess)
{
	ThreadWatcher watcher;
	std::shared_ptr<IFunction> ptrFunction(new CleanProcessTesting());
	std::shared_ptr<Process> process(new Process(ptrFunction));

	ASSERT_FALSE(watcher.existsProcess("PASS_A38"));
	watcher.addProcess("PASS_A38", process);
	ASSERT_TRUE(watcher.existsProcess("PASS_A38"));
	watcher.removeProcess("PASS_A38");
	ASSERT_FALSE(watcher.existsProcess("PASS_A38"));
}

TEST(TestThreadWatchers, ProcessMonitoring)
{
	ThreadWatcher watcher;
	std::shared_ptr<IFunction> ptrProcessClean(new CleanProcessTesting());
	std::shared_ptr<IFunction> ptrProcessKill(new KillProcessTesting());
	std::shared_ptr<Process> processClean(new Process(ptrProcessClean));
	std::shared_ptr<Process> processKill(new Process(ptrProcessKill));

	std::vector<std::string> processingNames = { "PASS_A38", "PASS_A39" };

	std::thread stopWatcher = std::thread(&::stopWatcher, std::ref(watcher));
	std::thread monitorWatcher = std::thread(&::monitorWatcher, std::ref(watcher), processingNames);

	watcher.addProcess("PASS_A38", processClean);
	watcher.addProcess("PASS_A39", processKill);
	watcher.watch();

	if (monitorWatcher.joinable())
		monitorWatcher.join();

	if (stopWatcher.joinable())
		stopWatcher.join();

	EXPECT_TRUE(watcher.hasProcessStatus("PASS_A38", Process::Status::FINISHED));
	EXPECT_TRUE(watcher.hasProcessStatus("PASS_A39", Process::Status::KILLED));
}

TEST(TestThreadWatchers, ProcessMonitoringAddNewProcessByRun)
{
	ThreadWatcher watcher;
	std::shared_ptr<IFunction> ptrProcessClean(new CleanProcessTesting());
	std::shared_ptr<IFunction> ptrProcessKill(new KillProcessTesting());
	std::shared_ptr<Process> processClean(new Process(ptrProcessClean));
	std::shared_ptr<Process> processKill(new Process(ptrProcessKill));

	std::vector<std::string> processingNames = {"PASS_A37", "PASS_A38", "PASS_A39" };

	std::thread stopWatcher = std::thread(&::stopWatcher, std::ref(watcher));
	std::thread monitorWatcher = std::thread(&::monitorWatcher, std::ref(watcher), processingNames);
	std::thread includeWatcherProcess = std::thread(&::addProcessToWatcher, std::ref(watcher));

	watcher.addProcess("PASS_A38", processClean);
	watcher.addProcess("PASS_A39", processKill);
	watcher.watch();

	if (includeWatcherProcess.joinable())
		includeWatcherProcess.join();

	if (monitorWatcher.joinable())
		monitorWatcher.join();

	if (stopWatcher.joinable())
		stopWatcher.join();

	EXPECT_TRUE(watcher.hasProcessStatus("PASS_A37", Process::Status::FINISHED));
	EXPECT_TRUE(watcher.hasProcessStatus("PASS_A38", Process::Status::FINISHED));
	EXPECT_TRUE(watcher.hasProcessStatus("PASS_A39", Process::Status::KILLED));
}