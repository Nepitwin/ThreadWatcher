# Thread Watcher

Thread watcher is a C++ based implementation to monitor running threads by executions states. 

| Build         | Status        |
| ------------- |:-------------:|
| Windows       | [![Build status](https://ci.appveyor.com/api/projects/status/8lek5cqicj7ma49o?svg=true)](https://ci.appveyor.com/project/Nepitwin/threadwatcher) |
| Linux         | [![Build Status](https://travis-ci.org/Nepitwin/ThreadWatcher.svg?branch=master)](https://travis-ci.org/Nepitwin/ThreadWatcher) |


![Thread Watcher][watcher]

## Process states

Eache processing thread starts by an Init state if only created but not started from thread watcher. If thread watcher is running all process will be automatic executed and has the running state. By stopping an process it trys to stop this thread clean (finished) and if it's not work hard killed (killed). If both methods to kill not work an killed exception status will be set to process.

![Process states][states]

## Build

This project can be build with cmake for all platforms. All dependencies will be downloaded from hunter repository. Currently the source code part to kill processes hard is not platform independent and use the Windows API. 

Build with ccmake
```
ccmake . 
```

Build with cmake
```
cd build
cmake ..
```

### Example

This example codes shows a simple usage from thread watcher.

First of all code an process class which implements the IFunction interface.

```
class CleanProcess : public ::IFunction
{

public:

    CleanProcess()
    {
        stopProcess = false;
    }

    virtual ~CleanProcess() {};

    /**
     * Overridden function.
     */
    void process() override
    {
        while (!stopProcess) 
        {
             // Implement here your processing code
        }
    }

    /**
     * Overridden function.
     */
    void stop() override
    {
        stopProcess = true;
    }

private:

    /**
     * @brief Atomic boolean to stop process.
     */
    std::atomic<bool> stopProcess;
};
```

Use this processing in an thread watcher to monitor his running states and start watcher to watch all added processes.

```
ThreadWatcher watcher;
std::shared_ptr<IFunction> ptrProcess(new CleanProcess());
std::shared_ptr<Process> processClean(new Process(ptrProcess));
watcher.addProcess("PROCESS_A39", processClean);
// Added process will be in state init if watcher not started.
watcher.hasProcessStatus("PROCESS_A39", Process::Status::INIT)

// Important this call will block the execution by thread.join() method from watcher.
watcher.watch();

// If watcher is started process should be in state run.
watcher.hasProcessStatus("PROCESS_A39", Process::Status::RUN)
```

## License

Copyright 2017-2018 Andreas Sekulski

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

[states]: Process_States.png
[watcher]: ThreadWatcher.png
