cmake_minimum_required(VERSION 3.1)

project(VladsTools VERSION 0.1 LANGUAGES CXX)

### VARIABLES ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### DIRS ###
include_directories(.)

### SUBDIRS ###
add_subdirectory(cpu_info)
add_subdirectory(hash)
add_subdirectory(network)
add_subdirectory(strings)
add_subdirectory(time)
