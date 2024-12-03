#include "RecipeManager.h"
#include <fstream>
#include "json/json.hpp"

//using json = nlohmann::json;

RecipeManager::RecipeManager(const std::string& recipeFilename) {
    recipes = loadRecipesFromJSON(recipeFilename);
    loadIngredientsFromFile("../data/storage.json");
}


void RecipeManager::saveHistory(const Recipe& recipe) {
    std::ifstream infile("../data/history.json");
    json history;

    if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
        infile >> history;
    }
    infile.close();

    if (!history.is_array()) {
        history = json::array();
    }

    json j;
    j["name"] = recipe.getRecipeName();
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&now));
    j["date"] = std::string(buffer);
    
    history.push_back(j);

    std::ofstream outfile("../data/history.json");
    if (outfile.is_open()) {
        outfile << history.dump(4);
        outfile.close();
    }
}

void RecipeManager::displayFullRecipe(const Recipe& recipe) {
    std::cout << "Recipe: " << recipe.getRecipeName() << "\n";
    std::cout << "Ingredients:\n";
    for (const auto& ingredient : recipe.getRequiredIngredients()) {
        std::cout << "- " << ingredient.first << ": " << ingredient.second << "\n";
    }

    std::cout << "Steps:\n";
    for (const auto& step : recipe.getSteps()) {
        std::cout << "- " << step << "\n";
    }
}

void RecipeManager::saveIngredientsToFile(const std::string& filename) {
    json j;
    j["Fridge"] = fridge.toJSON();
    j["Pantry"] = pantry.toJSON();

    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
        std::cout << "Ingredients saved to " << filename << "\n";
    } else {
        std::cerr << "Unable to open file " << filename << "\n";
    }
}


void RecipeManager::loadIngredientsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return;
    }

    json j;
    file >> j;

    if (j.contains("Fridge")) {
        fridge.fromJSON(j["Fridge"]);
    }

    if (j.contains("Pantry")) {
        pantry.fromJSON(j["Pantry"]);
    }

    file.close();
    std::cout << "Ingredients loaded from " << filename << "\n";
}

void RecipeManager::collectIngredients() {
    while (true) {
        std::string name;
        int quantity;
        std::cout << "Please enter the ingredient name (or type 'done' to finish): ";
        std::cin.ignore();
        std::getline(std::cin, name);
        if (name == "done") break;

        std::cout << "Enter the quantity of " << name << ": ";
        std::cin >> quantity;
        Ingredient newIngredient(name, quantity, "");

        std::string storageLocation;
        std::cout << "Is the ingredient stored in the (F)ridge or (P)antry? ";
        std::cin >> storageLocation;

        if (storageLocation == "F" || storageLocation == "f") {
            std::string expirationDate;
            std::cout << "Enter the expiration date (YYYY-MM-DD): ";
            std::cin >> expirationDate;
            newIngredient = Ingredient(name, quantity, expirationDate);
            fridge.addIngredient(newIngredient);
        } else if (storageLocation == "P" || storageLocation == "p") {
            pantry.addIngredient(newIngredient);
        } else {
            std::cout << "Invalid option. Please choose (F)ridge or (P)antry.\n";
            continue;
        }

        saveIngredientsToFile("../data/storage.json");
    }
}

