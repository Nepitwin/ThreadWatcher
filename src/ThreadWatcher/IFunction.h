#pragma once

/**
 * @brief Interface class to create thread based processing.
 * 
 * @author Andreas Sekulski
 */
class IFunction
{

public:
	
	/**
	 * @brief Execution method from thread.
	 */
	virtual void process() = 0;
	
	/**
	 * @brief Stop method for a clean stop from thread.
	 */
	virtual void stop() = 0;
};