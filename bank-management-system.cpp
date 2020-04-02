
#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>

using std::cout;
using std::cin;
using namespace std;

//***************************************************************
//                      ACCOUNT CLASS
//****************************************************************

class account {
    int acctno;
    char name[50];
    char type;
    int deposit;

public:
    void create_account();      // function to get new account data from user 
    void show_account() const;  // function to display stored account info
    void dep(int);              // function to accept amount and add to balance
    void draw(int);             // accepts amount and subtracts from balance
    void report() const;
    void modify();
    int retacctno() const;      // function to return stored account number
    int retdeposit() const;     // function to return balance amount
    char rettype() const;       // function to return stored account type
   
};


//***************************************************************
//              CREATE NEW ACCOUNT SCREEN
//****************************************************************

void account::create_account() {
    cout << "\nEnter an account number: ";
    cin >> acctno;
    cout<< "\nEnter name of account holder: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nEnter type of account, (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "\nEnter deposit amount (Savings - $500 min., Checking - $100 min.): ";
    cin >> deposit;
    cout << "\n\nAccount created...";
}

//***************************************************************
//               EXISTING ACCOUNT SCREEN
//****************************************************************

void account::show_account() const { 
    cout << "\nAccount number: " << acctno;
    cout << "\nAccount holder name: "; 
    cout << name;
    cout << "\n Type of account: "<< type;
    cout << "\nBalance amt.: " << deposit;
}

//***************************************************************
//               MODIFY ACCOUNT SCREEN
//****************************************************************

void account::modify() {
    cout << "\nAccount number: " << acctno;
    cout << "\nModify Account Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\nModify Account Type: ";
    cin >> type;
    cout << "\nModify Balance Amount: ";
    cin >> deposit;
}


// ACCEPT A NUMBER AND ADD NUMBER TO EXISTING DEPOSIT AMT
void account::dep(int x){
    deposit += x;
}

// ACCEPT A NUMBER AND SUBTRACT NUMBER FROM EXISTING DEPOSIT AMT

void account::draw(int x)
{
    deposit -= x;
}

void account::report() const {
    cout<<acctno << setw(10) <<" "<< name <<setw(10) << " " << type << setw(6) << deposit << endl;
}

int account::retacctno() const {
    return acctno; 
}

char account::rettype() const {
    return type;
}

int account::retdeposit() const
{
	return deposit;
}

//***************************************************************
//    	FUNCTION DECLARATIONS
//****************************************************************
void intro();
void write_account();               //write record to binary file
void deposit_withdraw(int, int);
void display_account(int);
void delete_account(int);
void display_all();
void modify_account(int);


//*************************************************************
//    	THE MAIN FUNCTION OF PROGRAM
//*************************************************************
int main() {
    char ch;
    intro();
    int num;

   do {
    // Appears after intro screen. Initial display of options.
        system("clear");
        cout << "\n\n\n\t MAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE INQUIRY";
        cout << "\n\n\t05. VIEW ALL ACCOUNTS";
        cout << "\n\n\t06. MODIFY AN ACCOUNT";
        cout << "\n\n\t07. CLOSE AN ACCOUNT";
        cout << "\n\n\t08. EXIT";
        cout << "\n\n\t Select Your Option (1-8) ";
        cin >> ch;
        system("clear");

        switch(ch) {
            case '1':
                write_account();
                break;
            case '2':
                cout << "\n\n\tEnter account number: "; cin >> num;
                deposit_withdraw(num, 1);
                break;
            case '3':
                cout << "\n\n\tEnter account number: "; cin >> num;
                deposit_withdraw(num, 2);
                break;
            case '4':
                cout << "\n\n\tEnter account number: "; cin >> num;
                display_account(num);
                break;
            case '5':
                display_all();
                break;
            case '6':
                cout << "Enter account to modify: "; cin >> num;
                modify_account(num);
                break;
            case '7':
                cout << "Enter account to close: "; cin >> num;
                delete_account(num);
                break;
            
            default:
                cout << "\a";
        }

        cin.ignore();
        cin.get();
        
    } while(ch != '8');

    return 0;
}

