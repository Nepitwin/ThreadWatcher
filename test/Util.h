#include <ThreadWatcher/IFunction.h>
#include <chrono>
#include <atomic>

/**
* @brief Test implementation from a process function which stopes clear.
*
* @author Andreas Sekulski
*/
class CleanProcessTesting : public ::IFunction
{

public:

	/**
	* @brief Construct a new Clean Process Testing object
	*/
	CleanProcessTesting()
	{
		stopProcess = false;
	}

	/**
	* @brief Destroy the Clean Process Testing object
	*/
	virtual ~CleanProcessTesting() {};

	/**
	 * Overridden function.
	 */
	void process() override
	{
		while (!stopProcess) {}
	}

	/**
	 * Overridden function.
	 */
	void stop() override
	{
		stopProcess = true;
	}

private:

	/**
	* @brief Atomic boolean to stop process.
	*/
	std::atomic<bool> stopProcess;
};

/**
* @brief Test implementation from a process function which only can be killed hard.
*
* @author Andreas Sekulski
*/
class KillProcessTesting : public ::IFunction
{

public:

	/**
	* @brief Construct a new Kill Process Testing object
	*/
	KillProcessTesting() {}

	/**
	* @brief Destroy the Kill Process Testing object
	*/
	virtual ~KillProcessTesting() {};

	/**
	 * Overridden function.
	 */
	void process() override
	{
		while (true) {}
	}

	/**
	 * Overridden function.
	 */
	void stop() override {}
};