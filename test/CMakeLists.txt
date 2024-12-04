cmake_minimum_required(VERSION 3.15)

# Project name
project(IntegrationTestExecutable)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR}/../include)

# Locate SQLite3
find_package(SQLite3 REQUIRED)
if(SQLite3_FOUND)
    include_directories(${SQLite3_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "SQLite3 not found. Please install SQLite3.")
endif()

# Include FetchContent module for external dependencies
include(FetchContent)

# Download nlohmann/json library
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2  # Replace with the latest stable version
)

# Make the library available
FetchContent_MakeAvailable(json)

# Locate Threads library (needed for JSON or other libraries in multi-threaded mode)
find_package(Threads REQUIRED)

# Add source files for the test executable
set(TEST_SOURCES
    integration_test.cpp
)

# Create the integration test executable
add_executable(IntegrationTest ${TEST_SOURCES})
target_link_libraries(IntegrationTest PRIVATE ${SQLite3_LIBRARIES} nlohmann_json::nlohmann_json Threads::Threads)

# Establish JSON data files (used during tests)
set(TEST_DATA_FILE ${CMAKE_SOURCE_DIR}/../data/test_data.json)

# Copy JSON file to the build directory
add_custom_command(TARGET IntegrationTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${TEST_DATA_FILE}" $<TARGET_FILE_DIR:IntegrationTest>
)

# Add include directories to the test executable
target_include_directories(IntegrationTest PRIVATE ${CMAKE_SOURCE_DIR}/../include)

# Optional: Display build settings
message(STATUS "Test Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")