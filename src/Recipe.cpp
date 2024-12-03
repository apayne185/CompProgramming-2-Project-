#include "Recipe.h"
#include "json/json.hpp"
#include <fstream>
#include <iostream>
using json = nlohmann::json;
using std::string;
using std::vector;
using std::pair;

Recipe::Recipe(std::string name, 
           std::vector<std::pair<std::string, std::string>> ingredients,
           std::vector<std::pair<std::string, std::string>> condiments, 
           std::vector<std::string> steps, std::string type)
    : recipeName(name), requiredIngredients(ingredients), condiments(condiments), steps(steps), category(type) {}

std::string Recipe::getRecipeName() const {
    return recipeName;
}

std::string Recipe::getType() const {
    return category;
}


bool Recipe::canMakeRecipe(const std::vector<Ingredient>& userIngredients, std::vector<std::string>& missingIngredients) const {
    bool hasAllMainIngredients = true;

    for (const auto& reqIngredient : requiredIngredients) {
        bool found = false;
        for (const auto& userIngredient : userIngredients) {
            std::string recipeIngredientName = reqIngredient.first;
            std::string userIngredientName = userIngredient.getName();

            std::transform(recipeIngredientName.begin(), recipeIngredientName.end(), recipeIngredientName.begin(), ::tolower);
            std::transform(userIngredientName.begin(), userIngredientName.end(), userIngredientName.begin(), ::tolower);

            if (userIngredientName == recipeIngredientName) {
                found = true;
                break;
            }
        }
        if (!found) {
            missingIngredients.push_back(reqIngredient.first);
            hasAllMainIngredients = false;
        }
    }

    return hasAllMainIngredients;
}

std::vector<std::pair<std::string, std::string>> Recipe::getRequiredIngredients() const {
    return requiredIngredients;
}

std::vector<std::pair<std::string, std::string>> Recipe::getCondiments() const {
    return condiments;
}

std::vector<std::string> Recipe::getSteps() const {
    return steps;
}


std::vector<Recipe> loadRecipesFromJSON(const std::string& filename) {
    std::vector<Recipe> recipes;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return recipes;
    }

    try {
        json j;
        inputFile >> j;

        for (const auto& recipeData : j["recipes"]) {
            std::string recipeName = recipeData.value("name", "Unnamed Recipe");
            std::string category = recipeData.value("category", "Uncategorized");
            std::vector<std::pair<std::string, std::string>> ingredients;
            if(recipeData.contains("ingredients")) {
                for (const auto& ingredient : recipeData["ingredients"]) {
                    std::string name = ingredient.value("name", "Unnamed Ingredient");
                    std::string quantity = ingredient.value("quantity", "0");
                    std::string unit = ingredient.value("unit", "");
                    ingredients.push_back({ name, quantity + " " + (unit.empty() ? "" : " " + unit)});
                }
            }

            std::vector<std::pair<std::string, std::string>> condiments;
            if(recipeData.contains("condiments")) {
                for (const auto& condiment : recipeData["condiments"]) {
                    std::string name = condiment.value("name", "Unknown Condiment");
                    std::string quantity = condiment.value("quantity", "0");
                    std::string unit = condiment.value("unit", "");
                    condiments.push_back({ name, quantity + " " + (unit.empty() ? "" : " " + unit)});
               }
            }
            std::vector<std::string> steps;
            if(recipeData.contains("steps")) {
                for (const auto& step : recipeData["steps"]) {
                    steps.push_back(step.get<std::string>());
                }
            }
            recipes.emplace_back(recipeName, ingredients, condiments, steps, category);
        }
    } catch (const std::exception& e) {
      std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
    }

    return recipes;
}
