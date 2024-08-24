cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_PROJECT_NAME              components)
##########################################
### Export CMakeLists.txt for Components
##########################################
project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    imu901 fsetting
)



##########################################
### IMU901 Driver
##########################################

add_library(imu901 INTERFACE)

file(GLOB COMP_IMU901 IMU901/*.c)

target_compile_definitions(imu901 INTERFACE
)

target_include_directories(imu901 INTERFACE
    IMU901/Include
)

target_sources(imu901 INTERFACE
    ${COMP_IMU901}
)

##########################################
### Setting Save Driver
##########################################

add_library(fsetting INTERFACE)

file(GLOB COMP_SETSAVE fSetting/*.c)

target_compile_definitions(fsetting INTERFACE
)

target_include_directories(fsetting INTERFACE
    fSetting
)

target_sources(fsetting INTERFACE
    ${COMP_SETSAVE}
)
