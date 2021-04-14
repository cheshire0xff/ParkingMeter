cmake_minimum_required(VERSION 3.20)

project(ParkingMeter)

find_package(SystemCLanguage CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD}) 

file(GLOB src_files "src/*.cpp")
add_executable(parking_meter ${src_files})

target_link_libraries(parking_meter SystemC::systemc)

include_directories("inc")
