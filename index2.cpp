#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include "Consolecolor.h"

using namespace std;

class Users {
private:
    string firstName;
    string lastName;
    string userName;
    string passWord;
    string role;
    int userID = 0;

public:
    // Constructor
    Users (string FirstName, string LastName, string UserName, string PassWord, string Role, int ID) {
        firstName = FirstName;
        lastName = LastName;
        userName = UserName;
        passWord = PassWord;
        role = Role;
        userID = ID;
    }

    static bool Validate(string FirstName, string LastName, string UserName, string PassWord) {
        if (FirstName.empty() || LastName.empty() || UserName.empty() || PassWord.empty()) {
            cout << "Initialization failed: Fields cannot be empty!" << endl;
            return false;
        }

        return true;
    }
    // ----- Setter and Getter -----
    string getUserName() const { return userName; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getRole() const { return role; }
    string getPassword() const { return passWord; }
    int getUserID() const { return userID;  }

};

struct UserInput {
    string FirstName;
    string LastName;
    string UserName;
    string PassWord;
    string Role;
    int ID = 0;
};

UserInput RegDisplay();
void Registration();
void Logining();
bool isAlphaString(const string& a);
bool isLowerString(const string& a);
void WriteUserToFile(const Users& user);
int randomFourDigit();
bool isUsernameUnique(const string& username);
vector<Users> ReadUsersFromFile();
int generateUniqueID();
void MainMenu();

int main()
{
    srand(time(nullptr));
    MainMenu();
}
void MainMenu() {
    char choose = '0';
    bool flag = true;
    do {
        if (!flag) {
            system("Pause");
        }
        system("CLS");
        cout << "Main Menu:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << color(RED)<< "3. Exit\n" << reset;

        cin >> choose;
        cin.ignore();

        switch (choose) {
        case '1':
            system("CLS");
            cout << "Login:\n";
            Logining();
            system("Pause");
            system("CLS");
            return;

            break;
        case '2':
            system("CLS");
            cout << "Registration:\n";
            Registration();
            break;
        case '3':
            cout << "Exiting program...\n";
            return;
        default:
            system("CLS");
            cout << color(RED) << "ERRORE: Invalid choice! Please try again.\n" << reset;
            break;
        }
        flag = false;
    }while (true);
}

UserInput RegDisplay() {
    UserInput input;
    cout << "NOTE : Name Cannot Contain Spaces!\n";
    cout << "FirstName: ";
    getline(cin, input.FirstName);
    cout << "LastName: ";
    getline(cin, input.LastName);
    cout << "Username: ";
    getline(cin, input.UserName);
    cout << "Password: ";
    getline(cin, input.PassWord);
    input.Role = "user";

    return input;
}

void Registration() {
    vector<Users> allUsers = ReadUsersFromFile();
    UserInput user = RegDisplay();

    // ----- Custome Roles -----

    const string OWNER_USERNAME = "owner";
    const string OWNER_PASSWORD = "x";

    const string ADMIN_USERNAME = "admin";
    const string ADMIN_PASSWORD = "y";

    if (!isAlphaString(user.FirstName) || !isAlphaString(user.LastName)){
        cout << "Error: First and Last names must contain only letters!\nTry Again!" << endl;
        return;
    }
    if (!isLowerString(user.UserName)){
        cout << "Error: Username must contain only lowercase letters!\nTry Again!" << endl;
        return;
    }
    if (!isUsernameUnique(user.UserName)) {
        cout << "Error: Username already exists!\nTry Again!" << endl;
        return;
    }
    if (user.PassWord.length() < 4) {
        cout << "Password must be at least 4 characters!\nTry Again!" << endl;
        return;
    }

    if (user.UserName == OWNER_USERNAME && user.PassWord == OWNER_PASSWORD) {
        user.Role = "owner";
    }

    if(user.UserName == OWNER_USERNAME && user.PassWord != OWNER_PASSWORD) {
        cout << "Initialization failed : Owner Password Is Not True!" << endl;
        return;
    }

    if (user.UserName == ADMIN_USERNAME && user.PassWord == ADMIN_PASSWORD) {
        user.Role = "admin";
    }

    if(user.UserName == ADMIN_USERNAME && user.PassWord != ADMIN_PASSWORD) {
        cout << "Initialization failed : Admin Password Is Not True!" << endl;
        return;
    }


    if (Users::Validate(user.FirstName, user.LastName, user.UserName, user.PassWord)) {
        int newID = generateUniqueID();
        Users newUser(user.FirstName, user.LastName, user.UserName, user.PassWord, user.Role,newID);
        allUsers.push_back(newUser);

        cout << "Initialized successfully! ID = " << newID << " Role = " << user.Role << endl;

        WriteUserToFile(newUser);
    }
}

void Logining() {
    string username, password, fname, lname;
    
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    vector<Users> users = ReadUsersFromFile();
    bool found = false;
    bool success = false;

    if (username == "owner" || username == "admin") {
        cout << "You Are Login As Owner or Admin. Please Enter Your Name First.\n";
        cout << "FirstName: ";
        getline(cin, fname);
        cout << "LastName: ";
        getline(cin, lname);
    }

    for (const auto& u : users) {
        if (u.getUserName() == username) {
            found = true;

            if ((username != "owner" && username != "admin") || (u.getFirstName() == fname && u.getLastName() == lname)) {
                if (u.getPassword() == password) {
                    success = true;
                    cout << "\nLogin Successful!" << endl;
                    cout << "Welcome " << u.getFirstName() << " " << u.getLastName() << endl;
                    cout << "Your Role: " << u.getRole() << endl;
                    cout << "Your ID: " << u.getUserID() << endl;

                    if (u.getRole() == "owner") cout << "You have OWNER privileges." << endl;
                    else if (u.getRole() == "admin") cout << "You have ADMIN privileges." << endl;

                    break;
                }
                else {
                    cout << "Error: Password is incorrect!" << endl;
                    return;
                }
            }
        }
    }

    if (!found) cout << "Error: Username not found!" << endl;
    else if (!success) cout << "Error: Name does not match!" << endl;
}

void WriteUserToFile(const Users& user) {
    ofstream file("users.txt", ios::app);
    if (!file) return;

    string line =
        to_string(user.getUserID()) + "|" +
        user.getFirstName() + "|" +
        user.getLastName() + "|" +
        user.getUserName() + "|" +
        user.getPassword() + "|" +
        user.getRole();

    file << line << endl;
    file.close();
}

vector<Users> ReadUsersFromFile() {
    vector<Users> users;
    ifstream file("users.txt");
    if (!file) return users;

    string line;
    while (getline(file, line)) {
        vector<string> parts;
        size_t prev = 0, pos = 0;

        while ((pos = line.find('|', prev)) != string::npos) {
            parts.push_back(line.substr(prev, pos - prev));
            prev = pos + 1;
        }
        parts.push_back(line.substr(prev));

        if (parts.size() == 6) {
            int id = stoi(parts[0]);
            string fname = parts[1];
            string lname = parts[2];
            string uname = parts[3];
            string pass = parts[4];
            string role = parts[5];

            users.emplace_back(fname, lname, uname, pass, role, id);
        }
    }

    file.close();
    return users;
}

bool isUsernameUnique(const string& username) {
    vector<Users> users = ReadUsersFromFile();

    if (username == "owner" || username == "admin") {
        return true;
    }

    for (const auto& u : users) {
        if (u.getUserName() == username) {
            return false;
        }
    }

    return true;
}

bool isAlphaString(const string& a) {
    for (char c : a) {
        if (c == ' ') {
            cout << "Error: Name Cannot Contain Spaces!\n";
            return false;
        }
        if (!isalpha(c)) return false;
    }
    return true;
}

bool isLowerString(const string& a) {
    for (char c : a) {
        if (!islower(c)) {
            if (c == '_') {
                continue;
            }
            else {
                return false;
            }
        }
    }

    for (char c : a) {
        if (c == '_') {
            continue;
        }
    }
    return true;
}

int generateUniqueID() {
    vector<Users> users = ReadUsersFromFile();
    int id;

    while (true) {
        id = randomFourDigit();
        bool unique = true;

        for (const auto& u : users) {
            if (u.getUserID() == id) {
                unique = false;
                break;
            }
        }

        if (unique) return id;
    }
}

int randomFourDigit() {
    return rand() % 9000 + 1000;
}
