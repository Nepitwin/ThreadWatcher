#include "ProcessThread.h"

#include <utility>

Threadwatcher::Process::Process(std::shared_ptr<IFunction> function) : function(std::move(function))
{
	this->processStatus = Process::Status::INIT;
}

void Threadwatcher::Process::Start()
{
	// Start std::thread here
	this->thread = std::thread(&::Threadwatcher::IFunction::Process, function);
	if (thread.joinable())
	{
        this->processStatus = Process::Status::RUN;
		// Join running thread and if closed Stop it
		this->thread.join();
	}
}

void Threadwatcher::Process::Stop()
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
			this->processStatus = Process::Status::FINISHED;
		}
		else
        {
            this->processStatus = Process::Status::RUNS_AFTER_KILL;
        }
	}
}

bool Threadwatcher::Process::HasStatus(Process::Status status)
{
	return this->processStatus == status;
}

bool Threadwatcher::Process::IsRunning()
{
	return thread.get_id() != std::thread::id();
}

void Threadwatcher::Process::UpdateStatus()
{
	if (!IsRunning() && this->processStatus != Process::Status::FINISHED && this->processStatus != Process::Status::RUNS_AFTER_KILL)
	{
		this->processStatus = Process::Status::KILLED;
	}
}
