cmake_minimum_required(VERSION 2.8)
SET( CMAKE_CXX_COMPILER /opt/kalmar/bin/clang++)
add_definitions("`clamp-config --install --cxxflags --ldflags`")
add_executable(Main
	main.cpp
	accelerator/accelerator_test.cpp
)

