cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(mark_simulator)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")

find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# Find all executables
include_directories(include)
file(GLOB CPPS src/*.cpp)

# Add project executable
add_executable(mark_simulator ${CPPS})
target_link_libraries(mark_simulator ${OpenCV_LIBRARIES})

