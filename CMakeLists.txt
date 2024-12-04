cmake_minimum_required(VERSION 3.15)  # CMake version check
project(RecipeManagerExecutable)     # Project name

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Include directories for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

# Add source files for the main application executable
set(MAIN_APP_SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Fridge.cpp
    ${SOURCE_DIR}/Ingredient.cpp
    ${SOURCE_DIR}/Pantry.cpp
    ${SOURCE_DIR}/Recipe.cpp
    ${SOURCE_DIR}/RecipeManager.cpp
    ${SOURCE_DIR}/Storage.cpp
)

# Add source files for the database setup executable
set(DATABASE_SETUP_SOURCES
    ${SOURCE_DIR}/build_database.cpp
)

# Create the main application executable
add_executable(RecipeManagerExecutable ${MAIN_APP_SOURCES})
target_link_libraries(RecipeManagerExecutable PRIVATE ${SQLite3_LIBRARIES} nlohmann_json::nlohmann_json Threads::Threads)
find_package(CURL REQUIRED)
target_link_libraries(RecipeManagerExecutable PRIVATE CURL::libcurl)


# Create the database setup executable
add_executable(BuildDatabaseExecutable ${DATABASE_SETUP_SOURCES})
target_link_libraries(BuildDatabaseExecutable PRIVATE ${SQLite3_LIBRARIES} nlohmann_json::nlohmann_json Threads::Threads)

# Establish JSON data files
set(DATA_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/data/history.json
    ${CMAKE_CURRENT_SOURCE_DIR}/data/recipes.json
    ${CMAKE_CURRENT_SOURCE_DIR}/data/storage.json
)

# Copy JSON files to the build directory after the main application executable is built
foreach(file ${DATA_FILES})
    add_custom_command(TARGET RecipeManagerExecutable POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${file}" $<TARGET_FILE_DIR:RecipeManagerExecutable>
    )
endforeach()

# Copy JSON files to the build directory for database setup executable
foreach(file ${DATA_FILES})
    add_custom_command(TARGET BuildDatabaseExecutable POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${file}" $<TARGET_FILE_DIR:BuildDatabaseExecutable>
    )
endforeach()

# Add include directories to both executables
target_include_directories(RecipeManagerExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(BuildDatabaseExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optional: Display build settings
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
