cmake_minimum_required(VERSION 3.20)
project(WinMMM10Editor VERSION 1.0.0 LANGUAGES CXX)

# ============================================================================
# C++ Standard Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Build Type Configuration
# ============================================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available build types" FORCE)

# ============================================================================
# Compiler Options and Warnings
# ============================================================================
if(MSVC)
    # Common compiler flags for all configurations
    add_compile_options(/MP)  # Multi-processor compilation
    add_compile_options(/W4)  # Warning level 4
    add_compile_options(/permissive-)  # Conformance mode
    
    # Release configuration flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL" CACHE STRING "Release compiler flags" FORCE)  # Maximize speed + whole program optimization
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG" CACHE STRING "Release linker flags" FORCE)  # Link-time code generation (LTO)
    
    # Debug configuration flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi /RTC1" CACHE STRING "Debug compiler flags" FORCE)  # Disable optimizations + debug info + runtime checks
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG" CACHE STRING "Debug linker flags" FORCE)  # Debug linker information
else()
    # GCC/Clang flags
    add_compile_options(-Wall -Wextra -Wpedantic)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -DNDEBUG)
    else()
        add_compile_options(-g -O0)
    endif()
endif()

# ============================================================================
# Qt Configuration
# ============================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try to find Qt6 in common Windows locations if CMAKE_PREFIX_PATH is not set
if(WIN32 AND NOT DEFINED ENV{Qt6_DIR} AND NOT DEFINED CMAKE_PREFIX_PATH)
    # Common Qt installation paths on Windows (check multiple versions and compilers)
    set(QT_POSSIBLE_PATHS
        "C:/Qt/6.10.1/msvc2022_64"
        "C:/Qt/6.10.1/msvc2019_64"
        "C:/Qt/6.10.0/msvc2022_64"
        "C:/Qt/6.10.0/msvc2019_64"
        "C:/Qt/6.7.0/msvc2022_64"
        "C:/Qt/6.7.0/msvc2019_64"
        "C:/Qt/6.6.0/msvc2022_64"
        "C:/Qt/6.6.0/msvc2019_64"
        "C:/Qt/6.5.0/msvc2022_64"
        "C:/Qt/6.5.0/msvc2019_64"
        "C:/Qt/6.4.0/msvc2022_64"
        "C:/Qt/6.4.0/msvc2019_64"
        "C:/Qt/6.10.1/mingw_64"
        "C:/Qt/6.10.0/mingw_64"
    )
    
    foreach(QT_PATH ${QT_POSSIBLE_PATHS})
        if(EXISTS "${QT_PATH}/lib/cmake/Qt6/Qt6Config.cmake")
            list(PREPEND CMAKE_PREFIX_PATH "${QT_PATH}")
            message(STATUS "Found Qt6 at: ${QT_PATH}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    OpenGL 
    OpenGLWidgets
    Json
)

# ============================================================================
# Application Version
# ============================================================================
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 0)
set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

# ============================================================================
# Source File Organization
# ============================================================================

# Source directories
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/core)
set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/ui)
set(MAPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/maps)
set(BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/binary)
set(HEURISTICS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/heuristics)
set(VALIDATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/validation)
set(MAPPACKS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/mappacks)
set(PLUGINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins)

# Core sources
set(CORE_SOURCES
    ${CORE_DIR}/Application.cpp
    ${CORE_DIR}/Project.cpp
    ${CORE_DIR}/ProjectManager.cpp
    ${CORE_DIR}/Settings.cpp
)

set(CORE_HEADERS
    ${CORE_DIR}/Application.h
    ${CORE_DIR}/Project.h
    ${CORE_DIR}/ProjectManager.h
    ${CORE_DIR}/Settings.h
)

# Binary sources
set(BINARY_SOURCES
    ${BINARY_DIR}/BinaryFile.cpp
    ${BINARY_DIR}/MemoryMapper.cpp
    ${BINARY_DIR}/EditHistory.cpp
    ${BINARY_DIR}/Checksum.cpp
)

set(BINARY_HEADERS
    ${BINARY_DIR}/BinaryFile.h
    ${BINARY_DIR}/MemoryMapper.h
    ${BINARY_DIR}/Endianness.h
    ${BINARY_DIR}/EditHistory.h
    ${BINARY_DIR}/Checksum.h
)

# Map sources
set(MAPS_SOURCES
    ${MAPS_DIR}/MapDefinition.cpp
    ${MAPS_DIR}/MapAxis.cpp
    ${MAPS_DIR}/Map2D.cpp
    ${MAPS_DIR}/Map3D.cpp
)

set(MAPS_HEADERS
    ${MAPS_DIR}/MapDefinition.h
    ${MAPS_DIR}/MapAxis.h
    ${MAPS_DIR}/Map2D.h
    ${MAPS_DIR}/Map3D.h
    ${MAPS_DIR}/ScalingEngine.h
)