void RecipeManager::matchRecipes() {
    int option;
    std::cout << "Do you want to generate (1) random recipes based on all ingredients, or (2) select ingredients manually? ";
    std::cin >> option;

    std::vector<Ingredient> selectedIngredients;
    if (option == 1) {
        selectedIngredients = fridge.getIngredients();
        selectedIngredients.insert(selectedIngredients.end(), pantry.getIngredients().begin(), pantry.getIngredients().end());
    } else if (option == 2) {
        std::vector<Ingredient> allIngredients = fridge.getIngredients();
        allIngredients.insert(allIngredients.end(), pantry.getIngredients().begin(), pantry.getIngredients().end());

        std::cout << "Available ingredients in Fridge and Pantry:\n";
        for (size_t i = 0; i < allIngredients.size(); ++i) {
            std::cout << i + 1 << ". " << allIngredients[i].getName() << " (Quantity: " << allIngredients[i].getQuantity() << ")\n";
        }

        int num;
        std::cout << "How many ingredients do you want to select? ";
        std::cin >> num;

        for (int i = 0; i < num; ++i) {
            int ingredientIndex;
            std::cout << "Enter the number of the ingredient (1 to " << allIngredients.size() << "): ";
            std::cin >> ingredientIndex;

            if (ingredientIndex >= 1 && ingredientIndex <= allIngredients.size()) {
                selectedIngredients.push_back(allIngredients[ingredientIndex - 1]);
            } else {
                std::cout << "Invalid selection.\n";
            }
        }
    } else {
        std::cout << "Invalid option.\n";
        return;
    }

    std::string recipeType;
    std::cout << "Do you want a (S)weet or (Sa)vory recipe? ";
    std::cin >> recipeType;
    std::transform(recipeType.begin(), recipeType.end(), recipeType.begin(), ::tolower);

    std::vector<std::string> possibleRecipes;
    std::vector<const Recipe*> matchingRecipes;

    for (const auto& recipe : recipes) {
        std::string category = recipe.getType();
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);

        if ((recipeType == "s" && category == "sweet") || (recipeType == "sa" && category == "savory")) {
            std::vector<std::string> missingIngredients;
            if (recipe.canMakeRecipe(selectedIngredients, missingIngredients)) {
                possibleRecipes.push_back(recipe.getRecipeName());
                matchingRecipes.push_back(&recipe);
            } else if (!missingIngredients.empty()) {
                std::cout << "You are missing the following ingredients for " << recipe.getRecipeName() << ": ";
                for (const auto& ingredient : missingIngredients) {
                    std::cout << ingredient << " ";
                }
                std::cout << "\n";
            }
        }
    }

    if (possibleRecipes.empty()) {
        std::cout << "Sorry, you cannot make any recipes with the ingredients you have.\n";
    } else {
        std::cout << "Based on your ingredients, you can make the following recipes:\n";
        for (size_t i = 0; i < possibleRecipes.size(); ++i) {
            std::cout << i + 1 << ". " << possibleRecipes[i] << "\n";
        }

        int choice;
        std::cout << "Enter the number of the recipe you want to see in full: ";
        std::cin >> choice;

        if (choice > 0 && choice <= matchingRecipes.size()) {
            displayFullRecipe(*matchingRecipes[choice - 1]);
            saveHistory(*matchingRecipes[choice - 1]);
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}


void RecipeManager::viewRecipeHistory() {
    std::ifstream file("../data/history.json");

    if (!file.is_open()) {
        std::cerr << "History file does not exist. Initializing history.\n";
        std::ofstream outfile("../data/history.json");
        outfile << "[]";  // Empty array
        outfile.close();
        return;
    }

    json history;
    try {
        file >> history;
    } catch (json::parse_error& e) {
        std::cerr << "Error parsing history file: " << e.what() << "\n";
        std::cerr << "Resetting history to an empty array.\n";
        std::ofstream outfile("../data/history.json");
        outfile << "[]";
        outfile.close();
        return;
    }

    if (!history.is_array()) {
        std::cerr << "History format is incorrect, resetting history to an empty array.\n";
        std::ofstream outfile("../data/history.json");
        outfile << "[]";
        outfile.close();
        return;
    }

    if (history.empty()) {
        std::cout << "No recipe history found.\n";
        return;
    }

    std::cout << "Recipe History:\n";
    for (size_t i = 0; i < history.size(); ++i) {
        if (history[i].contains("name") && history[i]["name"].is_string()) {
            std::cout << i + 1 << ". " << history[i]["name"] << "\n";
        } else {
            std::cout << i + 1 << ". Invalid entry in history.\n";
        }
    }

    int choice;
    std::cout << "Enter the number of the recipe to view details or 0 to go back to the main menu: ";
    std::cin >> choice;

    if (choice > 0 && choice <= history.size()) {
        std::string selectedRecipeName = history[choice - 1]["name"];
        for (const auto& recipe : recipes) {
            if (recipe.getRecipeName() == selectedRecipeName) {
                displayFullRecipe(recipe);
                break;
            }
        }
    } else if (choice == 0) {
        std::cout << "Returning to the main menu.\n";
    } else {
        std::cout << "Invalid choice. Returning to the main menu.\n";
    }

    file.close();
}

void RecipeManager::menu() {
    int option;
    do {
        std::cout << "What would you like to do?\n";
        std::cout << "1. Add ingredients\n2. Generate recipes\n3. View recipe history\n4. Check notifications (expiring soon and running low)\n5. Exit\n";
        std::cin >> option;

        switch (option) {
            case 1:
                collectIngredients();
                break;
            case 2:
                matchRecipes();
                break;
            case 3:
                viewRecipeHistory();
                break;
            case 4:
                fridge.expiringSoon();
                pantry.runningLow();
                break;
            case 5:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (option != 5);
}
