#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayHeader(const string& title) {
    clearScreen();
    cout << "\n\t\t" << BLUE << "************************************************************" << RESET;
    cout << "\n\t\t" << BLUE << "*                                                          *" << RESET;
    cout << "\n\t\t" << BLUE << "*           " << setw(40) << left << title << "       *" << RESET;
    cout << "\n\t\t" << BLUE << "*                                                          *" << RESET;
    cout << "\n\t\t" << BLUE << "************************************************************" << RESET << "\n";
}

void displayAsciiArt() {
    cout << MAGENTA << R"(
   ____            _             _     __  __                                   
  / ___|___  _ __ | |_ __ _  ___| |_  |  \/  | __ _ _ __   __ _  __ _  ___ _ __ 
 | |   / _ \| '_ \| __/ _` |/ __| __| | |\/| |/ _` | '_ \ / _` |/ _` |/ _ \ '__|
 | |__| (_) | | | | || (_| | (__| |_  | |  | | (_| | | | | (_| | (_| |  __/ |   
  \____\___/|_| |_|\__\__,_|\___|\__| |_|  |_|\__,_|_| |_|\__,_|\__, |\___|_|   
                                                                |___/   
)" << RESET << '\n';  
}

class Contact {
private:
    string name;
    string phoneNumber;

public:
    Contact(const string& n, const string& p) : name(n), phoneNumber(p) {}
    string getName() const { return name; }
    string getPhoneNumber() const { return phoneNumber; }
};

class ContactManager {
private:
    vector<Contact> contacts;
    const string filename = "contacts.txt";

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void saveContacts() {
        ofstream file(filename);
        for (const auto& contact : contacts) {
            file << contact.getName() << "," << contact.getPhoneNumber() << "\n";
        }
        file.close();
    }

    void loadContacts() {
        ifstream file(filename);
        string line, name, number;
        while (getline(file, line)) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                name = line.substr(0, commaPos);
                number = line.substr(commaPos + 1);
                contacts.emplace_back(name, number);
            }
        }
        file.close();
    }

public:
    ContactManager() {
        loadContacts();
    }

    void addContact() {
        displayHeader("Add New Contact");
        string name, phoneNumber;
        cout << GREEN << "Enter name: " << RESET;  
        clearInputBuffer();
        getline(cin, name);
        cout << GREEN << "Enter phone number: " << RESET;  
        getline(cin, phoneNumber);
        contacts.emplace_back(name, phoneNumber);
        saveContacts();
        cout << GREEN << "Contact added successfully.\n" << RESET;  
        system("pause");
    }

    void viewContacts() const {
        displayHeader("Contact List");
        if (contacts.empty()) {
            cout << RED << "No contacts found. Your contact list is empty.\n" << RESET;  
            system("pause");
            return;
        }
        cout << setw(5) << "Index" << setw(40) << "Name" << setw(15) << "Phone Number" << "\n";
        cout << string(70, '-') << "\n";
        for (size_t i = 0; i < contacts.size(); ++i) {
            cout << setw(5) << i + 1 << setw(40) << contacts[i].getName()
                << setw(15) << contacts[i].getPhoneNumber() << "\n";
        }
        system("pause");
    }

    void deleteContact() {
        displayHeader("Delete Contact");
        viewContacts();
        if (contacts.empty()) return;

        int index;
        cout << YELLOW << "\nEnter the index number of the contact to delete (0 to cancel): " << RESET;  
        cin >> index;
        clearInputBuffer();

        if (index == 0) {
            cout << YELLOW << "Deletion cancelled.\n" << RESET;  
            system("pause");
            return;
        }
        if (index >= 1 && index <= static_cast<int>(contacts.size())) {
            cout << RED << "Are you sure you want to delete " << contacts[index - 1].getName() << "? (y/n): " << RESET;  
            char confirm;
            cin >> confirm;
            clearInputBuffer();
            if (confirm == 'y' || confirm == 'Y') {
                contacts.erase(contacts.begin() + index - 1);
                saveContacts();
                cout << RED << "Contact deleted successfully.\n" << RESET;  
            }
            else {
                cout << YELLOW << "Deletion cancelled.\n" << RESET;  
            }
        }
        else {
            cout << RED << "Invalid contact number.\n" << RESET;  
        }
        system("pause");
    }

    void searchContact() {
        displayHeader("Search Contact");
        string searchTerm;
        cout << GREEN << "Enter name or phone number to search: " << RESET;  
        clearInputBuffer();
        getline(cin, searchTerm);

        bool found = false;
        cout << "\n" << CYAN << "--- Search Results ---" << RESET << "\n";  
        for (const auto& contact : contacts) {
            if (contact.getName().find(searchTerm) != string::npos ||
                contact.getPhoneNumber().find(searchTerm) != string::npos) {
                cout << contact.getName() << " - " << contact.getPhoneNumber() << "\n";
                found = true;
                break;
            }
        }
        if (!found) {
            cout << RED << "No matching contacts found.\n" << RESET;  
        }
        system("pause");
    }
};

class Admin {
private:
    string username;
    string password;
    const string credentialsFile = "admin_credentials.txt";

    void saveCredentials() {
        ofstream file(credentialsFile);
        file << username << "\n" << password;
        file.close();
    }

    bool loadCredentials() {
        ifstream file(credentialsFile);
        if (file.is_open()) {
            getline(file, username);
            getline(file, password);
            file.close();
            return true;
        }
        return false;
    }

    void setInitialCredentials() {
        displayHeader("Set Admin Credentials");
        cout << YELLOW << "No admin credentials found. Please set them now.\n" << RESET;  
        cout << GREEN << "Enter new username: " << RESET;  
        cin >> username;
        cout << GREEN << "Enter new password: " << RESET;  
        cin >> password;
        saveCredentials();
        cout << GREEN << "Admin credentials set successfully.\n" << RESET;  
        system("pause");

        clearScreen();
    }

public:
    Admin() {
        if (!loadCredentials()) {
            setInitialCredentials();
        }
    }

    bool login() {
        displayHeader("Admin Login");
        string inputUsername, inputPassword;
        cout << GREEN << "Enter username: " << RESET;  
        cin >> inputUsername;
        cout << GREEN << "Enter password: " << RESET;  
        cin >> inputPassword;
        return (inputUsername == username && inputPassword == password);
    }

    void changeCredentials() {
        displayHeader("Change Admin Credentials");
        string newUsername, newPassword;
        cout << GREEN << "Enter new username: " << RESET;  
        cin >> newUsername;
        cout << GREEN << "Enter new password: " << RESET;  
        cin >> newPassword;
        username = newUsername;
        password = newPassword;
        saveCredentials();
        cout << GREEN << "Admin credentials changed successfully.\n" << RESET;  
        system("pause");
    }

    void adminPanel(ContactManager& manager) {
        if (!login()) {
            cout << RED << "Login failed. Access denied.\n" << RESET;  
            system("pause");
            return;
        }

        int choice;
        while (true) {
            displayHeader("Admin Panel");
            cout << "1. Add Contact\n";
            cout << "2. View Contacts\n";
            cout << "3. Delete Contact\n";
            cout << "4. Search Contact\n";
            cout << "5. Change Admin Credentials\n";
            cout << "6. Logout\n";
            cout << "Enter your choice (1-6): ";

            if (!(cin >> choice)) {
                cout << RED << "Invalid input. Please enter a number.\n" << RESET;  
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1: manager.addContact(); break;
            case 2: manager.viewContacts(); break;
            case 3: manager.deleteContact(); break;
            case 4: manager.searchContact(); break;
            case 5: changeCredentials(); break;
            case 6: return;
            default: cout << RED << "Invalid choice. Please try again.\n" << RESET;  
            }
        }
    }
};

class Customer {
public:
    void customerPanel(ContactManager& manager) {
        int choice;
        while (true) {
            displayHeader("Customer Panel");
            cout << "1. View Contacts\n";
            cout << "2. Search Contact\n";
            cout << "3. Exit\n";
            cout << "Enter your choice (1-3): ";

            if (!(cin >> choice)) {
                cout << RED << "Invalid input. Please enter a number.\n" << RESET;  
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
            case 1: manager.viewContacts(); break;
            case 2: manager.searchContact(); break;
            case 3: return;
            default: cout << RED << "Invalid choice. Please try again.\n" << RESET;  
            }
        }
    }
};

int main() {
    ContactManager manager;
    Admin admin;
    Customer customer;
    int choice;

    displayAsciiArt();
    cout << GREEN << "Welcome to the Contact Management System!\n" << RESET;  
    system("pause");

    while (true) {
        displayHeader("Contact Management System");
        cout << "1. Admin\n";
        cout << "2. Customer\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";

        if (!(cin >> choice)) {
            cout << RED << "Invalid input. Please enter a number.\n" << RESET; 
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: admin.adminPanel(manager); break;
        case 2: customer.customerPanel(manager); break;
        case 3:
            displayHeader("Thank You");
            cout << GREEN << "Thank you for using the Contact Management System. Goodbye!\n" << RESET; 
            return 0;
        default:
            cout << RED << "Invalid choice. Please try again.\n" << RESET; 
            system("pause");
        }
    }
    return 0;
}