//***************************************************************
//    	SAVE/WRITE NEW ACCOUNT INFO TO FILE
//****************************************************************

void write_account() {
    account ac;
    std::ofstream outFile;
    outFile.open("account.dat", std::ios::binary | std::ios::app );
    ac.create_account();
    outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
    outFile.close();
}



void modify_account(int n) {
    account ac;
    bool flag = false;
    fstream File;
    File.open("account.dat", std::ios::binary | std::ios::in | std::ios::out);
    if(!File) {
        cout << "File";
        return;
    }

    while(!File.eof() && flag==false) {
        File.read(reinterpret_cast<char *>(&ac), sizeof(account));
        if(ac.retacctno() == n) {
            ac.show_account();
            ac.modify();
            int pos =(-1) * static_cast<int>(sizeof(account));
            File.seekp(pos, std::ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\n\tRecord updated!";
            flag=true;
        }
    }
    File.close();
    if(flag==false) {
        cout << "\n\nRecord not found";
    }
}
//***************************************************************
//    	DELETE ACCOUND RECORD FROM .DAT FILE
//****************************************************************

void delete_account(int n) {
    account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", std::ios::binary); 
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account))) {
        if(ac.retacctno() != n) {
            outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
        }
    }

    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\tRecord deleted...";
}

//***************************************************************
//    	DEPOSIT & WITHDRAW
//****************************************************************

void deposit_withdraw(int n, int option){
    int amt;
    bool flag=false;
    account ac;
    fstream File;

    File.open("account.dat", std::ios::binary | std::ios::in | std::ios::out);
    if(!File) {
        cout << "File could not be opened! Press any key...";
        return;
    }
    while(!File.eof() && flag==false) {
        File.read(reinterpret_cast<char *>(&ac), sizeof(account));
        if(ac.retacctno() == n) {
            ac.show_account();
            if(option==1) {
                cout << "\n\n\tDEPOSIT INTO ACCOUNT ";
                cout << "\n\nEnter deposit amount: ";
                cin >> amt;
                ac.dep(amt);
            }
            if(option==2) {
                cout << "\n\n\tWITHDRAW FROM ACCOUNT ";
                cout << "\n\nEnter amount to be withdrawn: ";
                cin >> amt;
                int bal = ac.retdeposit() - amt;
                if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C')){
                    cout << "Insufficient funds";
                } else {
                    ac.draw(amt);
                }
            }
            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, std::ios::cur);
            File.write(reinterpret_cast<char *>(&ac), sizeof(account));
            cout << "\n\nAccount updated!";
            flag=true;   
            }
        }
        File.close();
        if(flag==false) {
            cout << "\n\nRecord not found";
        }
} 


void display_account(int n) {
    account ac;
    bool flag=false;
    ifstream inFile;
    inFile.open("account.dat", std::ios::binary);
    if(!inFile) {
        cout << "File not found. Press any key...";
        return;
    }

    cout << "Balance details\n";

    while(inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        if(ac.retacctno()==n) {
            ac.show_account();
            flag=true;
        }
        inFile.close();
        if(flag==false) {
            cout << "Account does not exist";
        }
    }
    
}


void display_all() {
    account ac;
    ifstream inFile;
    inFile.open("account.dat", std::ios::binary);
    if(!inFile) {
        cout << "File not found.";
        return;
    }

    cout << "\n\n\tACCOUNT HOLDERS\n\n";
    cout<<"====================================================\n";
	cout<<"A/c no.      NAME           Type  Balance\n";
	cout<<"====================================================\n";
    while(inFile.read(reinterpret_cast<char *>(&ac), sizeof(account))) {
        ac.report();
    }
    inFile.close();
    
}

//***************************************************************
//    	INTRODUCTION FUNCTION
//****************************************************************

void intro() {
    cout << "\n\n\n\t BANK";
    cout << "\n\n\n\t MANAGEMENT>";
    cout << "\n\n\n\t SYSTEM";
    cout << "\n\n\n\t MADE BY: Seanita Tolliver";
    cin.get();
}

