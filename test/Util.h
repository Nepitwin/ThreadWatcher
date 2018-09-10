#include <ThreadWatcher/IFunction.h>
#include <chrono>
#include <atomic>

/**
* @brief Test implementation from a example process.
*
* @author Andreas Sekulski
*/
class ProcessTesting : public ::IFunction
{

public:

	/**
	* @brief Construct a new Clean Process Testing object
	*/
    ProcessTesting()
	{
		stopProcess = false;
	}

	/**
	* @brief Destroy the Clean Process Testing object
	*/
	virtual ~ProcessTesting() {};

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