cmake_minimum_required(VERSION 3.15)  # CMake version check
project(RecipeManagerExecutable)     # Project name

set(CMAKE_CXX_STANDARD 20)            # Enable C++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # Enforce C++20 compliance
set(CMAKE_CXX_EXTENSIONS OFF)         # Disable compiler-specific extensions

# Set source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Include directories for headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Locate SQLite3
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

# Source files for the main application
set(MAIN_APP_SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Fridge.cpp
    ${SOURCE_DIR}/Ingredient.cpp
    ${SOURCE_DIR}/Pantry.cpp
    ${SOURCE_DIR}/Recipe.cpp
    ${SOURCE_DIR}/RecipeManager.cpp
    ${SOURCE_DIR}/Storage.cpp
)

# Source files for the database setup executable
set(DATABASE_SETUP_SOURCES
    ${SOURCE_DIR}/build_database.cpp
)

# Create the main application executable
add_executable(RecipeManagerExecutable ${MAIN_APP_SOURCES})
target_link_libraries(RecipeManagerExecutable ${SQLite3_LIBRARIES})

# Create the database setup executable
add_executable(BuildDatabaseExecutable ${DATABASE_SETUP_SOURCES})
target_link_libraries(BuildDatabaseExecutable ${SQLite3_LIBRARIES})

# Establish JSON data files
set(DATA_FILES
    ${CMAKE_SOURCE_DIR}/data/history.json
    ${CMAKE_SOURCE_DIR}/data/recipes.json
    ${CMAKE_SOURCE_DIR}/data/storage.json
)

# Add JSON files to the build directory after the main application executable is built
foreach(file ${DATA_FILES})
    add_custom_command(TARGET RecipeManagerExecutable POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${file}" $<TARGET_FILE_DIR:RecipeManagerExecutable>
    )
endforeach()
