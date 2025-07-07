// Transaction.cpp
#include "Transaction.h"
#include <iostream>

Transaction::Transaction(int i, const std::string& d, const std::string& cat, const std::string& desc, double amt, bool income)
    : id(i), date(d), category(cat), description(desc), amount(amt), isIncome(income) {}


Transaction::Transaction(const std::string& d, const std::string& cat, const std::string& desc, double amt, bool income)
    : id(-1), date(d), category(cat), description(desc), amount(amt), isIncome(income) {}

std::string Transaction::getDate() const { return date; }
std::string Transaction::getCategory() const { return category; }
std::string Transaction::getDescription() const { return description; }
double Transaction::getAmount() const { return amount; }
bool Transaction::getIsIncome() const { return isIncome; }

void Transaction::setDate(const std::string& d) { date = d; }
void Transaction::setCategory(const std::string& cat) { category = cat; }
void Transaction::setDescription(const std::string& desc) { description = desc; }
void Transaction::setAmount(double amt) { amount = amt; }
void Transaction::setIsIncome(bool income) { isIncome = income; }

void Transaction::display() const {
    std::string color = isIncome ? "\033[32m" : "\033[31m";
    std::string reset = "\033[0m";
    std::cout << color << date << " | " << category << " | " << description << " | " << amount << (isIncome ? " [Income]" : " [Expense]") << reset << '\n';
}

std::string Transaction::toCSV() const {
    return date + "," + category + "," + description + "," + std::to_string(amount) + "," + (isIncome ? "1" : "0");
}

int Transaction::getId() const {
    return id;
}


// Tracker.cpp addition
#include <regex>
#include "Tracker.h"
#include <fstream>

