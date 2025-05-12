#include "Process.h"

#include <utility>

ThreadWatcher::Process::Process() {
    // TODO -> thread
}

void ThreadWatcher::Process::Start() {
    // Start std::thread here
    /*
    this->thread = std::thread(this, function);
    if (thread.joinable()) {
        this->processStatus = RUN;
        // Join running thread and if closed Stop it
        this->thread.join();
    }
    */
}

void ThreadWatcher::Process::Stop() {
    if (IsRunning()) {
        /*
        // Try to Stop thread clean
        this->function->Stop();
        // Time for thread to Stop
        std::this_thread::sleep_for(2s);
        // Check if thread is still running
        if (!IsRunning()) {
            this->processStatus = FINISHED;
        } else {
            this->processStatus = RUNS_AFTER_KILL;
        }
        */
    }
}

bool ThreadWatcher::Process::HasStatus(const Status status) const {
    return _processStatus == status;
}

bool ThreadWatcher::Process::IsRunning() const {
    return _thread.get_id() != std::thread::id();
}

void ThreadWatcher::Process::UpdateStatus() {
    if (!IsRunning() && _processStatus != FINISHED && _processStatus != RUNS_AFTER_KILL) {
        _processStatus = KILLED;
    }
}
