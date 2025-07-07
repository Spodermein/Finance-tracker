// Transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string date, category, description;
    double amount;
    bool isIncome;
    int id;


public:
    Transaction(int id, const std::string& d, const std::string& cat, const std::string& desc, double amt, bool income);

    Transaction(const std::string& d, const std::string& cat, const std::string& desc, double amt, bool income);

    std::string getDate() const;
    std::string getCategory() const;
    std::string getDescription() const;
    double getAmount() const;
    bool getIsIncome() const;
    void display() const;
    std::string toCSV() const;
    void setDate(const std::string& d);
    void setCategory(const std::string& cat);
    void setDescription(const std::string& desc);
    void setAmount(double amt);
    void setIsIncome(bool income);
    int getId() const;

};

#endif