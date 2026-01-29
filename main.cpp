#include <iostream>
#include <string>
#include <fstream>
#include <limits> 

using namespace std;


const string DB_FILENAME = "users.txt";

struct User {
    int id;
    string username;
    string password;
};

string encrypt(string text) {
    for (size_t i = 0; i < text.length(); i++) {
        text[i] = text[i] + 3;
    }
    return text;
}


bool isDuplicate(const string& name) {
    
    ifstream file(DB_FILENAME.c_str());
    
    if (!file.is_open()) return false;

    string line;
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


int getNextID() {
    ifstream file(DB_FILENAME.c_str());
    
    string line;
    int count = 1000;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (!line.empty()) count++;
        }
    }
    return count + 1;
}

void registerUser() {
    User u;
    cout << "\n--- Registration ---" << endl;
    cout << "Enter username (min 5 chars): ";
    cin >> u.username;

    if (u.username.length() < 5) {
        cout << "Error: Username too short!" << endl;
        return;
    }

    if (isDuplicate(u.username)) {
        cout << "Error: Username already exists!" << endl;
        return;
    }

    cout << "Enter password: ";
    cin >> u.password;

    u.id = getNextID();
    string encryptedPass = encrypt(u.password);

    
    ofstream outFile(DB_FILENAME.c_str(), ios::app);
    
    if (outFile.is_open()) {
        outFile << u.id << "," << u.username << "," << encryptedPass << "\n";
        outFile.close();
        cout << "Registration Successful! Your ID is: " << u.id << endl;
    } else {
        cout << "Error: Unable to open database file." << endl;
    }
}

void searchUser() {
    string searchID;
    cout << "\n--- Search User ---" << endl;
    cout << "Enter User ID to search: ";
    cin >> searchID;

    ifstream file(DB_FILENAME.c_str());
    
    if (!file.is_open()) {
        cout << "Database is empty or missing." << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t firstComma = line.find(',');

        if (firstComma != string::npos) {
            string currentID = line.substr(0, firstComma);

            if (currentID == searchID) {
                cout << "Record Found: " << line << endl;
                found = true;
                break;
            }
        }
    }

    if (!found) cout << "User not found." << endl;
}

int main() {
    string choice; 
    
    cout << "Welcome to Oriental Softwares' Employee Portal" << endl;

    while (true) {
        cout << "\n1. Register\n2. Search User (Admin)\n3. Exit\nChoice: ";
        cin >> choice;

        if (choice == "1") {
            registerUser();
        } else if (choice == "2") {
            searchUser();
        } else if (choice == "3") {
            cout << "Exiting system..." << endl;
            break;
        } else {
            cout << "Invalid option. Please enter 1, 2, or 3." << endl;
        }
    }
    return 0;
}
