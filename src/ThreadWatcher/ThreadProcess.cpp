#include "ThreadProcess.h"

#include <utility>

ThreadWatcher::ThreadProcess::ThreadProcess(std::shared_ptr<ThreadWatcher::IFunction> function) : function(std::move(function))
{
	this->processStatus = ThreadProcess::Status::INIT;
}

void ThreadWatcher::ThreadProcess::Start()
{
	// Start std::thread here
	this->thread = std::thread(&::ThreadWatcher::IFunction::Process, function);
	if (thread.joinable())
	{
        this->processStatus = ThreadProcess::Status::RUN;
		// Join running thread and if closed Stop it
		this->thread.join();
	}
}

void ThreadWatcher::ThreadProcess::Stop()
{
	if (IsRunning())
	{
		// Try to Stop thread clean
		this->function->Stop();
		// Time for thread to Stop
		std::this_thread::sleep_for(2s);
		// Check if thread is still running
		if (!IsRunning())
		{
			this->processStatus = ThreadProcess::Status::FINISHED;
		}
		else
        {
            this->processStatus = ThreadProcess::Status::RUNS_AFTER_KILL;
        }
	}
}

bool ThreadWatcher::ThreadProcess::HasStatus(ThreadProcess::Status status)
{
	return this->processStatus == status;
}

bool ThreadWatcher::ThreadProcess::IsRunning()
{
	return thread.get_id() != std::thread::id();
}

void ThreadWatcher::ThreadProcess::UpdateStatus()
{
	if (!IsRunning() && this->processStatus != ThreadProcess::Status::FINISHED && this->processStatus != ThreadProcess::Status::RUNS_AFTER_KILL)
	{
		this->processStatus = ThreadProcess::Status::KILLED;
	}
}
