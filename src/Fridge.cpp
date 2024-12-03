#include "../include/Fridge.h"
#include <iomanip>

void Fridge::addIngredient(const Ingredient& ingredient) {
    if (ingredient.getName().empty() || ingredient.getQuantity() <= 0) {
        throw std::invalid_argument("Ingredient name cannot be empty or ingredient quantity must be greater than 0.");
    }
    if(this->getCapcity() >= this->getMaxCapcity()) {
      throw std::overflow_error("Fridge is full, cannot add more ingredients.");
    }

    Storage::addIngredient(ingredient);
    std::cout << ingredient.getName() << " added to Fridge.\n";
}



void Fridge::expiringSoon() const {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    if (ingredients.empty()) {
        std::cout << "Fridge is empty. No ingredients to check for expiration.\n";
        return;
    }

    for (const auto& ingredient : ingredients) {
        if (!ingredient.getExpirationDate().empty()) {
            tm exp = {};
            if(strptime(ingredient.getExpirationDate().c_str(), "%Y-%m-%d", &exp) == nullpr){
              std::cerr<< "Invalid expiration date format for ingredient" << ingredient.getName() << "\n";
              continue;
            }
            time_t exp_time = mktime(&exp);

            if (difftime(exp_time, now) <= 5 * 24 * 60 * 60) {
                std::cout << ingredient.getName() << " is expiring in less than 5 days.\n";
            }
        } else {
            std::cerr << "Missing Expiration Date for " << ingredient.getName() << "\n";
        }
    }
}
