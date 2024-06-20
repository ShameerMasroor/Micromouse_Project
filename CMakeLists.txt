cmake_minimum_required(VERSION 3.20.0)
set(BOARD stm32f3_disco)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(Micromouse_Project)

# Add this line to include the header files in the 'include' directory
include_directories(${PROJECT_SOURCE_DIR}/../include)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/ultrasonic.c)
