#include "Process.h"

Process::Process(std::shared_ptr<IFunction> function) : function(function)
{
	this->processStatus = Process::Status::INIT;
}

Process::~Process()
{
}

void Process::start()
{
	// Start std::thread here
	this->thread = std::thread(&IFunction::process, function);
	if (thread.joinable())
	{
        this->processStatus = Process::Status::RUN;
		// Join running thread and if closed stop it
		this->thread.join();
	}
}

void Process::stop()
{
	if (isRunning())
	{
		// Try to stop thread clean
		this->function->stop();
		// Time for thread to stop
		std::this_thread::sleep_for(2s);
		// Check if thread is still running
		if (!isRunning())
		{
			this->processStatus = Process::Status::FINISHED;
		}
		else
        {
            this->processStatus = Process::Status::RUNS_AFTER_KILL;
        }
	}
}

bool Process::hasStatus(Process::Status status)
{
	return this->processStatus == status;
}

bool Process::isRunning()
{
	return thread.get_id() != std::thread::id();
}

void Process::updateStatus()
{
	if (!isRunning() && this->processStatus != Process::Status::FINISHED && this->processStatus != Process::Status::RUNS_AFTER_KILL)
	{
		this->processStatus = Process::Status::KILLED;
	}
}
