#pragma once

#include "IProcess.h"

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace ThreadWatcher
{
    /**
     * @brief Process class to manage a managed process.
     */
    class Process final : public IProcess
    {
    public:

        explicit Process();
        ~Process() override = default;

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
        bool HasStatus(Status status) const override;

        /**
         * @brief Checks if process is running.
         *
         * @return True if process is running otherwise false.
         */
        bool IsRunning() const override;

        /**
         * @brief Update status from running process. Checks if process runs or is death.
         */
        void UpdateStatus() override;

    private:

        /**
         * @brief Actual thread from running process.
         */
        std::thread _thread;

        /**
         * @brief Actual process status from process.
         */
        Status _processStatus = INIT;
    };
}