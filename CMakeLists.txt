cmake_minimum_required(VERSION 3.20)

project(ParkingMeter)

find_package(SystemCLanguage CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD}) 

add_executable(time_calc_test src/time_calc_test.cpp)
target_link_libraries(time_calc_test SystemC::systemc)

add_executable(controller_test src/controller_test.cpp)
target_link_libraries(controller_test SystemC::systemc)

include_directories("inc")
