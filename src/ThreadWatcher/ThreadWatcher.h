#pragma once

#include "IWatcher.h"
#include "Process.h"
#include <map>
#include <string>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

namespace ThreadWatcher
{
    /**
 * @brief Thread watcher class to handle thread management from all subproccess.
 *
 * @author Andreas Sekulski
 */
    class Watcher final : public IWatcher
    {
    public:

        /**
         * @brief Construct a new Thread Watcher object
         */
        Watcher();

        /**
         * @brief Destroy the Thread Watcher object
         */
        ~Watcher() override = default;

        /**
         * @brief Checks if given process name exists and has a specific status.
         *
         * @param processName Name from process to check.
         * @param status Status check from process.
         * @return True if processName exists and status is equal otherwise false.
         */
        bool HasProcessStatus(const std::string& processName, Process::Status status);

        /**
         * @brief Checks if given process exists by name.
         *
         * @param processName Process name to check.
         * @return True if process exists otherwise false.
         */
        bool ExistsProcess(const std::string& processName);

        /**
         * @brief Add process for thread management. If an process already exists by this name old process will be stopped and overide by new one.
         *
         * @param processName Process name to add.
         * @param process Process to add.
         */
        void AddProcess(const std::string& processName, const std::shared_ptr<Process>& process);

        /**
         * @brief Remove process by process name. If process is running this process will be stoped and removed.
         *
         * @param processName Process name to remove.
         */
        void RemoveProcess(const std::string &processName);

        /**
         * Overridden function.
         */
        void Stop() override;

    private:

        /**
         * @brief Map handling from all observing processes.
         */
        std::map<std::string, std::shared_ptr<Process>> processes;

        /**
         * @brief Thread watcher to handle all processes.
         */
        std::jthread _watcher_thread;

        /**
         * @brief Mutex to lock guards for thread safe map handling.
         */
        std::mutex mutex;

        /**
         * @brief Get the Process object from given process name,
         *
         * @param processName Name to obtain process.
         * @return std::shared_ptr<Process> Process object to handle process threading.
         */
        std::shared_ptr<Process> GetProcess(const std::string &processName);

        /**
         * @brief Stop given process by process name.
         *
         * @param processName Process to Stop.
         */
        void StopActiveThread(const std::string &processName);

        /**
         * @brief Processing method to handle all processes.
         */
        void Process(const std::stop_token &stopToken);

        /**
         * @brief Start a detached process as thread.
         *
         * @param process Process to Start detached.
         */
        static void StartDetachedProcess(const std::shared_ptr<Process>& process);
    };
}
