#include <string>
#include "../include/json.hpp"
#include "../include/RecipeManager.h"

using json = nlohmann::json;

int main() {
    try {
        std::string recipeFilename = "../data/recipes.json";
        RecipeManager manager(recipeFilename);
        manager.menu();
    } catch (const std::exception& e) {
        std::cerr << "Critical error in main: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
