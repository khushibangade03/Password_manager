#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream> 
#include <ctime>
class PasswordEntry
{
public:
    std::string username;
    std::string password;
    std::string service;
    std::string lastModified;

    PasswordEntry(const std::string &u, const std::string &p, const std::string &s)
        : username(u), password(p), service(s)
    {
        updateLastModified();
    }

    void updateLastModified()
    {
        time_t now = time(0);
        lastModified = ctime(&now);
        lastModified.pop_back(); // Remove newline
    }
};

class PasswordManager
{
private:
    std::vector<PasswordEntry> entries;
    const std::string filename = "passwords.txt";

    void saveToFile()
    {
        std::ofstream file(filename);
        for (const auto &entry : entries)
        {
            file << entry.service << "|" << entry.username << "|" << entry.password << "|" << entry.lastModified << "\n";
        }
        file.close();
    }

    void loadFromFile()
    {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string service, username, password, lastModified;
            std::getline(iss, service, '|');
            std::getline(iss, username, '|');
            std::getline(iss, password, '|');
            std::getline(iss, lastModified);
            entries.emplace_back(username, password, service);
            entries.back().lastModified = lastModified;
        }
        file.close();
    }

public:
    PasswordManager()
    {
        loadFromFile();
    }

    void addEntry()
    {
        std::string username, password, service;

        std::cout << "Enter service: ";
        std::getline(std::cin, service);

        std::cout << "Enter username: ";
        std::getline(std::cin, username);

        std::cout << "Enter password: ";
        std::getline(std::cin, password);

        entries.emplace_back(username, password, service);
        std::cout << "Entry added successfully!\n";
        saveToFile();
    }

    void viewEntries()
    {
        if (entries.empty())
        {
            std::cout << "No entries found.\n";
            return;
        }

        for (size_t i = 0; i < entries.size(); ++i)
        {
            std::cout << "Entry " << i + 1 << ":\n";
            std::cout << "  Service: " << entries[i].service << "\n";
            std::cout << "  Username: " << entries[i].username << "\n";
            std::cout << "  Password: " << entries[i].password << "\n";
            std::cout << "  Last Modified: " << entries[i].lastModified << "\n\n";
        }
    }

    void searchEntries()
    {
        std::string searchTerm;
        std::cout << "Enter search term: ";
        std::getline(std::cin, searchTerm);

        bool found = false;
        for (const auto &entry : entries)
        {
            if (entry.service.find(searchTerm) != std::string::npos ||
                entry.username.find(searchTerm) != std::string::npos)
            {
                std::cout << "Service: " << entry.service << "\n";
                std::cout << "Username: " << entry.username << "\n";
                std::cout << "Password: " << entry.password << "\n";
                std::cout << "Last Modified: " << entry.lastModified << "\n\n";
                found = true;
            }
        }

        if (!found)
        {
            std::cout << "No matching entries found.\n";
        }
    }

    void updateEntry()
    {
        std::string service;
        std::cout << "Enter the service name of the entry to update: ";
        std::getline(std::cin, service);

        auto it = std::find_if(entries.begin(), entries.end(),
                               [&service](const PasswordEntry &entry)
                               { return entry.service == service; });

        if (it != entries.end())
        {
            std::cout << "Enter new username (leave blank to keep current): ";
            std::string newUsername;
            std::getline(std::cin, newUsername);
            if (!newUsername.empty())
            {
                it->username = newUsername;
            }

            std::cout << "Enter new password (leave blank to keep current): ";
            std::string newPassword;
            std::getline(std::cin, newPassword);
            if (!newPassword.empty())
            {
                it->password = newPassword;
            }

            it->updateLastModified();
            std::cout << "Entry updated successfully!\n";
            saveToFile();
        }
        else
        {
            std::cout << "Entry not found.\n";
        }
    }

    void deleteEntry()
    {
        std::string service;
        std::cout << "Enter the service name of the entry to delete: ";
        std::getline(std::cin, service);

        auto it = std::remove_if(entries.begin(), entries.end(),
                                 [&service](const PasswordEntry &entry)
                                 { return entry.service == service; });

        if (it != entries.end())
        {
            entries.erase(it, entries.end());
            std::cout << "Entry deleted successfully!\n";
            saveToFile();
        }
        else
        {
            std::cout << "Entry not found.\n";
        }
    }
};

void printMenu()
{
    std::cout << "\nPassword Manager Menu:\n";
    std::cout << "1. Add new entry\n";
    std::cout << "2. View all entries\n";
    std::cout << "3. Search entries\n";
    std::cout << "4. Update entry\n";
    std::cout << "5. Delete entry\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

int main()
{
    PasswordManager manager;
    int choice;

    std::cout << "Welcome to the Enhanced Password Manager!\n";

    while (true)
    {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            manager.addEntry();
            break;
        case 2:
            manager.viewEntries();
            break;
        case 3:
            manager.searchEntries();
            break;
        case 4:
            manager.updateEntry();
            break;
        case 5:
            manager.deleteEntry();
            break;
        case 6:
            std::cout << "Thank you for using the Password Manager. Goodbye!\n";
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
