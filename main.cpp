#include <iostream>
#include <string>
#include <fstream>
#include <limits> // For numeric_limits
#include <cstdlib> // For system()

using namespace std;

// ==========================================
// DATA STRUCTURES
// ==========================================
struct User {
    int id;
    string username;
    string password;
};

// ==========================================
// BACKEND: LOGIC & FILE MANAGEMENT
// ==========================================
class UserManager {
private:
    const string FILENAME = "users.txt";

    // Requirement: Caesar Cipher Encryption (Shift by 3)
    string encrypt(string text) {
        for (size_t i = 0; i < text.length(); i++) {
            text[i] = text[i] + 3;
        }
        return text;
    }

    // Helper: Generate next sequential ID
    int getNextID() {
        ifstream file(FILENAME);
        string line;
        int count = 1000; // Starting ID base
        if (file.is_open()) {
            while (getline(file, line)) {
                if (!line.empty()) count++;
            }
            file.close();
        }
        return count + 1;
    }

public:
    // Requirement: Duplicate Prevention
    bool isDuplicate(const string& name) {
        ifstream file(FILENAME);
        string line;
        if (!file.is_open()) return false;

        while (getline(file, line)) {
            size_t firstComma = line.find(',');
            size_t secondComma = line.find(',', firstComma + 1);

            if (firstComma != string::npos && secondComma != string::npos) {
                string existingName = line.substr(firstComma + 1, secondComma - firstComma - 1);
                if (existingName == name) return true;
            }
        }
        return false;
    }

    // Requirement: Register User
    // Returns the new ID if successful, or -1 if duplicate
    int registerUser(string username, string password) {
        if (isDuplicate(username)) {
            return -1;
        }

        User u;
        u.id = getNextID();
        u.username = username;
        u.password = encrypt(password);

        ofstream outFile(FILENAME, ios::app);
        outFile << u.id << "," << u.username << "," << u.password << "\n";
        outFile.close();

        return u.id;
    }

    // Requirement: Search User
    // Returns the full record string or empty string if not found
    string searchUserByID(const string& searchID) {
        ifstream file(FILENAME);
        string line;
        
        if (!file.is_open()) return "";

        while (getline(file, line)) {
            // Precise matching: Check if line starts with "ID,"
            // This prevents ID "1" from matching "10" or "100"
            string idDelimiter = searchID + ",";
            if (line.find(idDelimiter) == 0) {
                return line;
            }
        }
        return "";
    }
};

// ==========================================
// FRONTEND: USER INTERFACE (UI)
// ==========================================
class ConsoleUI {
public:
    // ANSI Color Codes for Modern Terminal Look
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string CYAN = "\033[36m";
    const string BOLD = "\033[1m";

    void clearScreen() {
        // Works on Windows and Linux/Mac
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void printHeader(string title) {
        clearScreen();
        cout << CYAN << "==========================================" << RESET << endl;
        cout << "      " << BOLD << title << RESET << endl;
        cout << CYAN << "==========================================" << RESET << endl;
        cout << endl;
    }

    void showSuccess(string message) {
        cout << GREEN << "[SUCCESS] " << message << RESET << endl;
        pause();
    }

    void showError(string message) {
        cout << RED << "[ERROR] " << message << RESET << endl;
        pause();
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    // The Main Menu UI
    string showMainMenu() {
        clearScreen();
        cout << CYAN << "******************************************" << RESET << endl;
        cout << BOLD << "   ORIENTAL SOFTWARES - EMPLOYEE PORTAL" << RESET << endl;
        cout << CYAN << "******************************************" << RESET << endl;
        cout << "\n";
        cout << " [1] " << YELLOW << "Register New User" << RESET << endl;
        cout << " [2] " << YELLOW << "Search User Database" << RESET << endl;
        cout << " [3] " << RED << "Exit System" << RESET << endl;
        cout << "\n";
        cout << "Select an option: ";
        
        string choice;
        cin >> choice;
        return choice;
    }

    void handleRegistration(UserManager& mgr) {
        printHeader("USER REGISTRATION");
        
        string username, password;
        
        cout << "Enter username (min 5 chars): ";
        cin >> username;

        if (username.length() < 5) {
            showError("Username must be at least 5 characters.");
            return;
        }

        cout << "Enter password: ";
        cin >> password;

        int resultID = mgr.registerUser(username, password);

        if (resultID == -1) {
            showError("Username already exists in the database.");
        } else {
            showSuccess("User registered! Generated ID: " + to_string(resultID));
        }
    }

    void handleSearch(UserManager& mgr) {
        printHeader("SEARCH DATABASE");
        
        string id;
        cout << "Enter User ID to search: ";
        cin >> id;

        cout << "\nSearching..." << endl;
        string result = mgr.searchUserByID(id);

        if (!result.empty()) {
            cout << GREEN << "\nResult Found:\n" << RESET;
            cout << "------------------------------------------" << endl;
            cout << result << endl;
            cout << "------------------------------------------" << endl;
            pause();
        } else {
            showError("No user found with ID: " + id);
        }
    }
};

// ==========================================
// MAIN APPLICATION ENTRY
// ==========================================
int main() {
    UserManager userManager;
    ConsoleUI ui;
    bool running = true;

    while (running) {
        string choice = ui.showMainMenu();

        if (choice == "1") {
            ui.handleRegistration(userManager);
        } else if (choice == "2") {
            ui.handleSearch(userManager);
        } else if (choice == "3") {
            cout << "\nShutting down system..." << endl;
            running = false;
        } else {
            ui.showError("Invalid selection. Please try again.");
        }
    }

    return 0;
}