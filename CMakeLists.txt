cmake_minimum_required(VERSION 3.10)
project(TaskManagerPlus VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_AMD "Use AMD ADLX for GPU monitoring" ON)

# Add executable and source files
add_executable(TaskManagerPlus
    TaskManagerPlus/Main.cpp
    TaskManagerPlus/CpuMonitor.cpp
    TaskManagerPlus/DiskMonitor.cpp
    TaskManagerPlus/GpuMonitor.cpp
    TaskManagerPlus/MemoryMonitor.cpp
    TaskManagerPlus/MonitorUtils.cpp
    TaskManagerPlus/NetworkMonitor.cpp
    TaskManagerPlus/GuiController.cpp
    TaskManagerPlus/NVIDIAController.cpp
    TaskManagerPlus/ADLXController.cpp
    TaskManagerPlus/Include/ADLXHelper.cpp
    TaskManagerPlus/Include/WinAPIs.cpp
)

# Add include directories
target_include_directories(TaskManagerPlus PRIVATE
    ${CMAKE_SOURCE_DIR}/TaskManagerPlus/Include/
    ${CMAKE_SOURCE_DIR}/TaskManagerPlus/Include/ADLX
    ${CMAKE_SOURCE_DIR}/TaskManagerPlus/Include/CL
)

target_compile_definitions(TaskManagerPlus PRIVATE UNICODE _UNICODE)

if(USE_AMD)
    target_compile_definitions(TaskManagerPlus PRIVATE USE_AMD)
else()
    target_compile_definitions(TaskManagerPlus PRIVATE USE_NVIDIA)
endif()

target_link_libraries(TaskManagerPlus PRIVATE
    "${CMAKE_SOURCE_DIR}/TaskManagerPlus/lib/OpenCL.lib"
    "${CMAKE_SOURCE_DIR}/TaskManagerPlus/lib/pdcurses.lib"
    Pdh
    iphlpapi
    kernel32
    user32
    gdi32
    advapi32
)
