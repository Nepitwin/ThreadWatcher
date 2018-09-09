set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(ThreadWatcherTesting main.cpp
									TestProcess.cpp
								    TestThreadWatcher.cpp
									Util.h)

target_include_directories(ThreadWatcherTesting PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(ThreadWatcherTesting GTest::main ThreadWatcher)
add_test(NAME ThreadWatcherTesting COMMAND $<TARGET_FILE:ThreadWatcherTesting>)