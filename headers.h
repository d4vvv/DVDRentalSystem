#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

class DVD_Rental                                                                                        //Class containing mechanisms of the DVD Rental system
{
private:
    char* dir;                                                                                          //variable keeping database file name
    sqlite3* DB;                                                                                        //variable needed for sqlite database to work
    string currently_logged;                                                                            //currently logged in account
    static int callback(void * NotUsed, int argc, char** argv, char** azColName);                       //callback function displaying contents of certain tables from database
    static int callback_co(void * NotUsed, int argc, char** argv, char** azColName);                    //callback function displaying simplified contents of certain tables from database
    static int availability_callback(void * NotUsed, int argc, char** argv, char** azColName);          //callback for checking availability of a DVD
    static int log_callback(void * NotUsed, int argc, char** argv, char** azColName);                   //callback checking if output of sqlite query is 1 or something else

public:
    DVD_Rental();                                                                                       //constructor for DVD_Rental class
    void showTitles(bool isAdmin);                                                                      //method showing all DVDS (Slightly different for Admin and User)
    void orderTitlesShow();                                                                             //method for displaying all available DVDs
    int determineType(string login_);                                                                   //method returning type of currently logged in person
    void selectDVD(string title);                                                                       //method for displaying info about certain DVD
    void checkAvailability(string title);                                                               //method displaying availability info of some certain DVD
    int checkLogin(string login_, string psswd_);                                                       //method for validating login
    void addDVD(string title_, string description_, string price_, string availability_);               //method adding a new DVD to the database (available only for Admin)
    void displayOrders();                                                                               //method displaying orders (available only for Admin)
    void makeOrder(string title_, string login_);                                                       //method for making orders (available only for User)
    void updateQuantity(string title_, string quantity_);                                               //method for "updating availability" of a DVD
    void checkOrders(string buyer_);                                                                    //method displaying orders of a certain user
    int anyOrders(string login_);                                                                       //method checking if any orders were made
    void returnDVD(string title_);                                                                      //method for returning a DVD
    void makeAccount(string login_, string password_, string type_);                                    //method for creating a new account
    string getCurrentLog() { return currently_logged; };                                                //getter for currently_logged
    int checkIfLoginExists(string login_);                                                              //method for checking weather account with given login name exists
};

class Interface                                                                                         //class responsible for the communication with user
{
private:
    bool interfaceON;                                                                                   //variable determining weather the whole interface is on or off
    bool adminLoggedIN;                                                                                 //variable saying that currently logged person is Admin
    bool userLoggedIN;                                                                                  //variable saying that currently logged person is User
    string level;                                                                                       //variable determining current level of the interface
    DVD_Rental dvd_rental_;                                                                             //object of class DVD_Rental

public:
    Interface();                                                                                        //Constructor of class Interface responsible for overall program behavior
    void loginMenu();                                                                                   //Base Menu with availability to log in or quit
    void login();                                                                                       //Logging in menu
    void userMainMenu();                                                                                //Main menu for User type of account
    void adminMainMenu();                                                                               //Main menu for Admin type of account
    void moviesMenu();                                                                                  //Menu with list of all DVDS
    void checkAvailability();                                                                           //Menu with possibility to check availability of a certain DVD
    void wrongInput();                                                                                  //Method determining weather input is correct or not
    void addDVDMenu();                                                                                  //Menu allowing to add a DVD
    void makeOrder();                                                                                   //Menu allowing to order a DVD
    void displayOrders();                                                                               //Menu displaying orders
    void checkOrders();                                                                                 //Menu displaying orders for a certain user
    void updateQuantity();                                                                              //Menu for updating the quantity of a DVD
    void returnDVD();                                                                                   //Menu allowing to return a DVD
    void signIn();                                                                                      //Menu allowing to make an account
};
