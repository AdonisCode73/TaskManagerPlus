cmake_minimum_required(VERSION 3.10)
project(TaskManagerPlus VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# User option to toggle AMD support (ON = AMD, OFF = NVIDIA)
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

# Set compile definitions for Unicode support
target_compile_definitions(TaskManagerPlus PRIVATE UNICODE _UNICODE)

# GPU backend selection
if(USE_AMD)
    target_compile_definitions(TaskManagerPlus PRIVATE USE_AMD)
else()
    target_compile_definitions(TaskManagerPlus PRIVATE USE_NVIDIA)

    # Locate CUDA include path and nvml.lib for NVIDIA GPU monitoring
    find_path(CUDA_INCLUDE_DIR nvml.h
        PATHS
            "$ENV{CUDA_PATH}/include"
            "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.0/include"
        NO_DEFAULT_PATH
    )

    find_library(NVML_LIBRARY nvml
        PATHS
            "$ENV{CUDA_PATH}/lib/x64"
            "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.0/lib/x64"
        NO_DEFAULT_PATH
    )

    if(NOT CUDA_INCLUDE_DIR OR NOT NVML_LIBRARY)
        message(FATAL_ERROR "NVIDIA support is enabled, but nvml.h or nvml.lib not found. Set CUDA_PATH properly.")
    endif()

    target_include_directories(TaskManagerPlus PRIVATE ${CUDA_INCLUDE_DIR})
    target_link_libraries(TaskManagerPlus PRIVATE ${NVML_LIBRARY})
endif()

# Link system and custom libraries
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
