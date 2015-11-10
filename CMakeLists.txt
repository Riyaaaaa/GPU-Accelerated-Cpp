cmake_minimum_required(VERSION 2.8)
SET( CMAKE_CXX_COMPILER /opt/kalmar/bin/clang++)
SET( CMAKE_CXX_FLAGS "-std=c++amp -L/opt/kalmar/lib -Wl,--rpath=/opt/kalmar/lib -lc++ -lcxxrt -ldl -lpthread -Wl,--whole-archive -lmcwamp -Wl,--no-whole-archive -std=c++amp -I/opt/kalmar/include -I/opt/kalmar/include/c++/v1")
add_executable(Main
	main.cpp
	accelerator/accelerator_test.cpp
)

