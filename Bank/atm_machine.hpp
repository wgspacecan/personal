/*
 * ATM Machine
 * Version 1.2
 *
 * Created by William Gregory.
 * Copyright 2017 (c) All rights reserved.
 *
 * Info:
 * - Bank with users and acconts
 * - Bank is limited to one user at a time
 *
 * ToDo:
 * - migrate to UserInfo struct
 * - create FileIO import export
 * - add password encryption (OpenSSL)
 * - https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 * - if login username is 'exit' then exit
 * - add (h) help option
 *
 * Ideas:
 * - add login ID for ATM to specific user so only one ATM can modify a user at once
 * - - then multiple ATM instances can share the same bank object
 *
 */

//#define ATM_DEBUG

#include <vector>
#include <string>
#include <iostream>

namespace ATM {

struct AccountInfo {
    std::string name;
    unsigned int id;
    double balance;
};

struct UserInfo {
    std::string username;
    std::string userpass;
    bool admin;
    std::vector <AccountInfo> accounts;  // just info - for export
};

// personal account with basic functions to interact with balance
// complete
class Account {
private:
    AccountInfo info;
public:
	Account() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Account::Account" << std::endl;
#endif
		info.name = "null";
		info.id = 0;
		info.balance = 0;
	}
	void setup(std::string in_name, unsigned int in_id) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Account::setup" << std::endl;
#endif
		info.name = in_name;
		info.id = in_id;
	}
    bool deposit(double in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Account::deposit" << std::endl;
#endif
        info.balance += in;
        return true;
    }
    bool withdraw(double in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Account::withdraw" << std::endl;
#endif
        if (info.balance >= in) {
            info.balance -= in;
            return true;
        } return false;
    }
    AccountInfo account_info() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Account::account_info" << std::endl;
#endif
        return info;
    }
};

// stores user login and associated accounts
// complete
class User {
private:
    UserInfo info;
    std::vector <Account> accounts;
    /// functions
    int find_account(unsigned int in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::find_account" << std::endl;
#endif
        for (std::size_t i=0; i<accounts.size(); ++i) {
            if (accounts.at(i).account_info().id == in) return i;
        } return -1;
    }
public:
    User() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::User" << std::endl;
#endif
        info.username = "null";
        info.userpass = "null";
        info.admin = false;
        accounts.clear();
    }
    void setup(std::string in_name, std::string in_pass) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::setup" << std::endl;
#endif
        info.username = in_name;
        info.userpass = in_pass;
    }
	bool mod_admin(bool in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::mod_admin" << std::endl;
#endif
		info.admin = in;
        return true;
	}
	bool return_admin() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::return_admin" << std::endl;
#endif
		return info.admin;
	}
    std::string return_username() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::return_username" << std::endl;
#endif
        return info.username;
    }
    bool compare_userpass(std::string in_pass) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::compare_userpass" << std::endl;
#endif
        if (in_pass == info.userpass) return true;
        return false;
    }
    int account_create(std::string in_name) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::account_create" << std::endl;
#endif
		for (std::size_t i=0; i<accounts.size(); ++i) {
			if (in_name == accounts.at(i).account_info().name) return -1;
		}
		unsigned int i = 1; // ID
        // add check for existing account name FIX
		if (accounts.size() > 0) {
			i = accounts.at(accounts.size()-1).account_info().id+1;
		}
		Account a;
		a.setup(in_name, i);
		accounts.push_back(a);
		return i;
	}
    bool account_delete(unsigned int in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::account_delete" << std::endl;
#endif
        int i = find_account(in);
        if (accounts.at(i).account_info().balance != 0) return false;
        if (i != -1) {
            accounts.erase(accounts.begin()+i);
            return true;
        } return false;
	}
    bool deposit(unsigned int in_acc, double in_val) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::deposit" << std::endl;
#endif
        int i = find_account(in_acc);
        if (i != -1) return accounts.at(i).deposit(in_val);
		return false;
    }
    bool withdraw(unsigned int in_acc, double in_val) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::withdraw" << std::endl;
#endif
        int i = find_account(in_acc);
        if (i != -1) return accounts.at(i).withdraw(in_val);
		return false;
    }
    std::vector <AccountInfo> account_info() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::account_info" << std::endl;
