cmake_minimum_required(VERSION 3.15)  # CMake version check
project(RecipeManagerExecutable)     # Creates project under this name

set(CMAKE_CXX_STANDARD 20)            # Enable c++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) #ensures uniform compiler behavior between macos and windows
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")   # Enables execptoin handling
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")  # path handling - windows uses \, mac uses / --> this sets it to /



#includes our header&nlohmann_json files  --> lets test, but we can likely remove "../include/json.hpp" and turn to #include "json.hpp" now
#include_directories(include)
#include_directories(json/include)
include_directories(${CMAKE_SOURCE_DIR}/include)



#includes our src (.cpp) files
set(SOURCE_FILES
        src/main.cpp
        src/Fridge.cpp
        src/Ingredient.cpp
        src/Pantry.cpp
        src/Recipe.cpp
        src/RecipeManager.cpp
        src/Storage.cpp)

# Add executable target with source files listed above
add_executable(RecipeManagerExecutable ${SOURCE_FILES})

#ensures this package is required across all platforms - nlohmann_json
#add_subdirectory(json)
#find_package(nlohmann_json REQUIRED)
#target_link_libraries(RecipeManagerExecutable PRIVATE nlohmann_json::nlohmann_json)


# establishes our data files
set(DATA_FILES
        ${CMAKE_SOURCE_DIR}/data/history.json
        ${CMAKE_SOURCE_DIR}/data/recipes.json
        ${CMAKE_SOURCE_DIR}/data/storage.json
)
#adds json files to build directory after the executable is built
foreach(file ${DATA_FILES})
    add_custom_command(TARGET RecipeManagerExecutable POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${file}" $<TARGET_FILE_DIR:RecipeManagerExecutable>
    )
endforeach()

#this should work, but if we encounter issues between platforms, check our CMAKE_SOURCE_DIR and TARGET_FILE_DIR paths