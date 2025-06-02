#include <iostream>
#include <vector>
#include <memory>
using namespace std;

const double SAVINGS_INTEREST = 4.5;
const double FDR_INTEREST = 7.0;
const double DPS_INTEREST = 8.5;

class Account {
protected:
    string name, accountNumber;
    double balance;
public:
    Account(string n, string num, double bal)
        : name(n), accountNumber(num), balance(bal) {}
    virtual ~Account() = default;
    virtual void calculateInterest() = 0;
    virtual bool withdraw(double amt) {
        if (amt > balance) return cout << "Insufficient funds!\n", false;
        balance -= amt;
        cout << "Withdrawn: $" << amt << ". Remaining: $" << balance << endl;
        return true;
    }
    virtual void display() const {
        cout << "Name: " << name << "\nAccount Number: " << accountNumber
             << "\nBalance: $" << balance << endl;
    }
    void deposit(double amt) {
        balance += amt;
        cout << "Deposited: $" << amt << ". New Balance: $" << balance << endl;
    }
    string getNumber() const { return accountNumber; }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(string n, string num, double bal)
        : Account(n, num, bal) {}
    void calculateInterest() override {
        double interest = balance * SAVINGS_INTEREST / 100;
        balance += interest;
        cout << "Interest Added: $" << interest << ". Total: $" << balance << endl;
    }
    void display() const override {
        Account::display();
        cout << "Type: Savings\nInterest Rate: " << SAVINGS_INTEREST << "%\n";
    }
};

class FDRAccount : public Account {
    int term;
public:
    FDRAccount(string n, string num, double bal, int t)
        : Account(n, num, bal), term(t) {}
    void calculateInterest() override {
        double interest = balance * FDR_INTEREST / 100 * (term / 12.0);
        balance += interest;
        cout << "FDR matured. Interest: $" << interest << ". Total: $" << balance << endl;
    }
    bool withdraw(double) override {
        cout << "Cannot withdraw from FDR early!\n";
        return false;
    }
    void display() const override {
        Account::display();
        cout << "Type: FDR\nTerm: " << term << " months\nInterest Rate: " << FDR_INTEREST << "%\n";
    }
};

class DPSAccount : public Account {
    int term;
public:
    DPSAccount(string n, string num, double bal, int t)
        : Account(n, num, bal), term(t) {}
    void calculateInterest() override {
        double interest = balance * DPS_INTEREST / 100;
        balance += interest;
        cout << "DPS Interest: $" << interest << ". Total: $" << balance << endl;
    }
    void display() const override {
        Account::display();
        cout << "Type: DPS\nTerm: " << term << " years\nInterest Rate: " << DPS_INTEREST << "%\n";
    }
};

class Bank {
    vector<shared_ptr<Account>> accounts;
    int accID = 1001;

    string genAccNumber() { return "ACCT" + to_string(accID++); }
    shared_ptr<Account> find(const string& num) {
        for (auto& acc : accounts)
            if (acc->getNumber() == num) return acc;
        return nullptr;
    }

public:
    void create(const string& name, double bal, const string& type, int term = 0) {
        shared_ptr<Account> acc;
        string num = genAccNumber();
        if (type == "Savings") acc = make_shared<SavingsAccount>(name, num, bal);
        else if (type == "FDR") acc = make_shared<FDRAccount>(name, num, bal, term);
        else if (type == "DPS") acc = make_shared<DPSAccount>(name, num, bal, term);
        else return void(cout << "Invalid account type!\n");

        accounts.push_back(acc);
        cout << "Account Created Successfully:\n";
        acc->display();
    }

    void deposit(const string& num, double amt) {
        auto acc = find(num);
        if (acc) acc->deposit(amt);
        else cout << "Account not found!\n";
    }

    void withdraw(const string& num, double amt) {
        auto acc = find(num);
        if (acc) acc->withdraw(amt);
        else cout << "Account not found!\n";
    }

    void interest(const string& num) {
        auto acc = find(num);
        if (acc) acc->calculateInterest();
        else cout << "Account not found!\n";
    }

    void show(const string& num) {
        auto acc = find(num);
        if (acc) acc->display();
        else cout << "Account not found!\n";
    }

    void showAll() const {
        if (accounts.empty()) return void(cout << "No accounts to display!\n");
        for (const auto& acc : accounts) {
            acc->display();
            cout << "------------------------\n";
        }
    }
};

void menu() {
    cout << "\n1.Create\n2.Deposit\n3.Withdraw\n4.Interest\n5.Show\n6.Show All\n7.Exit\nChoice: ";
}

int main() {
    Bank bank;
    int ch, term;
    string name, type, num;
    double amt;

    do {
        menu();
        cin >> ch;
        cin.ignore();

        switch (ch) {
            case 1:
                cout << "Name: "; getline(cin, name);
                cout << "Type (Savings/FDR/DPS): "; cin >> type;
                cout << "Deposit: $"; cin >> amt;
                if (type != "Savings") { cout << "Term: "; cin >> term; bank.create(name, amt, type, term); }
                else bank.create(name, amt, type);
                break;
            case 2:
                cout << "Account Number: "; cin >> num;
                cout << "Amount: $"; cin >> amt;
                bank.deposit(num, amt);
                break;
            case 3:
                cout << "Account Number: "; cin >> num;
                cout << "Amount: $"; cin >> amt;
                bank.withdraw(num, amt);
                break;
            case 4:
                cout << "Account Number: "; cin >> num;
                bank.interest(num);
                break;
            case 5:
                cout << "Account Number: "; cin >> num;
                bank.show(num);
                break;
            case 6:
                bank.showAll();
                break;
            case 7:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option!\n";
        }
    } while (ch != 7);
    return 0;
}
