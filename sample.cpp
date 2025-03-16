#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Base class
class Employee {
protected:
    string id, name;
    double salary;

public:
    Employee(string emp_id, string emp_name, double emp_salary) : id(emp_id), name(emp_name), salary(emp_salary) {}

    virtual double computeSalary() const = 0;
    virtual void displayInfo() const = 0;

    string getID() const { return id; }

    static string toUpperCase(string str) {
        for (char &c : str) c = toupper(c);
        return str;
    }

    virtual ~Employee() {}
};

class FullTimeEmployee : public Employee {
public:
    FullTimeEmployee(string emp_id, string emp_name, double emp_salary)
        : Employee(emp_id, emp_name, emp_salary) {}

    double computeSalary() const override {
        return salary;
    }

    void displayInfo() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Fixed Monthly Salary: $" << salary << "\n";
    }
};

class PartTimeEmployee : public Employee {
private:
    int hoursWorked;

public:
    PartTimeEmployee(string emp_id, string emp_name, double hourlyWage, int hours)
        : Employee(emp_id, emp_name, hourlyWage), hoursWorked(hours) {}

    double computeSalary() const override {
        return salary * hoursWorked;
    }

    void displayInfo() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Hourly Wage: $" << salary << "\n";
        cout << "Hours Worked: " << hoursWorked << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

class ContractualEmployee : public Employee {
private:
    int projectsCompleted;

public:
    ContractualEmployee(string emp_id, string emp_name, double emp_salary, int projects)
        : Employee(emp_id, emp_name, emp_salary), projectsCompleted(projects) {}

    double computeSalary() const override {
        return salary * projectsCompleted;
    }

    void displayInfo() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n";
        cout << "Contract Payment Per Project: $" << salary << "\n";
        cout << "Projects Completed: " << projectsCompleted << "\n";
        cout << "Total Salary: $" << computeSalary() << "\n";
    }
};

bool isValidName(const string &name) {
    if (name.empty()) return false;
    if (!isalpha(name[0])) return false; // First character must be a letter
    for (size_t i = 1; i < name.size(); i++) {
        if (!isalpha(name[i]) && !(name[i] == ' ' && i + 1 < name.size() && isalpha(name[i + 1]))) {
            return false;
        }
    }
    return true;
}

int getValidMenuChoice(bool firstInput) {
    string input;
    int choice;
    bool validInput = false;

    while (!validInput) {
        cout << "\nMenu:\n";
        cout << "1 - Full-time Employee\n";
        cout << "2 - Part-time Employee\n";
        cout << "3 - Contractual Employee\n";
        cout << "4 - Display Payroll Report\n";
        cout << "5 - Exit\n";
        cout << "Enter your choice: ";
        cin >> input;

        if (input.length() == 1 && isdigit(input[0])) {
            choice = input[0] - '0';

            // Allow 1 or 5 only if it's the first input
            if (firstInput && choice != 1 && choice != 5) {
                cout << "Invalid input, please try again.\n";
            } 
            else if (choice >= 1 && choice <= 5) {
                validInput = true;
            } 
            else {
                cout << "Invalid input, please enter a number between 1 and 5.\n";
            }
        } 
        else {
            cout << "Invalid input, please enter a number between 1 and 5.\n";
        }
    }
    
    return choice;
}

bool isUniqueID(const string &id, Employee *employees[], int count) {
    string upperID = Employee::toUpperCase(id);
    for (int i = 0; i < count; i++) {
        if (Employee::toUpperCase(employees[i]->getID()) == upperID) {
            cout << "Duplicate ID!\n";
            return false;
        }
    }
    return true;
}

bool isValidSalary(const string &salary) {
    if (salary.empty()) return false;
    for (char c : salary) {
        if (!isdigit(c) && c != '.') return false;
    }
    return stod(salary) > 0;
}

int main() {
    const int MAX_EMPLOYEES = 100;
    Employee *employees[MAX_EMPLOYEES];
    int empCount = 0;
    bool running = true;
    bool firstInput = true; // Ensure 1 or 5 is selected first

    while (running) {
        int choice = getValidMenuChoice(firstInput);

        if (choice == 5) {
            running = false;
            continue;
        }

        if (choice == 4) {
            cout << "------ Employee Payroll Report ------\n";
            if (empCount == 0) {
                cout << "No employees recorded.\n";
            } else {
                for (int i = 0; i < empCount; i++) {
                    employees[i]->displayInfo();
                    cout << "---------------------------------\n";
                }
            }
            continue;
        }

        if (choice == 1) {
            firstInput = false; // Allow all menu choices after selecting 1
        }

        string id, name, salaryStr;
        double salary;
        int hoursOrProjects;

        cout << "Enter ID: ";
        cin >> id;
        if (!isUniqueID(id, employees, empCount)) continue;

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);
        if (!isValidName(name)) {
            cout << "Invalid name! Try again.\n";
            continue;
        }

        cout << "Enter Salary: ";
        cin >> salaryStr;
        if (!isValidSalary(salaryStr)) {
            cout << "Invalid salary! Try again.\n";
            continue;
        }
        salary = stod(salaryStr);

        if (choice == 2 || choice == 3) {
            cout << "Enter " << (choice == 2 ? "Hours Worked" : "Number of Projects") << ": ";
            cin >> hoursOrProjects;
            if (hoursOrProjects <= 0) {
                cout << "Invalid input! Must be a positive number.\n";
                continue;
            }
        }

        if (choice == 1) {
            employees[empCount++] = new FullTimeEmployee(id, name, salary);
        } else if (choice == 2) {
            employees[empCount++] = new PartTimeEmployee(id, name, salary, hoursOrProjects);
        } else if (choice == 3) {
            employees[empCount++] = new ContractualEmployee(id, name, salary, hoursOrProjects);
        }
    }

    for (int i = 0; i < empCount; i++) {
        delete employees[i];
    }

    return 0;
}
