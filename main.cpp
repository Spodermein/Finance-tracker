#include <iostream>
#include "Tracker.h"
#include <limits>
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    Database db("finance.db");
    Tracker tracker(db);

    int choice;
    do {
         std::cout << "\n========================================\n";
        std::cout << "        Personal Finance Tracker         \n";
        std::cout << "========================================\n";
        std::cout << " 1. Add Transaction\n";
        std::cout << " 2. View All Transactions\n";
        std::cout << " 3. View Summary by Category\n";
        std::cout << " 4. View Monthly Summary\n";
        std::cout << " 5. Delete a Transaction\n";
        std::cout << " 6. Edit a Transaction\n";
        std::cout << " 7. Export filtered transactions to CSV\n";
        std::cout << " 8. Show ASCII bar chart summary\n";
        std::cout << " 9. Save & Exit\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Choose: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[!] Invalid input. Please enter a number.\n";
            pause();
            continue;
        }

        switch (choice) {
            case 1: tracker.addTransaction(); break;
            case 2: tracker.showAllTransactions(); break;
            case 3: tracker.showSummaryByCategory(); break;
            case 4: tracker.showSummaryByMonth(); break;
            case 5: tracker.deleteTransaction(); break;
            case 6: tracker.editTransaction(); break;
            case 7: tracker.exportFilteredToFile(); break;
            case 8: tracker.showBarChart(); break;
            case 9:
                tracker.saveToFile("storage.txt");
                std::cout << "\n[✔] Data saved. Exiting...\n";
                break;
            default:
                std::cout << "\n[!] Invalid choice! Try again.\n";
                break;
        }

        if (choice != 9) pause();
    } while (choice != 9);

    return 0;
}
