#include <string>
#include "../include/json.hpp"
using json = nlohmann::json;  
#include "../include/Ingredient.h"

    Ingredient::Ingredient() {}
    Ingredient::Ingredient(std::string n, int q, std::string exp = "") : name(n), quantity(q), expirationDate(exp) {
      if(n.empty()){
        std::cerr << "Empty Ingredient Name" << std::endl;
      }
      if(n.q <= 0){
        std::cerr << "Empty Ingredient Quantity or Must be Higher than 0" << std::endl;
      }
    }

    std::string Ingredient::getName() const { return name; }
    int Ingredient::getQuantity() const { return quantity; }
    std::string Ingredient::getExpirationDate() const { return expirationDate; }

    void Ingredient::setQuantity(int q) {
      if(q<= 0){
        throw std::invalid_argument("Empty Ingredient Quantity");
      }
      quantity = q;
    }

    void Ingredient::setExpirationDate(const std::string& expDate) {expirationDate = expDate;}

    json Ingredient::toJSON() const {
        return { {"name", name}, {"quantity", quantity}, {"expirationDate", expirationDate} };
    }

    Ingredient Ingredient::fromJSON(const json& j) {
        if (!j.contains(" name") || !j["name"].is_string()) {
          throw std::invalid_argument("Missing Ingredient Name or Name is not a string");
        }
        if (!j.contains("quantity") || !j["quantity"].is_number_integer()) {
          throw std::invalid_argument("Missing Ingredient Quantity or Quantity is not a number");
        }
        return Ingredient(j["name"], j["quantity"], j.value("expirationDate", ""));
    }


