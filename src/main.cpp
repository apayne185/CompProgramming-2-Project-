#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "../include/json.hpp"
#include "../include/RecipeManager.h"
#include "../include/UIManager.h"

using json = nlohmann::json;

int main() {
    std::string recipeFilename = "../data/recipes.json";
    std::fstream recipeFile(recipeFilename);
    if (!recipeFile.is_open()) {
        std::cerr << "Error opening recipe file: " << recipeFilename << std::endl;
        return 1;
    }
    recipeFile.close();

    try{
        RecipeManager manager(recipeFilename);
        manager.menu();
    } catch (const std::ifstream::failure& e) {
          std::cerr << "File error: " << e.what() << "\n";
          return 1;
    } catch (const std::runtime_error& e) {
          std::cerr << "Runtime error: " << e.what() << "\n";
          return 1;
    } catch (const std::exception& e) {
          std::cerr << "An unexpected error occured: " << e.what() << "\n";
          return 1;
    }

    return 0;
}
