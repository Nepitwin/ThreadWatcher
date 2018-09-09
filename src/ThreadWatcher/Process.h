#pragma once

#include "IFunction.h"
// WIN32
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

/**
 * @brief Process class to handle process threads.
 * 
 * @author Andreas Sekulski
 */
class Process
{
public:

	/**
	 * @brief Thread conditions from process which can be reached.
	 * 
	 * INIT -> RUN -> FINISHED (Thread clear finished)
	 *             -> KILLED (Thread forced to kill)
	 * 			   -> KILLEXCEPTION (Thread can not be hard killed because of an exception)
	 */            
	enum Status
	{
		INIT, 			///< Initialization from process but not started
		RUN, 			///< Process is running
		FINISHED, 		///< Process is stopped clear
		KILLED, 		///< Process is hard killed
		KILLEXCEPTION	///< Process can not be killed by an exception
	};

	/**
	 * @brief Construct a new Process object
	 * 
	 * @param function 
	 */
	Process(std::shared_ptr<IFunction> function);

	/**
	 * @brief Destroy the Process object
	 */
	~Process();

	/**
	 * @brief Start executions from process.
	 */
	void start();

	/**
	 * @brief Stop process if running.
	 */
	void stop();

	/**
	 * @brief Checks actual process status.
	 * 
	 * @param status Status to check from process.
	 * @return True if process is equal to checked status otherwise false.
	 */
	bool hasStatus(Process::Status status);

	/**
	 * @brief Checks if process is running.
	 * 
	 * @return True if process is running otherwise false.
	 */
	bool isRunning();

	/**
	 * @brief Update status from running process. Checks if process runs or is death.
	 */
	void updateStatus();

private:

	/**
	 * @brief Native handler from running thread.
	 */
	void* native_handle;

	/**
	 * @brief Actual thread from running process.
	 */
	std::thread thread;

	/**
	 * @brief Implemented function to execute.
	 */
	std::shared_ptr<IFunction> function;

	/**
	 * @brief Actual process status from process.
	 */
	Process::Status processStatus;
};
