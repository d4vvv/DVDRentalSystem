#include "headers.h"

    int DVD_Rental::callback(void * NotUsed, int argc, char** argv, char** azColName)
        {
            for (int i = 0; i < argc; i++)
            {
                cout << azColName[i] << ": " << argv[i] << endl;
            }
            cout << endl;

            return 0;
        }

    int DVD_Rental::callback_co(void * NotUsed, int argc, char** argv, char** azColName)
        {
            for (int i = 0; i < argc; i++)
            {
                cout << argv[i] << endl;
            }
            cout << endl;

            return 0;
        }

    int DVD_Rental::availability_callback(void * NotUsed, int argc, char** argv, char** azColName)
        {
            if(argc == 1)
            {
                cout << "There are " << argv[0] << " copies available." << endl;
            }
            else if (argv[0] == "0")
            {
               cout << "We are sorry. There are no copies available at the moment" << endl;
            }
            else
            {
                cout << "We are sorry. We don't have such movie." << endl;
            }
            return 0;
        }

    int DVD_Rental::log_callback(void * NotUsed, int argc, char** argv, char** azColName)
        {
            int check = atoi(argv[0]);
            if(check == 1)
            {
                return 3;
            }
            else
            {
                return 0;
            }
        }

    DVD_Rental::DVD_Rental() :dir("database.db") {sqlite3_open(dir,&DB); };

    void DVD_Rental::showTitles(bool isAdmin)
    {
        string sql;
        if(isAdmin)
        {
            sql = "SELECT * FROM DVDS;";
        }
        else
        {
            sql = "SELECT TITLE, DESCRIPTION, PRICE FROM DVDS;";
        }

        sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
    }

    void DVD_Rental::orderTitlesShow()
    {
        string sql = "SELECT TITLE FROM DVDS WHERE AVAILABILITY > 0";

        sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
    }

    int DVD_Rental::determineType(string login_)
    {
        string sql = "SELECT TYPE FROM USERS WHERE NAME = \"" + login_ +"\";";

        int process = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);

        return process;
    }

    void DVD_Rental::selectDVD(string title)
    {
        string sql = "SELECT * FROM DVDS WHERE TITLE = \"" + title +"\";";

        sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
    }

    void DVD_Rental::checkAvailability(string title)
    {
        string sql = "SELECT AVAILABILITY FROM DVDS WHERE TITLE = \"" + title +"\";";

        sqlite3_exec(DB, sql.c_str(), availability_callback, NULL, NULL);
    }

    int DVD_Rental::checkLogin(string login_, string psswd_)
    {
        string sql = "SELECT CASE WHEN COUNT(PASSWORD) > 0 THEN 1 ELSE 0 END AS CHECC FROM USERS WHERE NAME = \""+login_+"\" AND PASSWORD = \""+psswd_+"\";";

        int process = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);
        if(process == 4)
        {
            currently_logged = login_;
            int UserType = determineType(login_);
            if(UserType == 4)
            {
                return 2;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }

    void DVD_Rental::addDVD(string title_, string description_, string price_, string availability_)
    {
        char* messageError;

        string sql ="INSERT INTO DVDS (TITLE, DESCRIPTION, PRICE, AVAILABILITY) VALUES('" + title_ + "','" + description_ + "'," + price_ + "," + availability_ + ");";

        int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if(process != SQLITE_OK)
        {
            cerr << "\nSome of the given parameters were wrong. DVD was not added to the database" << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nDVD added successfully!" << endl;
        }
    }

    void DVD_Rental::displayOrders()
    {
        string sql = "SELECT * FROM ORDERS WHERE TITLE != \"\";";

        sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
    }

    void DVD_Rental::makeOrder(string title_, string login_)
    {
        char* messageError;

        string sql ="UPDATE DVDS SET AVAILABILITY=AVAILABILITY-1 WHERE TITLE = '"+title_+"';";
        int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if(process != SQLITE_OK)
        {
            cerr << "\nProvided title was not proper. Order was not made." << endl;
            sqlite3_free(messageError);
        }
        else
        {
            sql = "INSERT INTO ORDERS (TITLE,BUYER) VALUES((SELECT TITLE FROM DVDS WHERE TITLE = '" + title_ + "'),'" + login_ + "');";
            process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

            if(process != SQLITE_OK)
            {
                cerr << "\nProvided title was not proper. Order was not made." << endl;
                sqlite3_free(messageError);
            }
            else
            {
                cout << "\nDVD added successfully!" << endl;
            }
        }
    }

    void DVD_Rental::updateQuantity(string title_, string quantity_)
    {
        char* messageError;

        string sql ="UPDATE DVDS SET AVAILABILITY=AVAILABILITY+"+ quantity_ + " WHERE TITLE = '"+title_+"';";
        int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if(process != SQLITE_OK)
        {
            cerr << "\nProvided inputs were not proper. Quantity was not updated." << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nQuantity updated successfully!" << endl;
        }
    }

    void DVD_Rental::checkOrders(string buyer_)
    {
        string sql = "SELECT TITLE FROM ORDERS WHERE BUYER = '" + buyer_ +"';";
        sqlite3_exec(DB, sql.c_str(), callback_co, NULL, NULL);
    }

    int DVD_Rental::anyOrders(string login_)
    {
        string sql = "SELECT CASE WHEN COUNT(BUYER) > 0 THEN 1 ELSE 0 END AS CHECC FROM ORDERS WHERE BUYER = '" + login_ + "';";
        int process = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);

        if(process == 4)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    void DVD_Rental::returnDVD(string title_)
    {
        string sql = "SELECT CASE WHEN COUNT(TITLE) > 0 THEN 1 ELSE 0 END AS CHECC FROM ORDERS WHERE BUYER = '"+currently_logged+"' AND TITLE = '"+title_+"';";

        int process_start = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);

        if(process_start == 4)
        {
            char* messageError;

            sql = "UPDATE ORDERS SET TITLE = \"\", BUYER = \"\" WHERE TITLE = '"+title_+"' AND BUYER = '"+currently_logged+"';";
            int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

            sql ="UPDATE DVDS SET AVAILABILITY=AVAILABILITY+1 WHERE TITLE = '"+title_+"';";
            sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
            if(process != SQLITE_OK)
            {
                cerr << "\nProvided inputs were not proper. Quantity was not updated." << endl;
                sqlite3_free(messageError);
            }
            else
            {
                cout << "DVD returned successfully!" << endl;
            }
        }
        else
        {
            cout << "You provided wrong title or u haven rented such movie." << endl;
        }
    }

    void DVD_Rental::makeAccount(string login_, string password_, string type_)
    {
        char* messageError;

        string sql ="INSERT INTO USERS (NAME, TYPE, PASSWORD) VALUES('" + login_ + "'," + type_ + ",'" + password_ + "');";

        int process = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

        if(process != SQLITE_OK)
        {
            cerr << "\nSomething went wrong, please try again." << endl;
            sqlite3_free(messageError);
        }
        else
        {
            cout << "\nAccount created successfully!" << endl;
        }
    }

    int DVD_Rental::checkIfLoginExists(string login_)
    {
        string sql = "SELECT CASE WHEN COUNT(NAME) > 0 THEN 1 ELSE 0 END AS CHECC FROM USERS WHERE NAME = \"" + login_ + "\";";

        int process = sqlite3_exec(DB, sql.c_str(), log_callback, NULL, NULL);

        if(process != 4)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
