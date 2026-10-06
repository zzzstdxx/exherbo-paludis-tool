#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <limits>
#include <string>
#include <memory>
#include <array>

const std::string RED     = "\033[1;31m";
const std::string GREEN   = "\033[1;32m";
const std::string YELLOW  = "\033[1;33m";
const std::string MAGENTA = "\033[1;35m";
const std::string CYAN    = "\033[1;36m";
const std::string RESET   = "\033[0m";

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getCommandOutput(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
    if (getuid() != 0) {
        std::cerr << RED << "Must run as root. Aborting..." << RESET << "\n";
        return 1;
    }

    while (true) {
        int choice = 0;
        int trigger = 0;
        std::string query;

        std::cout << "\n" << MAGENTA << "--- Paludis Management Toolkit ---" << RESET << "\n";
        std::cout << CYAN << "(1) Upgrade worldset\n(2) Sync repositories\n(3) Purge unneeded\n(4) Search packages\n(5) Show package details\n(6) Read unread repository news\n(0) Exit" << RESET << "\n";
        std::cout << "Select an option: ";
        
        if (!(std::cin >> choice)) {
            std::cout << RED << "Invalid option format!" << RESET << "\n";
            clearInput();
            continue;
        }

        if (choice == 0) {
            std::cout << GREEN << "Exiting toolkit. Goodbye!" << RESET << "\n";
            break;
        }

        if (choice == 1) {
            std::system("cave resolve world");
            std::cout << YELLOW << "Run the trigger? (1/0) " << RESET;
            if (std::cin >> trigger && trigger == 1) {
                std::system("cave resolve world -x");
                std::cout << GREEN << "Done" << RESET << "\n";
            } else {
                std::cout << RED << "Aborting..." << RESET << "\n";
            }
            clearInput();
        } 
        else if (choice == 2) {
            std::system("cave sync");
            std::cout << GREEN << "Done" << RESET << "\n";
        } 
        else if (choice == 3) {
            std::system("cave purge");
            std::cout << YELLOW << "Run the trigger? (1/0) " << RESET;
            if (std::cin >> trigger && trigger == 1) {
                std::system("cave purge -x");
                std::cout << GREEN << "Done" << RESET << "\n";
            } else {
                std::cout << RED << "Aborting..." << RESET << "\n";
            }
            clearInput();
        } 
        else if (choice == 4) {
            std::cout << "Enter package search query: ";
            std::cin >> query;
            std::string cmd = "cave search " + query;
            std::system(cmd.c_str());
        } 
        else if (choice == 5) {
            std::cout << "Enter exact package name: ";
            std::cin >> query;
            std::string cmd = "cave show " + query;
            std::system(cmd.c_str());
        }
        else if (choice == 6) {
            std::string output = getCommandOutput("eclectic news read new");
            if (output.empty() || output == "\n") {
                std::cout << YELLOW << "(nothing new)" << RESET << "\n";
            } else {
                std::cout << output;
            }
        }
        else {
            std::cout << RED << "Invalid choice!" << RESET << "\n";
        }
    }

    return 0;
}

