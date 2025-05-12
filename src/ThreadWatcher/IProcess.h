#pragma once
#include "Status.h"

namespace ThreadWatcher {

    /**
     * @brief Interface class to create a process watcher object.
     */
    class IProcess
    {

    public:
        virtual ~IProcess() = default;

        /**
         * @brief Checks actual process status.
         *
         * @param status Status to check from process.
         * @return True if process is equal to checked status otherwise false.
         */
        virtual bool HasStatus(Status status) const = 0;

        /**
         * @brief Checks if process is running.
         *
         * @return True if process is running otherwise false.
         */
        virtual bool IsRunning() const = 0;

        /**
         * @brief Update status from running process. Checks if process runs or is death.
         */
        virtual void UpdateStatus() = 0;
    };


}
