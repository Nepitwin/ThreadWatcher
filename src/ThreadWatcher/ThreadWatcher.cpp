#include "ThreadWatcher.h"

ThreadWatcher::ThreadWatcher()
{
	std::shared_ptr<IFunction> ptrFunction(this);
	this->watcherThread = new Process(ptrFunction);
	this->exitWatcher = false;
}

ThreadWatcher::~ThreadWatcher()
{
}

bool ThreadWatcher::hasProcessStatus(std::string processName, Process::Status status)
{	
	if (existsProcess(processName))
	{
		return getProcess(processName)->hasStatus(status);
	}

	return false;
}

bool ThreadWatcher::existsProcess(std::string processName)
{
	mutex.lock();
	std::map<std::string, std::shared_ptr<Process>>::iterator it = threads.find(processName);
	mutex.unlock();
	return it != threads.end();
}

void ThreadWatcher::addProcess(std::string processName, std::shared_ptr<Process> process)
{
	stopActiveThread(processName);
	mutex.lock();
	threads.insert(std::pair<std::string, std::shared_ptr<Process>>(processName, process));
	mutex.unlock();

	// If watcher thread runs execute this process thread
	if (this->watcherThread->hasStatus(Process::Status::RUN))
		startDetachedProcess(process);
}

void ThreadWatcher::removeProcess(std::string processName)
{
	stopActiveThread(processName);
	mutex.lock();
	threads.erase(processName);
	mutex.unlock();
}

void ThreadWatcher::watch()
{
	if (this->watcherThread->hasStatus(Process::Status::INIT))
	{
		// Start watcher thread to watch all sub processes
		this->watcherThread->start();
	}
}

void ThreadWatcher::process()
{
	while (!exitWatcher)
	{
		mutex.lock();
		for (auto const& x : threads)
		{
			std::shared_ptr<Process> process = x.second;
			// Check if a new thread is add or update status from all.
			if (process->hasStatus(Process::Status::INIT))
				startDetachedProcess(process);	
			else
				process->updateStatus();
		}
		mutex.unlock();
		std::this_thread::sleep_for(5s);
	}
}

void ThreadWatcher::stop()
{
	mutex.lock();
	for (auto const& x : threads)
	{
		std::shared_ptr<Process> process = x.second;
		if (process->hasStatus(Process::Status::RUN))
			process->stop();
	}
	exitWatcher = true;
	mutex.unlock();
}

std::shared_ptr<Process> ThreadWatcher::getProcess(std::string processName)
{
	mutex.lock();
	std::shared_ptr<Process> process = threads.find(processName)->second;
	mutex.unlock();
	return process;
}

void ThreadWatcher::stopActiveThread(std::string processName)
{
	if (existsProcess(processName))
	{
		std::shared_ptr<Process> ptrProcess = getProcess(processName);
		if (ptrProcess->hasStatus(Process::Status::RUN))
			ptrProcess->stop();
	}
}

void ThreadWatcher::startDetachedProcess(std::shared_ptr<Process> process)
{
	std::thread pThread(&Process::start, process);
	pThread.detach();
}
