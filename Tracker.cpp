#include "Tracker.h"
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>

Tracker::Tracker(Database& database) : db(database) {
    db.createTable();
}

void Tracker::addTransaction() {
    std::string date, category, desc;
    double amount;
    int type;

    std::cin.ignore();
    std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
    do {
        std::cout << "Date (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        if (!std::regex_match(date, dateRegex)) {
            std::cout << "Invalid format. Try again.\n";
        }
    } while (!std::regex_match(date, dateRegex));

    std::cout << "Category: ";
    std::getline(std::cin, category);
    std::cout << "Description: ";
    std::getline(std::cin, desc);

    std::cout << "Amount: ";
    while (!(std::cin >> amount)) {
        std::cout << "Invalid amount. Try again: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cout << "Type (1=income, 0=expense): ";
    while (!(std::cin >> type) || (type != 0 && type != 1)) {
        std::cout << "Invalid type. Enter 1 or 0: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    Transaction t(date, category, desc, amount, type == 1);
    db.insertTransaction(t);
    std::cout << "Transaction added.\n";
}

void Tracker::showAllTransactions() const {
    auto transactions = db.getAllTransactions();
    for (const auto& t : transactions)
        t.display();
}

void Tracker::showSummaryByCategory() const {
    auto transactions = db.getAllTransactions();
    std::map<std::string, double> income, expense;

    for (const auto& t : transactions) {
        if (t.getIsIncome())
            income[t.getCategory()] += t.getAmount();
        else
            expense[t.getCategory()] += t.getAmount();
    }

    std::cout << "\n=== Summary by Category ===\n";
    for (const auto& [cat, amt] : income)
        std::cout << "[Income] " << cat << ": $" << amt << '\n';
    for (const auto& [cat, amt] : expense)
        std::cout << "[Expense] " << cat << ": $" << amt << '\n';
}

void Tracker::showSummaryByMonth() const {
    auto transactions = db.getAllTransactions();
    std::map<std::string, double> income, expense;

    for (const auto& t : transactions) {
        std::string month = t.getDate().substr(0, 7); // "YYYY-MM"
        if (t.getIsIncome())
            income[month] += t.getAmount();
        else
            expense[month] += t.getAmount();
    }

    std::cout << "\n=== Monthly Summary ===\n";
    for (const auto& [month, amt] : income)
        std::cout << "[Income] " << month << ": $" << amt << '\n';
    for (const auto& [month, amt] : expense)
        std::cout << "[Expense] " << month << ": $" << amt << '\n';
}

void Tracker::deleteTransaction() {
    auto transactions = db.getAllTransactions();
    if (transactions.empty()) {
        std::cout << "No transactions to delete.\n";
        return;
    }

    // Display transactions with 1-based index
    for (size_t i = 0; i < transactions.size(); ++i) {
        std::cout << i + 1 << ". ";
        transactions[i].display();
    }

    std::cout << "Enter index to delete (starting from 1): ";
    int index;
    std::cin >> index;

    // Handle non-numeric input
    if (std::cin.fail()) {
        std::cin.clear(); // reset fail state
        std::cin.ignore(10000, '\n'); // discard bad input
        std::cout << "Invalid input. Please enter a number.\n";
        return;
    }

    // Check range (1 to transactions.size)
    if (index < 1 || index > static_cast<int>(transactions.size())) {
        std::cout << "Invalid index.\n";
        return;
    }

    // Delete by ID
    int id = transactions[index - 1].getId();  // Requires getId() method in Transaction
    db.deleteTransaction(id);

    std::cout << "Transaction deleted from database.\n";
}




void Tracker::editTransaction() {
    auto transactions = db.getAllTransactions();
    if (transactions.empty()) {
        std::cout << "No transactions to edit.\n";
        return;
    }

    for (size_t i = 0; i < transactions.size(); ++i) {
        std::cout << i + 1 << ". ";
        transactions[i].display();
    }

    std::cout << "Enter index to edit (starting from 1): ";
    int index;
    std::cin >> index;
    std::cin.ignore();

    if (index < 1 || index > transactions.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    std::string date, category, desc;
    double amount;
    int type;
    std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");

    do {
        std::cout << "New Date (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        if (!std::regex_match(date, dateRegex)) {
            std::cout << "Invalid format. Try again.\n";
        }
    } while (!std::regex_match(date, dateRegex));

    std::cout << "New Category: ";
    std::getline(std::cin, category);
    std::cout << "New Description: ";
    std::getline(std::cin, desc);

    std::cout << "New Amount: ";
    while (!(std::cin >> amount)) {
        std::cout << "Invalid amount. Try again: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cout << "New Type (1=income, 0=expense): ";
    while (!(std::cin >> type) || (type != 0 && type != 1)) {
        std::cout << "Invalid type. Enter 1 or 0: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    Transaction updated(date, category, desc, amount, type == 1);
    db.editTransaction(index, updated);  // ✅ actually saves changes
    std::cout << "Transaction updated successfully.\n";
}


void Tracker::saveToFile(const std::string& filename) const {
    auto transactions = db.getAllTransactions();
    std::ofstream file(filename);
    for (const auto& t : transactions)
        file << t.toCSV() << '\n';
}

void Tracker::exportFilteredToFile() {
    std::string filterType;
    std::cout << "Filter by 'month' or 'category'? ";
    std::cin >> filterType;

    std::cin.ignore();
    std::vector<Transaction> filtered;
    if (filterType == "month") {
        std::string month;
        std::cout << "Enter month (YYYY-MM): ";
        std::getline(std::cin, month);

        for (const auto& t : db.getAllTransactions()) {
            if (t.getDate().substr(0, 7) == month) {
                filtered.push_back(t);
            }
        }
    } else if (filterType == "category") {
        std::string category;
        std::cout << "Enter category: ";
        std::getline(std::cin, category);

        for (const auto& t : db.getAllTransactions()) {
            if (t.getCategory() == category) {
                filtered.push_back(t);
            }
        }
    } else {
        std::cout << "Invalid filter type.\n";
        return;
    }

    std::string filename;
    std::cout << "Enter filename to export (e.g., filtered.csv): ";
    std::getline(std::cin, filename);

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    for (const auto& t : filtered) {
        file << t.toCSV() << '\n';
    }

    std::cout << "Exported " << filtered.size() << " transactions to " << filename << '\n';
}

void Tracker::showBarChart() {
    auto transactions = db.getAllTransactions();
    std::map<std::string, double> income, expense;
    double maxVal = 0.0;

    for (const auto& t : transactions) {
        std::string month = t.getDate().substr(0, 7);
        if (t.getIsIncome()) {
            income[month] += t.getAmount();
            maxVal = std::max(maxVal, income[month]);
        } else {
            expense[month] += t.getAmount();
            maxVal = std::max(maxVal, expense[month]);
        }
    }

    if (transactions.empty()) {
        std::cout << "No transactions to display.\n";
        return;
    }

    const int maxBarWidth = 50;

    std::set<std::string> allMonths;
    for (const auto& [m, _] : income) allMonths.insert(m);
    for (const auto& [m, _] : expense) allMonths.insert(m);

    std::cout << "\n=== Monthly Income & Expense Bar Chart ===\n";
    std::cout << "Month\n";

    for (const auto& month : allMonths) {
        double inAmt = income[month];
        double exAmt = expense[month];

        int inBars = static_cast<int>((inAmt / maxVal) * maxBarWidth);
        int exBars = static_cast<int>((exAmt / maxVal) * maxBarWidth);

        std::string inBar = std::string(inBars, '#');
        std::string exBar = std::string(exBars, '*');

        std::cout << month << "\n";
        std::cout << "  Income  : " << inBar << " $" << inAmt << "\n";
        std::cout << "  Expense : " << exBar << " $" << exAmt << "\n";
    }
}


// TODO: implement deleteTransaction() and editTransaction() using SQL later
