#pragma once

#include "Process.h"
#include "IFunction.h"
#include <map>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

/**
 * @brief Thread watcher class to handle thread management from all subproccess.
 * 
 * @author Andreas Sekulski
 */
class ThreadWatcher : public :: IFunction
{
public:

	/**
	 * @brief Construct a new Thread Watcher object
	 */
	ThreadWatcher();

	/**
	 * @brief Destroy the Thread Watcher object
	 */
	~ThreadWatcher();

	/**
	 * @brief Checks if given process name exists and has a specific status.
	 * 
	 * @param processName Name from process to check.
	 * @param status Status check from process.
	 * @return True if processName exists and status is equal otherwise false.
	 */
	bool hasProcessStatus(std::string processName, Process::Status status);

	/**
	 * @brief Checks if given process exists by name.
	 * 
	 * @param processName Process name to check.
	 * @return True if process exists otherwise false.
	 */
	bool existsProcess(std::string processName);

	/**
	 * @brief Add process for thread management. If an process already exists by this name old process will be stopped and overide by new one.
	 * 
	 * @param processName Process name to add.
	 * @param process Process to add.
	 */
	void addProcess(std::string processName, std::shared_ptr<Process> process);

	/**
	 * @brief Remove process by process name. If process is running this process will be stoped and removed.
	 * 
	 * @param processName Process name to remove.
	 */
	void removeProcess(std::string processName);

	/**
	 * @brief Watch method to execute thread management watching. This method will block all calls by join thread.
	 */
	void watch();

	/**
	 * Overridden function.
	 */
	void process() override;

	/**
	 * Overridden function.
	 */
	void stop() override;

private:

	/**
	 * @brief Map handling from all subproccess.
	 */
	std::map<std::string, std::shared_ptr<Process>> threads;

	/**
	 * @brief Watcher thread process to execute thread management.
	 */
	Process *watcherThread;

	/**
	 * @brief Mutex to lock guards for thread safe map handling.
	 */
	std::mutex mutex;

	/**
	 * @brief Atomic indicator to stop watcher thread.
	 */
	std::atomic<bool> exitWatcher;

	/**
	 * @brief Get the Process object from given process name,
	 * 
	 * @param processName Name to obtain process.
	 * @return std::shared_ptr<Process> Process object to handle process threading.
	 */
	std::shared_ptr<Process> getProcess(std::string processName);

	/**
	 * @brief Stop given process by process name.
	 * 
	 * @param processName Process to stop.
	 */
	void stopActiveThread(std::string processName);

	/**
	 * @brief Start a detached process as thread.
	 * 
	 * @param process Process to start detached.
	 */
	void startDetachedProcess(std::shared_ptr<Process> process);
};
