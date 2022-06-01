#pragma once

#include "IFunction.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace ThreadWatcher
{
    /**
     * @brief Thread process class to handle threads.
     *
     * @author Andreas Sekulski
     */
    class ThreadProcess
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
            RUNS_AFTER_KILL	///< Process can not be killed and runs always.
        };

        /**
         * @brief Construct a new Process object
         *
         * @param function
         */
        explicit ThreadProcess(std::shared_ptr<IFunction> function);

        /**
         * @brief Destroy the Process object
         */
        ~ThreadProcess() = default;

        /**
         * @brief Start executions from process.
         */
        void Start();

        /**
         * @brief Stop process if running.
         */
        void Stop();

        /**
         * @brief Checks actual process status.
         *
         * @param status Status to check from process.
         * @return True if process is equal to checked status otherwise false.
         */
        bool HasStatus(ThreadProcess::Status status);

        /**
         * @brief Checks if process is running.
         *
         * @return True if process is running otherwise false.
         */
        bool IsRunning();

        /**
         * @brief Update status from running process. Checks if process runs or is death.
         */
        void UpdateStatus();

    private:

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
        ThreadProcess::Status processStatus;
    };
}