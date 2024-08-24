cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_PROJECT_NAME              components)
##########################################
### Export CMakeLists.txt for Components
##########################################
project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    imu901
)



##########################################
### Example Driver
##########################################

add_library(imu901 INTERFACE)

file(GLOB COMPS_IMU901 IMU901/*.c)

target_compile_definitions(imu901 INTERFACE
)

target_include_directories(imu901 INTERFACE
    IMU901
)

target_sources(imu901 INTERFACE
    ${COMPS_IMU901}
)
