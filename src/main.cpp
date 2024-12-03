#include <string>
#include "json/json.hpp"
#include "RecipeManager.h"


using json = nlohmann::json;

int main() {
    std::string recipeFilename = "../data/recipes.json";
    RecipeManager manager(recipeFilename);

    manager.menu();

    return 0;
}
