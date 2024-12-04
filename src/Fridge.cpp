#include "../include/Fridge.h"

void Fridge::addIngredient(const Ingredient& ingredient) {
    Storage::addIngredient(ingredient);
    std::cout << ingredient.getName() << " added to Fridge.\n";
}

void Fridge::expiringSoon() const {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    json groceryList;

    std::ifstream file("../data/grocery_list.json");
    if (file.is_open()) {
        file >> groceryList;
        file.close();
    }

    for (const auto& ingredient : ingredients) {
        if (!ingredient.getExpirationDate().empty()) {
            tm exp = {};
            strptime(ingredient.getExpirationDate().c_str(), "%Y-%m-%d", &exp);
            time_t exp_time = mktime(&exp);

            if (difftime(exp_time, now) <= 5 * 24 * 60 * 60) {
                std::cout << ingredient.getName() << " is expiring in less than 5 days.\n";

                char addToList;
                std::cout << "Would you like to add " << ingredient.getName() << " to the grocery list? (y/n): ";
                std::cin >> addToList;

                if (addToList == 'y' || addToList == 'Y') {
                    bool alreadyInList = false;
                    for (const auto& item : groceryList) {
                        if (item["name"] == ingredient.getName()) {
                            alreadyInList = true;
                            break;
                        }
                    }

                    if (!alreadyInList) {
                        groceryList.push_back({{"name", ingredient.getName()}});
                    }
                }
            }
        }
    }

    // Save updated grocery list
    std::ofstream outFile("../data/grocery_list.json");
    if (outFile.is_open()) {
        outFile << groceryList.dump(4);
        outFile.close();
        std::cout << "Updated grocery list saved.\n";
    }
}

