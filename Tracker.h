#pragma once
#include <vector>
#include "Transaction.h"
#include "Database.h" 

class Tracker {
private:
    Database& db;


public:
    Tracker(Database& database);
    void addTransaction();
    void showAllTransactions() const;
    void showSummaryByCategory() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void showSummaryByMonth() const;
    void deleteTransaction();
    void editTransaction();
    void exportFilteredToFile();
    void showBarChart();



};