# Heuristics sources
set(HEURISTICS_SOURCES
    ${HEURISTICS_DIR}/MapDetector.cpp
    ${HEURISTICS_DIR}/PatternAnalyzer.cpp
)

set(HEURISTICS_HEADERS
    ${HEURISTICS_DIR}/MapDetector.h
    ${HEURISTICS_DIR}/PatternAnalyzer.h
)

# Validation sources
set(VALIDATION_SOURCES
    ${VALIDATION_DIR}/Validator.cpp
    ${VALIDATION_DIR}/DifferenceCalculator.cpp
)

set(VALIDATION_HEADERS
    ${VALIDATION_DIR}/Validator.h
    ${VALIDATION_DIR}/DifferenceCalculator.h
)

# MapPack sources
set(MAPPACKS_SOURCES
    ${MAPPACKS_DIR}/MapPack.cpp
)

set(MAPPACKS_HEADERS
    ${MAPPACKS_DIR}/MapPack.h
)

# Plugin sources
set(PLUGINS_SOURCES
    ${PLUGINS_DIR}/PluginManager.cpp
)

set(PLUGINS_HEADERS
    ${PLUGINS_DIR}/PluginInterface.h
    ${PLUGINS_DIR}/PluginManager.h
)

# UI sources
set(UI_SOURCES
    ${UI_DIR}/MainWindow.cpp
    ${UI_DIR}/HexEditor.cpp
    ${UI_DIR}/HexEditorWidget.cpp
    ${UI_DIR}/Map2DViewer.cpp
    ${UI_DIR}/Map3DViewer.cpp
    ${UI_DIR}/MapDefinitionDialog.cpp
    ${UI_DIR}/ProjectDialog.cpp
    ${UI_DIR}/MapListWidget.cpp
    ${UI_DIR}/StatusBar.cpp
    ${UI_DIR}/DarkTheme.cpp
    ${UI_DIR}/DockManager.cpp
)

set(UI_HEADERS
    ${UI_DIR}/MainWindow.h
    ${UI_DIR}/HexEditor.h
    ${UI_DIR}/HexEditorWidget.h
    ${UI_DIR}/Map2DViewer.h
    ${UI_DIR}/Map3DViewer.h
    ${UI_DIR}/MapDefinitionDialog.h
    ${UI_DIR}/ProjectDialog.h
    ${UI_DIR}/MapListWidget.h
    ${UI_DIR}/StatusBar.h
    ${UI_DIR}/DarkTheme.h
    ${UI_DIR}/DockManager.h
)

# Main application
set(MAIN_SOURCE src/main.cpp)

# All sources
set(ALL_SOURCES
    ${MAIN_SOURCE}
    ${CORE_SOURCES}
    ${BINARY_SOURCES}
    ${MAPS_SOURCES}
    ${HEURISTICS_SOURCES}
    ${VALIDATION_SOURCES}
    ${MAPPACKS_SOURCES}
    ${PLUGINS_SOURCES}
    ${UI_SOURCES}
)

set(ALL_HEADERS
    ${CORE_HEADERS}
    ${BINARY_HEADERS}
    ${MAPS_HEADERS}
    ${HEURISTICS_HEADERS}
    ${VALIDATION_HEADERS}
    ${MAPPACKS_HEADERS}
    ${PLUGINS_HEADERS}
    ${UI_HEADERS}
)

# ============================================================================
# Main Executable Target
# ============================================================================
add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_HEADERS})

# Precompiled Headers (PCH)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Json
)

# Windows specific
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.rc
    )
endif()

# ============================================================================
# Testing
# ============================================================================
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    enable_testing()
    
    set(TEST_SOURCES
        tests/TestMain.cpp
        tests/TestChecksum.cpp
        tests/TestMapDetection.cpp
        tests/TestMapPack.cpp
    )
    
    set(TEST_HEADERS
        tests/TestChecksum.h
        tests/TestMapDetection.h
        tests/TestMapPack.h
    )
    
    add_executable(${PROJECT_NAME}_Tests ${TEST_SOURCES} ${TEST_HEADERS})
    
    # Precompiled Headers for tests (optional but recommended)
    target_precompile_headers(${PROJECT_NAME}_Tests PRIVATE src/pch.h)
    
    target_link_libraries(${PROJECT_NAME}_Tests
        Qt6::Core
        Qt6::Test
        Qt6::Widgets
        Qt6::Json
    )
    target_include_directories(${PROJECT_NAME}_Tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
    
    add_test(NAME ${PROJECT_NAME}_Tests COMMAND ${PROJECT_NAME}_Tests)
endif()

# ============================================================================
# Installation
# ============================================================================
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Copy resources
install(DIRECTORY resources/ DESTINATION resources)

# Copy plugin directory structure
install(DIRECTORY plugins/ DESTINATION plugins OPTIONAL)
