#include "ThreadWatcher.h"

#include <functional>
#include <thread>

ThreadWatcher::Watcher::Watcher() {
    _watcher_thread = std::jthread(std::bind_front(&Watcher::Process, this));
}

bool ThreadWatcher::Watcher::HasProcessStatus(const std::string &processName, const Process::Status status) {

    if (ExistsProcess(processName)) {
        return GetProcess(processName)->HasStatus(status);
    }

    return false;
}

bool ThreadWatcher::Watcher::ExistsProcess(const std::string &processName) {
    mutex.lock();
    const auto it = processes.find(processName);
    mutex.unlock();
    return it != processes.end();
}

void ThreadWatcher::Watcher::AddProcess(const std::string &processName, const std::shared_ptr<Process> &process) {
    StopActiveThread(processName);
    mutex.lock();
    processes.insert(std::pair<std::string, std::shared_ptr<Process> >(processName, process));
    mutex.unlock();

    // If watcher thread runs execute this process thread
    StartDetachedProcess(process);
}

void ThreadWatcher::Watcher::RemoveProcess(const std::string &processName) {
    StopActiveThread(processName);
    mutex.lock();
    processes.erase(processName);
    mutex.unlock();
}

void ThreadWatcher::Watcher::Process(const std::stop_token &stopToken) {
    while (!stopToken.stop_requested()) {
        mutex.lock();
        for (auto const &x: processes) {

            std::shared_ptr<Process> process = x.second;

            // Check if a new thread is add or update status from all.
            if (process->HasStatus(Process::Status::INIT)) {
                StartDetachedProcess(process);
            }
            else {
                process->UpdateStatus();
            }
        }
        mutex.unlock();
        std::this_thread::sleep_for(5s);
    }
}

void ThreadWatcher::Watcher::Stop() {
    mutex.lock();
    for (auto const &x: processes) {
        std::shared_ptr<Process> process = x.second;
        if (process->HasStatus(Process::Status::RUN)) {
            process->Stop();
        }
    }
    mutex.unlock();
}

std::shared_ptr<ThreadWatcher::Process> ThreadWatcher::Watcher::GetProcess(const std::string &processName) {
    mutex.lock();
    std::shared_ptr<Process> process = processes.find(processName)->second;
    mutex.unlock();
    return process;
}

void ThreadWatcher::Watcher::StopActiveThread(const std::string &processName) {
    if (ExistsProcess(processName)) {
        std::shared_ptr<Process> ptrProcess = GetProcess(processName);
        if (ptrProcess->HasStatus(Process::Status::RUN))
            ptrProcess->Stop();
    }
}

void ThreadWatcher::Watcher::StartDetachedProcess(const std::shared_ptr<Process> &process) {
    std::thread pThread(&Process::Start, process);
    pThread.detach();
}
