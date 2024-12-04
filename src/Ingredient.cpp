#include <string>
#include <stdexcept>
#include <iostream>
#include "../include/json.hpp"
using json = nlohmann::json;  
#include "../include/Ingredient.h"

    Ingredient::Ingredient() {}
    Ingredient::Ingredient(std::string n, int q, std::string exp = "") : name(n), quantity(q), expirationDate(exp) {}

    std::string Ingredient::getName() const { return name; }
    int Ingredient::getQuantity() const { return quantity; }
    std::string Ingredient::getExpirationDate() const { return expirationDate; }

    void Ingredient::setQuantity(int q) { quantity = q; }

    void Ingredient::setExpirationDate(const std::string& expDate) {expirationDate = expDate;}

    json Ingredient::toJSON() const {
        return { {"name", name}, {"quantity", quantity}, {"expirationDate", expirationDate} };
    }

    Ingredient Ingredient::fromJSON(const json& j) {
    try {
        if (!j.contains("name") || !j.contains("quantity")) {
            throw std::runtime_error("JSON missing required fields: name or quantity");
        }
        return Ingredient(j["name"], j["quantity"], j.value("expirationDate", ""));
    } catch (const std::exception& e) {
        std::cerr << "Error creating ingredient from JSON: " << e.what() << std::endl;
        return Ingredient(); // Return a default-constructed Ingredient in case of error
    }
}


