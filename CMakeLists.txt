cmake_minimum_required(VERSION 3.10)
project(TaskManagerPlus VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_PLATFORM_SOURCES
    TaskManagerPlus/src/common/Main.cpp
    TaskManagerPlus/src/common/MonitorUtils.cpp
    TaskManagerPlus/src/common/GuiController.cpp
)

add_executable(TaskManagerPlus ${COMMON_PLATFORM_SOURCES})

target_include_directories(TaskManagerPlus PRIVATE
    TaskManagerPlus/Include/common
)

if(WIN32)
    option(USE_AMD "Use AMD ADLX for GPU monitoring" ON)

    target_sources(TaskManagerPlus PRIVATE
        TaskManagerPlus/src/windows/CpuMonitor.cpp
        TaskManagerPlus/src/windows/DiskMonitor.cpp
        TaskManagerPlus/src/windows/GpuMonitor.cpp
        TaskManagerPlus/src/windows/MemoryMonitor.cpp
        TaskManagerPlus/src/windows/NetworkMonitor.cpp
        TaskManagerPlus/src/windows/ADLXController.cpp
        TaskManagerPlus/src/windows/NVIDIAController.cpp
        TaskManagerPlus/Include/windows/WinAPIs.cpp
        TaskManagerPlus/Include/windows/ADLXHelper.cpp
    )

    target_include_directories(TaskManagerPlus PRIVATE
        TaskManagerPlus/Include/windows
        TaskManagerPlus/Include/windows/ADLX
        TaskManagerPlus/Include/windows/CL
    )

    target_compile_definitions(TaskManagerPlus PRIVATE
        UNICODE _UNICODE
        PLATFORM_WINDOWS
    )
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

    target_link_libraries(TaskManagerPlus PRIVATE
        "${CMAKE_SOURCE_DIR}TaskManagerPlus/lib/windows/pdcurses.lib"
        Pdh
        iphlpapi
        kernel32
        user32
        gdi32
        advapi32
    )
elseif(UNIX)
    target_sources(TaskManagerPlus PRIVATE
        TaskManagerPlus/src/linux/CpuMonitor.cpp
        TaskManagerPlus/src/linux/DiskMonitor.cpp
        TaskManagerPlus/src/linux/GpuMonitor.cpp
        TaskManagerPlus/src/linux/MemoryMonitor.cpp
        TaskManagerPlus/src/linux/NetworkMonitor.cpp
    )

    target_include_directories(TaskManagerPlus PRIVATE
        TaskManagerPlus/Include/linux
    )

    target_compile_definitions(TaskManagerPlus PRIVATE
        PLATFORM_LINUX
    )

    # Linux has no pdcurses — use ncurses from the system
    find_package(Curses REQUIRED)
    target_include_directories(TaskManagerPlus PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(TaskManagerPlus PRIVATE ${CURSES_LIBRARIES})

endif()

target_compile_options(TaskManagerPlus PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:
        -fsanitize=address,undefined -fno-omit-frame-pointer>
)

target_link_options(TaskManagerPlus PRIVATE
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:
        -fsanitize=address,undefined>
)
