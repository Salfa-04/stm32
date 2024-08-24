cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_PROJECT_NAME              components)
##########################################
### Export CMakeLists.txt for Components
##########################################
project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    imu901 fsetting tb6612 mctrl
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

##########################################
### TB6612 Motor Driver
##########################################

add_library(tb6612 INTERFACE)

file(GLOB COMP_MOTOR TB6612/*.c)

target_compile_definitions(tb6612 INTERFACE
)

target_include_directories(tb6612 INTERFACE
    TB6612/Include
)

target_sources(tb6612 INTERFACE
    ${COMP_MOTOR}
)

##########################################
### Motor Control Driver
##########################################

add_library(mctrl INTERFACE)

file(GLOB COMP_MCTRL MCTRL/*.c)

target_compile_definitions(mctrl INTERFACE
)

target_include_directories(mctrl INTERFACE
    MCTRL/Include
)

target_sources(mctrl INTERFACE
    ${COMP_MCTRL}
)
