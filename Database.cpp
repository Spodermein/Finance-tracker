#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB\n";
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::createTable() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS transactions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "date TEXT, "
        "category TEXT, "
        "description TEXT, "
        "amount REAL, "
        "isIncome INTEGER);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errMsg << '\n';
        sqlite3_free(errMsg);
    }
}

void Database::insertTransaction(const Transaction& t) {
    std::string sql = "INSERT INTO transactions (date, category, description, amount, isIncome) VALUES (?,?,?,?,?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, t.getDate().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, t.getCategory().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, t.getDescription().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 4, t.getAmount());
        sqlite3_bind_int(stmt, 5, t.getIsIncome());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Insert failed\n";
        }
        sqlite3_finalize(stmt);
    }
}

std::vector<Transaction> Database::getAllTransactions() {
    std::vector<Transaction> results;
    const char* sql = "SELECT id, date, category, description, amount, isIncome FROM transactions;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string date = (const char*)sqlite3_column_text(stmt, 1);
            std::string category = (const char*)sqlite3_column_text(stmt, 2);
            std::string description = (const char*)sqlite3_column_text(stmt, 3);
            double amount = sqlite3_column_double(stmt, 4);
            bool isIncome = sqlite3_column_int(stmt, 5);
;

            results.emplace_back(id, date, category, description, amount, isIncome);

        }
        sqlite3_finalize(stmt);
    }

    return results;
}

void Database::editTransaction(int index, const Transaction& t) {
    auto transactions = getAllTransactions();
    if (index < 1 || index > transactions.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    // Find the original transaction's rowid (or you can use another unique ID)
    int rowid = index;

    std::string sql = "UPDATE transactions SET date=?, category=?, description=?, amount=?, isIncome=? WHERE rowid=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, t.getDate().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, t.getCategory().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, t.getDescription().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 4, t.getAmount());
        sqlite3_bind_int(stmt, 5, t.getIsIncome() ? 1 : 0);
        sqlite3_bind_int(stmt, 6, rowid);

        if (sqlite3_step(stmt) != SQLITE_DONE)
            std::cerr << "Failed to update transaction.\n";
    } else {
        std::cerr << "SQL error in updateTransaction.\n";
    }
    sqlite3_finalize(stmt);
}



void Database::deleteTransaction(int index) {
    auto transactions = getAllTransactions();
    if (index < 1 || index > transactions.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    int rowid = index;

    std::string sql = "DELETE FROM transactions WHERE rowid=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, rowid);

        if (sqlite3_step(stmt) != SQLITE_DONE)
            std::cerr << "Failed to delete transaction.\n";
    } else {
        std::cerr << "SQL error in deleteTransaction.\n";
    }
    sqlite3_finalize(stmt);
}


