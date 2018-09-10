set(SOURCE_FILES ThreadWatcher/IFunction.h
                 ThreadWatcher/Process.h ThreadWatcher/Process.cpp
                 ThreadWatcher/ThreadWatcher.h ThreadWatcher/ThreadWatcher.cpp)

add_library(ThreadWatcher STATIC ${SOURCE_FILES})

target_include_directories(ThreadWatcher PUBLIC ${PROJECT_SOURCE_DIR}/src)
