#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>
using namespace std;

// Function to check if input is a valid number (only digits, no spaces or symbols)
bool isValidNumber(const string& input) {
    return !input.empty() && all_of(input.begin(), input.end(), ::isdigit);
}

// Function to get a valid numeric input from the user
double getValidNumberInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;

        if (!isValidNumber(input)) {
            cout << "Invalid input! Please enter a valid number without spaces or symbols.\n";
            continue;
        }

        return stod(input);
    }
}

// Employee Base Class
class Employee {
protected:
    string id;
    string name;
public:
    Employee(string id, string name) {
        transform(id.begin(), id.end(), id.begin(), ::toupper); // Convert ID to uppercase
        this->id = id;
        this->name = name;
    }
    virtual void displaySalary() const = 0;
    string getID() const { return id; }
    virtual ~Employee() {}
};

class FullTimeEmployee : public Employee {
private:
    double salary;
public:
    FullTimeEmployee(string id, string name, double salary) : Employee(id, name), salary(salary) {}
    void displaySalary() const override {
        cout << "Employee: " << name << " (ID: " << id << ")" << endl;
        cout << "Fixed Monthly Salary: $" << salary << "\n" << endl;
    }
};

class PartTimeEmployee : public Employee {
private:
    double hourlyWage;
    int hoursWorked;
public:
    PartTimeEmployee(string id, string name, double hourlyWage, int hoursWorked)
        : Employee(id, name), hourlyWage(hourlyWage), hoursWorked(hoursWorked) {}
    void displaySalary() const override {
        cout << "Employee: " << name << " (ID: " << id << ")" << endl;
        cout << "Hourly Wage: $" << hourlyWage << endl;
        cout << "Hours Worked: " << hoursWorked << endl;
        cout << "Total Salary: $" << (hourlyWage * hoursWorked) << "\n" << endl;
    }
};

class ContractualEmployee : public Employee {
private:
    double paymentPerProject;
    int projectsCompleted;
public:
    ContractualEmployee(string id, string name, double paymentPerProject, int projectsCompleted)
        : Employee(id, name), paymentPerProject(paymentPerProject), projectsCompleted(projectsCompleted) {}
    void displaySalary() const override {
        cout << "Employee: " << name << " (ID: " << id << ")" << endl;
        cout << "Contract Payment Per Project: $" << paymentPerProject << endl;
        cout << "Projects Completed: " << projectsCompleted << endl;
        cout << "Total Salary: $" << (paymentPerProject * projectsCompleted) << "\n" << endl;
    }
};

const int MAX_EMPLOYEES = 100;
Employee* employees[MAX_EMPLOYEES];
int employeeCount = 0;

bool isValidID(const string& input) {
    return all_of(input.begin(), input.end(), ::isalnum);
}

bool isUniqueID(string id) {
    transform(id.begin(), id.end(), id.begin(), ::toupper);
    for (int i = 0; i < employeeCount; i++) {
        string existingID = employees[i]->getID();
        if (existingID == id) return false;
    }
    return true;
}

bool isValidName(const string& name) {
    return all_of(name.begin(), name.end(), [](char ch) { return isalpha(ch) || isspace(ch); }) && !name.empty();
}

void addEmployee(int type) {
    string id, name;
    double salary;
    int hoursWorked, projectsCompleted;

    do {
        cout << "Enter ID: ";
        cin >> id;
        transform(id.begin(), id.end(), id.begin(), ::toupper); // Convert to uppercase
        if (!isValidID(id) || !isUniqueID(id)) {
            cout << "Invalid or Duplicate ID! Try again.\n";
        }
    } while (!isValidID(id) || !isUniqueID(id));

    cin.ignore();
    do {
        cout << "Enter Name: ";
        getline(cin, name);
        if (!isValidName(name)) {
            cout << "Invalid name! Only letters and spaces are allowed.\n";
        }
    } while (!isValidName(name));

    if (type == 1) {
        salary = getValidNumberInput("Enter Fixed Monthly Salary: ");
        employees[employeeCount++] = new FullTimeEmployee(id, name, salary);
    } else if (type == 2) {
        salary = getValidNumberInput("Enter Hourly Wage: ");
        hoursWorked = getValidNumberInput("Enter Hours Worked: ");
        employees[employeeCount++] = new PartTimeEmployee(id, name, salary, hoursWorked);
    } else if (type == 3) {
        salary = getValidNumberInput("Enter Payment Per Project: ");
        projectsCompleted = getValidNumberInput("Enter Projects Completed: ");
        employees[employeeCount++] = new ContractualEmployee(id, name, salary, projectsCompleted);
    }

    cin.ignore(); // Prevents menu skipping issue after input
}

void displayPayrollReport() {
    cout << "------ Employee Payroll Report ------\n";
    for (int i = 0; i < employeeCount; i++) {
        employees[i]->displaySalary();
    }
}

bool isValidChoice(const string& input) {
    return !input.empty() && input.find_first_not_of("0123456789") == string::npos && stoi(input) >= 1 && stoi(input) <= 5;
}

void menu() {
    string choiceStr;
    int choice;
    while (true) {
        cout << "\nMenu\n";
        cout << "1 - Full-time Employee\n";
        cout << "2 - Part-time Employee\n";
        cout << "3 - Contractual Employee\n";
        cout << "4 - Display Payroll Report\n";
        cout << "5 - Exit\n";
        cout << "Enter choice: ";
        getline(cin, choiceStr);

        if (!isValidChoice(choiceStr)) {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }

        choice = stoi(choiceStr);
        if (choice == 5) {
            cout << "Exiting...\n";
            break;
        }

        switch (choice) {
            case 1: case 2: case 3: addEmployee(choice); break;
            case 4: displayPayrollReport(); break;
            default: break;
        }
    }
}

int main() {
    menu();
    for (int i = 0; i < employeeCount; i++) {
        delete employees[i];
    }
    return 0;
}
