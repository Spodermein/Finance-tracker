#pragma once
#include "Transaction.h"
#include <vector>
#include <string>
#include <sqlite3.h>

class Database {
private:
    sqlite3* db;
public:
    Database(const std::string& filename);
    ~Database();

    void createTable();
    void insertTransaction(const Transaction& t);
    std::vector<Transaction> getAllTransactions();
    void editTransaction(int index, const Transaction& t);
    void deleteTransaction(int index);
};