#endif
        std::vector <AccountInfo> t;
		for (std::size_t i=0; i<accounts.size(); ++i) {
			t.push_back(accounts.at(i).account_info());
		} return t;
    }
    // for export
    UserInfo return_users() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: User::return_users" << std::endl;
#endif
        info.accounts.clear();
        for (std::size_t i=0; i<accounts.size(); ++i) {
            info.accounts.push_back(accounts.at(i).account_info());
        }
        return info;
    }
};

// bank manages users and their accounts
// complete
class Bank {
private:
    std::vector <User> users;
    User current_user;
    bool logged_in;
    /// functions

    // check if a user exits with given name
    int user_exists(std::string in) {
		#ifdef ATM_DEBUG
			std::cout << "ATM_DEBUG: Bank::user_exists" << std::endl;
		#endif
        for (std::size_t i=0; i<users.size(); ++i) {
            if (in == users.at(i).return_username()) return i;
        } return -1;
    }
public:
    Bank() {
		#ifdef ATM_DEBUG
			std::cout << "ATM_DEBUG: Bank::Bank" << std::endl;
		#endif
        // inital users - clear before import
        User u1;
		users.push_back(u1);
        User u2;
        u2.setup("boss", "123");
        u2.mod_admin(true);
        users.push_back(u2);
        // inital paramenters
        current_user = users.at(0);
        logged_in = false;
    }
    // admin
    // ---------------------------------
    // create account given username and userpass
    bool user_create(std::string in_name, std::string in_pass) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::user_create" << std::endl;
#endif
		if (current_user.return_admin()) {
        	if (user_exists(in_name) == -1) {
            	User u;
                u.setup(in_name, in_pass);
				users.push_back(u);
            	return true;
			}
        } return false;
    }
    // remove user with given username
    bool user_delete(std::string in_name) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::user_delete" << std::endl;
#endif
		if (current_user.return_admin()) {
        	int t = user_exists(in_name);
        	if (t != -1 && t != 0 && t != 1) {
                users.erase(users.begin()+t);
            	return true;
        	}
		} return false;
    }
    bool mod_admin (std::string in_name, bool in_mod) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::mod_admin" << std::endl;
#endif
        if (current_user.return_admin()) {
            int t =  user_exists(in_name);
            if (t != -1) return users.at(t).mod_admin(in_mod);
        } return false;
    }
    std::vector <std::string> return_usernames() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::return_usernames" << std::endl;
#endif
        std::vector <std::string> t;
        if (current_user.return_admin()) {
            for (std::size_t i=0; i<users.size(); ++i) {
                t.push_back(users.at(i).return_username());
            }
        } return t;
    }
    std::vector <UserInfo> return_users() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::return_users" << std::endl;
#endif
        std::vector <UserInfo> t;
        if (current_user.return_admin()) {
            for (std::size_t i=0; i<users.size(); ++i) {
                t.push_back(users.at(i).return_users());
            }
        } return t;
    }
    // general
    // ---------------------------------
    // login user given username and userpass
    bool login(std::string in_name, std::string in_pass) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::login" << std::endl;
#endif
        int t = user_exists(in_name);
        if (t != -1) {
            if (users.at(t).compare_userpass(in_pass)) {
                current_user = users.at(t);
                logged_in = true;
                return true;
            }
        } return false;
    }
    // logout current user
    bool logout() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::logout" << std::endl;
#endif
        if (logged_in) {
            current_user = users.at(0);
            logged_in = false;
            return true;
        } return false;
    }
    // return all accounts AccountInfo
    std::vector <AccountInfo> account_info() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::account_info" << std::endl;
#endif
        std::vector <AccountInfo> t;
        if (logged_in) t = current_user.account_info();
        return t;
    }
    // create account with given name, return ID
    int account_create(std::string in_name) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::account_create" << std::endl;
#endif
		if (logged_in) {
            return current_user.account_create(in_name);
		} return -1;
    }
    // delete account given ID
	bool account_delete(unsigned int in_id) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::account_delete" << std::endl;
#endif
		if (logged_in) {
			return current_user.account_delete(in_id);
		} return false;
	}
    // deposid given ammount into given account using ID
    bool deposit(unsigned int in_acc, double in_val) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::deposit" << std::endl;
#endif
        if (logged_in) {
			return current_user.deposit(in_acc, in_val);
		} return false;
    }
    // withdraw given ammount from given account using ID
    bool withdraw(unsigned int in_acc, double in_val) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::withdraw" << std::endl;
