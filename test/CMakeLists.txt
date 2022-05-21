add_executable(ThreadWatcherTesting main.cpp
									TestProcess.cpp
								    TestThreadWatcher.cpp
									Util.h)

target_include_directories(ThreadWatcherTesting PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(ThreadWatcherTesting PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main ThreadWatcher)
add_test(NAME ThreadWatcherTesting COMMAND $<TARGET_FILE:ThreadWatcherTesting>)