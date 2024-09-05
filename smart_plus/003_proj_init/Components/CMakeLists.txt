cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_PROJECT_NAME              components)
##########################################
### Export CMakeLists.txt for Components
##########################################
project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    hwt101 fsetting motor mctrl
)



##########################################
### HWT101 Driver
##########################################

add_library(hwt101 INTERFACE)
file(GLOB COMP_HWT101 HWT101/*.c)
target_include_directories(hwt101 INTERFACE
    HWT101
)

target_sources(hwt101 INTERFACE
    ${COMP_HWT101}
)

##########################################
### Setting Save Driver
##########################################

add_library(fsetting INTERFACE)
file(GLOB COMP_SETSAVE fSetting/*.c)
target_include_directories(fsetting INTERFACE
    fSetting
)

target_sources(fsetting INTERFACE
    ${COMP_SETSAVE}
)

##########################################
### TB6612 Motor Driver
##########################################

add_library(motor INTERFACE)
file(GLOB COMP_MOTOR Motor/*.c)
target_include_directories(motor INTERFACE
    Motor/Include
)

target_sources(motor INTERFACE
    ${COMP_MOTOR}
)

##########################################
### Motor Control Driver
##########################################

add_library(mctrl INTERFACE)
file(GLOB COMP_MCTRL MCTRL/*.c)
target_include_directories(mctrl INTERFACE
    MCTRL/Include
)

target_sources(mctrl INTERFACE
    ${COMP_MCTRL}
)