#endif
        if (logged_in) {
			return current_user.withdraw(in_acc, in_val);
		} return false;
    }
    bool return_state() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::return_state" << std::endl;
#endif
        return logged_in;
    }
    bool return_admin() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: Bank::return_admin" << std::endl;
#endif
        if (logged_in) return current_user.return_admin();
        return false;
    }
};

// not really needed
class Utilities {
private:

public:
    // return account ID given name
    // password encrypt
    // password decrypt
};

// import and export bank data
class FileIO {
private:
    // for users:
    // - info
    // - for accounts:
    // - - info
public:
    std::vector <UserInfo> import_bank() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: FileIO::import_bank" << std::endl;
#endif
        std::vector <UserInfo> t;
        return t;
    }
    bool export_bank(std::vector <UserInfo> in) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: FileIO::export_bank" << std::endl;
#endif
        for (std::size_t u=0; u<in.size(); ++u) {
            // users
            // in.at(u).username
            // in.at(u).userpass
            // in.at(u).admin
            for (std::size_t a=0; a<in.at(u).accounts.size(); ++a) {
                // in.at(u).acconts.at(a).name
                // in.at(u).acconts.at(a).id
                // in.at(u).acconts.at(a).balance
            }
        }
        return false;
    }
};

// for ATM display and user interaction
class ATM {
private:
    Bank b;
    /// functions
    bool login(std::string in_name, std::string in_pass) {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::login" << std::endl;
#endif
        bool t = b.login(in_name, in_pass);
        if (t) std::cout << "welcome " << in_name << std::endl;
        else std::cout << "error: login failed" << std::endl;
        return t;
    }
    void logout() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::logout" << std::endl;
#endif
        if (b.logout()) std::cout << "you have been logged out" << std::endl;
        else std::cout << "note: no users was logged in." << std::endl;
    }
    void account_info() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::account_info" << std::endl;
#endif
        std::cout << " - account info - " << std::endl;
        std::vector <AccountInfo> t = b.account_info();
        if (t.size() == 0) std::cout << "error: no accounts found" << std::endl;
        else {
            for (std::size_t i=0; i<t.size(); ++i) {
                std::cout << \
                    "name: " << t.at(i).name << \
                    "\t ID: " << t.at(i).id << \
                    "\t balance: " << t.at(i).balance << \
                    std::endl;
            }
        }
    }
    void account_create() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::account_create" << std::endl;
#endif
        std::cout << " - create account - " << std::endl;
        std::cout << "name: ";
        std::string t;
        std::cin >> t;
        int t2 = b.account_create(t);
        if(t2 != -1) std::cout << "account created with ID: " << t2 << std::endl;
        else std::cout << "error: account name already exists" << std::endl;
    }
    void account_delete() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::account_delete" << std::endl;
#endif
        // add name and ID option FIX
        std::cout << " - delete account - " << std::endl;
        std::cout << "ID: ";
        unsigned int t;
        std::cin >> t;
        if (b.account_delete(t)) std::cout << "account deleted" << std::endl;
        else std::cout << "error: account does not exist" << std::endl;
    }
    void deposit() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::deposit" << std::endl;
#endif
        std::cout << " - deposit - " << std::endl;
        std::cout << "account ID: ";
        unsigned int t;
        std::cin >> t;
        std::cout << "amount: ";
        double t2;
        std::cin >> t2;
        if (b.deposit(t, t2)) std::cout << "deposit made" << std::endl;
        else std::cout << "error: account does not exist" << std::endl;
    }
    void withdraw() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::withdraw" << std::endl;
#endif
        std::cout << " - withdraw - " << std::endl;
        std::cout << "account ID: ";
        unsigned int t;
        std::cin >> t;
        std::cout << "amount: ";
        double t2;
        std::cin >> t2;
        if (b.withdraw(t, t2)) std::cout << "withdraw made" << std::endl;
        else std::cout << "error: account does not exist or insufficient funds" << std::endl;
    }
    void user_create() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::user_create" << std::endl;
#endif
        std::cout << " - create user -" << std::endl;
        std::cout << "username: ";
        std::string tu;
        std::cin >> tu;
        std::cout << "userpass: ";
        std::string tp;
        std::cin >> tp;
        if (b.user_create(tu, tp)) std::cout << "user created" << std::endl;
        else std::cout << "error: failed to create user" << std::endl;
    }
    void user_delete() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::user_delete" << std::endl;
