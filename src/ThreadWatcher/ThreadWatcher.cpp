#include "ThreadWatcher.h"

ThreadWatcher::Watcher::Watcher()
{
	std::shared_ptr<IFunction> ptrFunction(this);
	this->watcherThread = new ThreadProcess(ptrFunction);
	this->exitWatcher = false;
}

bool ThreadWatcher::Watcher::HasProcessStatus(const std::string& processName, ThreadProcess::Status status)
{	
	if (ExistsProcess(processName))
	{
		return GetProcess(processName)->HasStatus(status);
	}

	return false;
}

bool ThreadWatcher::Watcher::ExistsProcess(const std::string& processName)
{
	mutex.lock();
	auto it = threads.find(processName);
	mutex.unlock();
	return it != threads.end();
}

void ThreadWatcher::Watcher::AddProcess(const std::string& processName, const std::shared_ptr<ThreadProcess>& process)
{
    StopActiveThread(processName);
	mutex.lock();
	threads.insert(std::pair<std::string, std::shared_ptr<ThreadProcess>>(processName, process));
	mutex.unlock();

	// If watcher thread runs execute this process thread
	if (this->watcherThread->HasStatus(ThreadProcess::Status::RUN))
        StartDetachedProcess(process);
}

void ThreadWatcher::Watcher::RemoveProcess(const std::string& processName)
{
    StopActiveThread(processName);
	mutex.lock();
	threads.erase(processName);
	mutex.unlock();
}

void ThreadWatcher::Watcher::Watch()
{
	if (this->watcherThread->HasStatus(ThreadProcess::Status::INIT))
	{
		// Start watcher thread to Watch all sub processes
        this->watcherThread->Start();
	}
}

void ThreadWatcher::Watcher::Process()
{
	while (!exitWatcher)
	{
		mutex.lock();
		for (auto const& x : threads)
		{
			std::shared_ptr<ThreadProcess> process = x.second;
			// Check if a new thread is add or update status from all.
			if (process->HasStatus(ThreadProcess::Status::INIT))
                StartDetachedProcess(process);
			else
                process->UpdateStatus();
		}
		mutex.unlock();
		std::this_thread::sleep_for(5s);
	}
}

void ThreadWatcher::Watcher::Stop()
{
	mutex.lock();
	for (auto const& x : threads)
	{
		std::shared_ptr<ThreadProcess> process = x.second;
		if (process->HasStatus(ThreadProcess::Status::RUN))
            process->Stop();
	}
	exitWatcher = true;
	mutex.unlock();
}

std::shared_ptr<ThreadWatcher::ThreadProcess> ThreadWatcher::Watcher::GetProcess(const std::string& processName)
{
	mutex.lock();
	std::shared_ptr<ThreadProcess> process = threads.find(processName)->second;
	mutex.unlock();
	return process;
}

void ThreadWatcher::Watcher::StopActiveThread(const std::string& processName)
{
	if (ExistsProcess(processName))
	{
		std::shared_ptr<ThreadProcess> ptrProcess = GetProcess(processName);
		if (ptrProcess->HasStatus(ThreadProcess::Status::RUN))
            ptrProcess->Stop();
	}
}

void ThreadWatcher::Watcher::StartDetachedProcess(const std::shared_ptr<ThreadProcess>& process)
{
	std::thread pThread(&ThreadProcess::Start, process);
	pThread.detach();
}
