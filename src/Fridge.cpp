#include "../include/Fridge.h"

void Fridge::addIngredient(const Ingredient& ingredient) {
    Storage::addIngredient(ingredient);
    std::cout << ingredient.getName() << " added to Fridge.\n";
}

void Fridge::expiringSoon() const {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    for (const auto& ingredient : ingredients) {
        if (!ingredient.getExpirationDate().empty()) {
            tm exp = {};
            strptime(ingredient.getExpirationDate().c_str(), "%Y-%m-%d", &exp);
            time_t exp_time = mktime(&exp);

            if (difftime(exp_time, now) <= 5 * 24 * 60 * 60) {
                std::cout << ingredient.getName() << " is expiring in less than 5 days.\n";
            }
        }
    }
}
