cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_PROJECT_NAME              components)
##########################################
### Export CMakeLists.txt for Components
##########################################
project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE
    bmi088 ist8310 mahony
)



##########################################
### BMI088 Driver
##########################################

add_library(bmi088 INTERFACE)

file(GLOB BMI088_SRC bmi088/*.c)

target_compile_definitions(bmi088 INTERFACE)

target_include_directories(bmi088 INTERFACE
    bmi088/Include
)

target_sources(bmi088 INTERFACE ${BMI088_SRC})


##########################################
### BMI088 Driver
##########################################

add_library(ist8310 INTERFACE)

file(GLOB IST8310_SRC ist8310/*.c)

target_compile_definitions(ist8310 INTERFACE)

target_include_directories(ist8310 INTERFACE
    ist8310/Include
)

target_sources(ist8310 INTERFACE ${IST8310_SRC})


##########################################
### MagonyAHRS Driver
##########################################

add_library(mahony INTERFACE)

file(GLOB MAHONYAHRS MahonyAHRS/*.c)

target_compile_definitions(mahony INTERFACE)

target_include_directories(mahony INTERFACE
    MahonyAHRS
)

target_sources(mahony INTERFACE ${MAHONYAHRS})
