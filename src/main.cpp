#include <string>
#include "../include/json.hpp"
#include "../include/RecipeManager.h"

using json = nlohmann::json;

int main() {
    const std::string dbName = "data.db";
    RecipeManager manager(dbName);
    manager.menu();
    return 0;
}