#endif
        std::cout << " - delete user -" << std::endl;
        std::cout << "username: ";
        std::string tu;
        std::cin >> tu;
        if (b.user_delete(tu)) std::cout << "user deleted" << std::endl;
        else std::cout << "error: failed to delete user" << std::endl;
    }
    void mod_admin() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::mod_admin" << std::endl;
#endif
        std::cout << " - mod admin - " << std::endl;
        std::cout << "username: ";
        std::string tu;
        std::cin >> tu;
        std::cout << "admin (y, n): ";
        std::string ta;
        std::cin >> ta;
        bool tat = false;
        if (ta.at(0) == 'y') tat = true;
        if (b.mod_admin(tu, tat)) std::cout << "user mod admin success" << std::endl;
        else std::cout << "error: failed to mod admin" << std::endl;
    }
    void display_users() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::display_users" << std::endl;
#endif
        std::cout << " - display users - " << std::endl;
        std::vector <std::string> t = b.return_usernames();
        for (std::size_t i=0; i<t.size(); ++i) {
            std::cout << t.at(i);
            if (i != t.size()-1) std::cout << ", ";
        } std::cout << " ;" << std::endl;
    }
    void export_bank() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::export_bank" << std::endl;
#endif
        std::cout << " - export bank - WIP - " << std::endl;
    }
    void import_bank() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::import_bank" << std::endl;
#endif
        std::cout << " - import bank - WIP - " << std::endl;
    }
    //
    void user_menu() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::user_menu" << std::endl;
#endif
        while (b.return_state()) {
            std::cout << std::endl;
            std::cout << "User Menu" << std::endl;
            std::cout << "-----------------" << std::endl;
            std::cout << "(0) logout" << std::endl;
            std::cout << "(1) deposit" << std::endl;
            std::cout << "(2) withdraw" << std::endl;
            std::cout << "(3) account info" << std::endl;
            std::cout << "(4) delete account" << std::endl;
            std::cout << "(5) create account" << std::endl;
            if (b.return_admin()) {
                std::cout << "ADMIN" << std::endl;
                std::cout << "(6) create user" << std::endl;
                std::cout << "(7) delete user" << std::endl;
                std::cout << "(8) mod admin" << std::endl;
                std::cout << "(9) display users" << std::endl;
                std::cout << "(e) export bank WIP" << std::endl;
                std::cout << "(i) import bank WIP" << std::endl;
            }
            std::cout << std::endl;
            std::cout << "option: ";
            unsigned int o;
            std::cin >> o;
            std::cout << std::endl;
            if (o == 'e' || o == 'i' || o>5) {
                if (!b.return_admin()) {
                    std::cout << "error: unknown option" << std::endl;
                } else {
                    if (o == 'e') export_bank();
                    else if (o == 'i') import_bank();
                    else {
                        switch (o) {
                            case 6:
                                user_create();
                                break;
                            case 7:
                                user_delete();
                                break;
                            case 8:
                                mod_admin();
                                break;
                            case 9:
                                display_users();
                                break;
                            default:
                                std::cout << "error: unknown option" << std::endl;
                                break;
                        }
                    }
                }
            } else {
                switch (o) {
                    case 0:
                        logout();
                        break;
                    case 1:
                        deposit();
                        break;
                    case 2:
                        withdraw();
                        break;
                    case 3:
                        account_info();
                        break;
                    case 4:
                        account_delete();
                        break;
                    case 5:
                        account_create();
                        break;
                    default:
                        std::cout << "error: unknown option" << std::endl;
                        break;
                }
            }
        }
    }
public:
    void use_atm() {
#ifdef ATM_DEBUG
	std::cout << "ATM_DEBUG: ATM::use_atm" << std::endl;
#endif
        if (b.return_state()) b.logout();
        std::cout << std::endl;
        std::cout << "ATM System Active" << std::endl;
        std::cout << "Version 1.2" << std::endl;
        std::cout << std::endl;
        std::cout << "Login Menu" << std::endl;
        std::cout << "-----------------" << std::endl;
        std::cout << "username: ";
        std::string tu;
        std::cin >> tu;
        std::cout << "userpass: ";
        std::string tp;
        std::cin >> tp;
        login(tu, tp);
        if (b.return_state()) user_menu();
    }
};

}
