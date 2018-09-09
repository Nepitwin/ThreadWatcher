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
	this->processStatus = Process::Status::RUN;
	// Start std::thread here
	this->thread = std::thread(&IFunction::process, function);
	if (thread.joinable())
	{
		// Join running thread and if closed stop it
		this->native_handle = this->thread.native_handle();
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
		if (isRunning())
		{
			// ----- WINDOWS ONLY -------
			// https://docs.microsoft.com/en-us/windows/desktop/api/processthreadsapi/nf-processthreadsapi-terminatethread
			// If termination was not successfully try to kill hard thread process
			DWORD result = ::TerminateThread(native_handle, 1);
			if (result != 0)
			{
				this->processStatus = Process::Status::KILLED;
			}
			else
			{
				this->processStatus = Process::Status::KILLEXCEPTION;
			}
			// ----- WINDOWS ONLY -------
		}
		else
		{
			this->processStatus = Process::Status::FINISHED;
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
	if (!isRunning() && this->processStatus != Process::Status::FINISHED && this->processStatus != Process::Status::KILLEXCEPTION)
	{
		this->processStatus = Process::Status::KILLED;
	}
}
