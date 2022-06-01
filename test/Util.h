#include <ThreadWatcher/IFunction.h>
#include <chrono>
#include <atomic>

/**
* @brief Test implementation from a example process.
*
* @author Andreas Sekulski
*/
class ProcessTesting : public ThreadWatcher::IFunction
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
	virtual ~ProcessTesting() = default;

	/**
	 * Overridden function.
	 */
	void Process() override
	{
		while (!stopProcess) {}
	}

	/**
	 * Overridden function.
	 */
	void Stop() override
	{
		stopProcess = true;
	}

private:

	/**
	 * @brief Atomic boolean to Stop process.
	 */
	std::atomic<bool> stopProcess;
};