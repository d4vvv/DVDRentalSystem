#include "headers.h"
#include <iostream>

using namespace std;

    Interface::Interface() :interfaceON(true), adminLoggedIN(false), userLoggedIN(false), level("LoginMenu")                    //Constructor
    {

        while (interfaceON)
        {
            while((interfaceON) && (!userLoggedIN) && (!adminLoggedIN))
            {
                system("CLS");
                if(level == "LoginMenu") { loginMenu(); }
                if(level == "SignIn") { signIn(); }
                else if(level == "Login") { login(); }

            }
            while(interfaceON && adminLoggedIN)
            {
                system("CLS");
                if(level == "AdminMain") { adminMainMenu(); }
                else if(level == "AllDVDs") { moviesMenu(); }
                else if(level == "CheckAv") { checkAvailability(); }
                else if(level == "AddDVD") { addDVDMenu(); }
                else if(level == "DisplayOrders") { displayOrders(); }
                else if(level == "UpdateQuantity") { updateQuantity(); }
                else{adminMainMenu();}
            }
            while(interfaceON && userLoggedIN)
            {
                system("CLS");
                if(level == "UserMain") { userMainMenu(); }
                else if(level == "AllDVDs") { moviesMenu(); }
                else if(level == "CheckAv") { checkAvailability(); }
                else if(level == "ReturnDVD") { returnDVD(); }
                else if(level == "MakeOrder") { makeOrder(); }
                else if(level == "CheckOrders") { checkOrders(); }
                else{adminMainMenu();}
            }
        }
    }

    void Interface::loginMenu()
    {
        cout << "1. Log in" << endl;
        cout << "2. Make new account" << endl;
        cout << "3. Quit" << endl;
        cout << endl;

        string option;

        cout << "Choose option: ";
        cin >> option;

        if (option == "1")
            {
                level = "Login";
            }
        else if(option == "2")
            {
                level ="SignIn";
            }
        else if(option =="3")
            {
                interfaceON = false;
            }
        else
            {
                wrongInput();
            }
    }

    void Interface::login()
    {
        system("CLS");

        string login, password;

        cout << "Name: ";
        cin.ignore();
        getline(cin, login);

        cout << "Password: ";
        getline(cin, password);

        const int xx = dvd_rental_.checkLogin(login,password);
        if(xx == 1)
        {
            cout << "Successfully logged in admin!" << endl;
            adminLoggedIN = true;
            level = "AdminMain";
            system("pause");
        }
        else if(xx == 2)
        {
            cout << "Successfully logged in!" << endl;
            userLoggedIN = true;
            level = "UserMain";
            system("pause");
        }
        else
        {
            cout << "Wrong login or password" << endl;
            level = "LoginMenu";
            system("pause");
        }
    }

    void Interface::signIn()
    {
        system("CLS");

        string login, password1, password2, type;

        cout << "Please provide login name: ";
        cin.ignore();
        getline(cin, login);

        cout << "Please provide a password: ";
        getline(cin, password1);

        cout << "Please repeat the password: ";
        getline(cin, password2);

        cout << "Please choose the account type (1 - User, 2 - Admin): ";
        getline(cin, type);

        int check_login = dvd_rental_.checkIfLoginExists(login);

        if(check_login == 0)
        {
            cout << "Provided login name already exists in the system" << endl;
        }
        else
        {
            if(password1 != password2)
            {
                cout << "Provided passwords are not the same" << endl;
            }
            else
            {
                if((type != "1")&&(type != "2"))
                {
                    cout << "Provided type number was not proper" << endl;
                }
                else
                {
                    dvd_rental_.makeAccount(login,password1,type);
                }
            }
        }

        level = "LoginMenu";
        system("pause");
    }

    void Interface::userMainMenu()
    {
        system("CLS");

        cout << "1. See all DVDs" << endl;
        cout << "2. Check availability" << endl;
        cout << "3. Make an order" << endl;
        cout << "4. Check your orders" << endl;
        cout << "5. Return a DVD" << endl;
        cout << "6. Logout" << endl;
        cout << "7. Quit" << endl;
        cout << endl;

        string option;

        cout << "Choose option: ";
        cin >> option;

        if (option == "1")
            {
                level = "AllDVDs";
            }
        else if(option == "2")
            {
                level = "CheckAv";
            }
        else if(option =="3")
            {
                level = "MakeOrder";
            }
        else if(option == "4")
        {
            level = "CheckOrders";
        }
        else if(option == "5")
        {
            level = "ReturnDVD";
        }
        else if(option == "6")
            {
                userLoggedIN = false;
                level = "LoginMenu";
            }
        else if(option == "7")
            {
                interfaceON = false;
            }
        else
            {
                wrongInput();
            }
        }


    void Interface::adminMainMenu()
    {
        system("CLS");

        cout << "1. See all DVDs" << endl;
        cout << "2. Check availability" << endl;
        cout << "3. Display orders" << endl;
        cout << "4. Add a DVD" << endl;
        cout << "5. Add quantity" << endl;
        cout << "6. Logout" << endl;
        cout << "7. Quit" << endl;
        cout << endl;

        string option;

        cout << "Choose option: ";
        cin >> option;

        if (option == "1")
            {
                level = "AllDVDs";
            }
        else if(option == "2")
            {
                level = "CheckAv";
            }
        else if(option =="3")
            {
                level = "DisplayOrders";
            }
        else if(option == "4")
            {
                level = "AddDVD";
            }
        else if(option == "5")
            {
                level = "UpdateQuantity";
            }
        else if(option == "6")
            {
                adminLoggedIN = false;
                level = "LoginMenu";
            }
        else if(option == "7")
            {
                interfaceON = false;
            }
        else
            {
                wrongInput();
            }
        }

    void Interface::moviesMenu()
    {
        dvd_rental_.showTitles(adminLoggedIN);

        cout << endl;
        cout << "Go to main menu" << endl;
        if(adminLoggedIN) {level = "AdminMain";}
        else {level = "UserMain"; }
        system("pause");

    }

    void Interface::checkAvailability()
    {
        cout << "What movie would you like to check? ";

        string wanted_movie;
        cin.ignore();
        getline(cin, wanted_movie);
        dvd_rental_.checkAvailability(wanted_movie);

        if(adminLoggedIN) {level = "AdminMain";}
        else {level = "UserMain"; }
        system("pause");
    }

    void Interface::wrongInput()
    {
        system("CLS");
        cout << "You've provided wrong input" << endl;
        system("pause");
    }

    void Interface::addDVDMenu()
    {
        string title_, description_, price_, availability_;

        cout << "Please provide the title: ";
        cin.ignore();
        getline(cin, title_);

        cout << "\nPlease provide the description of the movie: ";
        getline(cin, description_);

        cout << "\nPlease specify the price of the DVD: ";
        getline(cin, price_);

        cout << "\nPlease specify the availability of the DVD: ";
        getline(cin, availability_);

        dvd_rental_.addDVD(title_, description_, price_, availability_);
        level = "AdminMain";
        system("pause");
    }

    void Interface::makeOrder()
    {
        dvd_rental_.orderTitlesShow();
        cout << "Please choose a movie: ";
        string chosen;
        cin.ignore();
        getline(cin,chosen);
        dvd_rental_.makeOrder(chosen,dvd_rental_.getCurrentLog());
        level = "UserMain";
        system("pause");
    }

    void Interface::displayOrders()
    {
        dvd_rental_.displayOrders();
        level = "AdminMain";
        system("pause");
    }

    void Interface::checkOrders()
    {
        if(dvd_rental_.anyOrders(dvd_rental_.getCurrentLog())==1)
        {
            dvd_rental_.checkOrders(dvd_rental_.getCurrentLog());
        }
        else
        {
            cout << "No orders have been made yet." << endl;
        }
        level = "UserMain";
        system("pause");
    }

    void Interface::updateQuantity()
    {
        string title_, quantity_;
        cout << "Please provide DVD title: ";
        cin.ignore();
        getline(cin, title_);
        cout << "Please provide the quantity: ";
        getline(cin, quantity_);
        dvd_rental_.updateQuantity(title_, quantity_);
        level = "AdminMain";
        system("pause");
    }
    void Interface::returnDVD()
    {
        if(dvd_rental_.anyOrders(dvd_rental_.getCurrentLog())==1)
        {
            cout << "Title of the movie you want to return: ";
            string title_;
            cin.ignore();
            getline(cin, title_);
            dvd_rental_.returnDVD(title_);
        }
        else
        {
            cout << "No orders have been made yet." << endl;
        }

        level = "UserMain";
        system("pause");
    }
