#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>

using namespace std;

class Table
{
    public:
    string name;
    vector <vector<string>> columnArray;
    Table(string _name)
    {
        this->name = _name;
    }
    void AddingColumn(string _name)
    {
        vector <string> newCol;
        newCol.push_back(_name);
        columnArray.push_back(newCol);
    }
    void DeleteColumn(string _name)
    {
        auto it = columnArray.begin();
        for(int i = 0; i < columnArray.size();i++)
        {
            if(_name.compare(columnArray[i][0]) == 0)
                columnArray.erase(it);
            else
                it++;
        }
    }
    bool ColumnNameSearching(string _name)
    {
        for(int i = 0; i < (this->columnArray.size()) ; i++)
        {
            if(_name.compare(this->columnArray[i][0]))
            {
                cout<<this->columnArray[i][0];
                getch();
                return true;
            }

        }
        return false;
    }

};
class Database
{
public:
    string name;
    vector <Table> tableList;
    static vector <Database> databaseList;
    Database(string _name)
    {
        this->name = _name;
    }
    static void ShowDatabase()
    {
        system("cls");
        cout<<"Existing Databases"<<endl;
        cout<<"====================="<<endl;
        for(int  i = 0;i < databaseList.size() ; i++)
            cout<<databaseList[i].name<<endl;
        cout<<"-----------------------"<<endl;
        cout<<"Enter any key to return"<<endl;
        getch();
    }
    static bool DatabaseNameSearching(string _name)
    {
        for(int i = 0; i < databaseList.size();i++)
        {
            if(_name.compare(databaseList[i].name) == 0)
                return true;
        }
        return false;
    }
    static Database* SearchedDatabase(string _name)
    {
        for(int i = 0; i < databaseList.size();i++)
        {
            if(_name.compare(databaseList[i].name) == 0)
                return &databaseList[i];
        }
    }
    static void DeleteDatabase(string _name)
    {
        auto it = databaseList.begin();
        for(int i = 0; i < databaseList.size();i++)
        {
            if(_name.compare(databaseList[i].name) == 0)
                databaseList.erase(it);
            else
                it++;
        }
    }
    // --------------->> Table Methods
    bool TableNameSearching(string _name)
    {
        for(int i = 0; i < this->tableList.size();i++)
        {
            if(_name.compare(this->tableList[i].name) == 0)
                return true;
        }
        return false;
    }
    Table* SearchedTable(string tableName)
    {
        for(int i = 0; i < this->tableList.size();i++)
        {
            if(tableName.compare(this->tableList[i].name) == 0)
                return & this->tableList[i];
        }
    }
    void ShowTable()
    {
        cout<<"Tables of Database : "<<this->name<<endl;
        cout<<"==============================="<<endl;
        for(int i = 0; i < tableList.size(); i++)
            cout<<tableList[i].name<<endl;
        cout<<"Press any key to return"<<endl;
    }
    void DeleteTable(string _name)
    {
        auto it = this->tableList.begin();
        for(int i = 0 ;i < this->tableList.size(); i++)
        {
            if(_name.compare(this->tableList[i].name) == 0)
            {
                this->tableList.erase(it);
                break;
            }
            else
                it++;
        }
    }
};
//Utility Functions
int WordsCounting(string statement)
{
    int count = 0;
    for (int i = 0; statement[i] != '\0';i++)
    {
        if (statement[i] == ' ')
            count++;
    }
    return ++count;
}
void ErrorScreen(string errorMessage)
{
    system("cls");
    cout<<errorMessage<<endl;
    cout<<"Enter any key to return"<<endl;
    getch();
}
void HelpMenu()
{
    system("cls");
    cout<<"\n---------------------------------Query Options-------------------------------------------\n"<<endl;

    cout<<"----------->Database Query"<<endl;
    cout<<"To Create Database  :  \'CREATE DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Drop Database    :  \'DROP DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Display Database :  \'SHOW DATABASES\'"<<endl;
    cout<<endl;

    cout<<"----------->Table Query"<<endl;
    cout<<"To Create Table of Database      :    \' USING DATABASE [DATABASE NAME] CREATE TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Drop Table  of Database       :    \' USING DATABASE [DATABASE NAME] DROP TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Add Column to Table           :    \'USING DATABASE [DATABASE NAME] ALTER TABLE [TABLE NAME] ADD COLUMN [NAME]\'"<<endl;
    cout<<"To DELETE Column to Table        :    \'USING DATABASE [DATABASE NAME] ALTER TABLE [TABLE NAME] DELETE COLUMN [NAME]\'"<<endl;
    cout<<"To Display Tables Of Database    :    \' USING DATABASE [DATABASE NAME] SHOW TABLES \'"<<endl;
    cout<<"===================================================="<<endl;
    cout<<"Press any key to return"<<endl;
    getch();
}

void Heading()
{
    system("cls");
    cout<<"\t\t\t\t\t      ****************************"<<endl;
    cout<<"------------------------------------------------------DBMS Console------------------------------------------------------\n"<<endl;
    cout<<"     - Start Querying"<<endl;
    cout<<"     - Help"<<endl;
    cout<<"     - Exit"<<endl<<endl;
}
vector <Database> Database::databaseList;
void ConsoleCommand()
{
    Heading();
    bool flag = true;
    while(flag)
    {
        string input;
        getline(cin,input);
        string originalInput = input;
        transform(input.begin(),input.end(),input.begin(),::toupper);
        if(input.find("CREATE DATABASE") == 0 && WordsCounting(input) == 3)
        {
            string databaseName =  originalInput.substr(16);
            if(Database::DatabaseNameSearching(databaseName) == false)
            {
                Database newDatabase (databaseName) ;
                Database::databaseList.push_back(newDatabase);
            }
            else
            {
                ErrorScreen("Can't Have Duplicated Database");
                Heading();
            }

        }
        else if(input.find("SHOW DATABASES") == 0 && WordsCounting(input) == 2)
        {
            if(!Database::databaseList.empty())
            {
                Database::ShowDatabase();
                Heading();
            }
            else
            {
                ErrorScreen("No Database Exists");
                Heading();
            }
        }
        else if(input.find("DROP DATABASE") == 0 && WordsCounting(input) == 3 )
        {
            string databaseName = originalInput.substr(14);
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database::DeleteDatabase(databaseName);
            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("CREATE TABLE") != string::npos && WordsCounting(input) == 6)
        {
            string databaseName = originalInput.substr(15,input.find("CREATE TABLE") - 16);
            string tableName = originalInput.substr(input.find("CREATE TABLE") + 13);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                if(!currentDatabase->TableNameSearching(tableName))
                {
                    Table table(tableName);
                    currentDatabase->tableList.push_back(table);
                }
                else
                {
                    ErrorScreen("Can't Have Duplicate Table");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }


        }
        else if(input.find("USING DATABASE") == 0 && input.find("SHOW TABLES") != string::npos && WordsCounting(input) == 5)
        {

            string databaseName = originalInput.substr(15,input.find("SHOW TABLE") - 16);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                system("cls");
                currentDatabase->ShowTable();
                getch();
                Heading();
            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }

        }
        else if(input.find("USING DATABASE") == 0 && input.find("DROP TABLE") != string::npos && WordsCounting(input) == 6)
        {
            string databaseName = originalInput.substr(15,input.find("DROP TABLE") - 16);
            string tableName = originalInput.substr(input.find("DROP TABLE") + 11);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                if(currentDatabase->TableNameSearching(tableName))
                {
                    currentDatabase->DeleteTable(tableName);
                    getch();
                }
                else
                {
                    ErrorScreen("No Table With This Name Exists");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("ALTER TABLE") != string::npos
                && input.find("ADD COLUMN")!= string::npos && WordsCounting(input) == 9)
        {
            string databaseName = originalInput.substr(15,input.find("ALTER TABLE") - 16);
            if(Database::DatabaseNameSearching(databaseName))
            {
                string tableName = originalInput.substr(input.find("ALTER TABLE") + 12);
                string usedToCutText = tableName;
                transform(usedToCutText.begin(),usedToCutText.end(),usedToCutText.begin(),::toupper);
                tableName.erase(usedToCutText.find("ADD COLUMN") - 1);
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    string columnName = originalInput.substr(input.find("ADD COLUMN") + 11);
                    if(!currentTable->ColumnNameSearching(columnName))
                    {
                        currentTable->AddingColumn(columnName);
                    }
                    else
                    {
                        ErrorScreen("Can't Have Duplicate Column");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("No Table With This Name Exists");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("ALTER TABLE") != string::npos
        && input.find("DELETE COLUMN")!= string::npos && WordsCounting(input) == 9 )
        {
            string databaseName = originalInput.substr(15,input.find("ALTER TABLE") - 16);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                string tableName = originalInput.substr(input.find("ALTER TABLE") + 12);
                string usedToCutText = tableName;
                transform(usedToCutText.begin(),usedToCutText.end(),usedToCutText.begin(),::toupper);
                tableName.erase(usedToCutText.find("DELETE COLUMN") - 1);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    string columnName = originalInput.substr(input.find("DELETE COLUMN") + 13);
                    if(currentTable->ColumnNameSearching(columnName))
                    {
                        currentTable->DeleteColumn(columnName);
                    }
                    else
                    {
                        ErrorScreen("No Column With This Name Exists");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("No Table With This Name Exists");
                    Heading();
                }

            }
            else
            {
                ErrorScreen("No Database With This Name Exists");
                Heading();
            }
        }
        else if(input.compare("HELP") == 0)
        {
            HelpMenu();
            Heading();
        }
        else if(input.compare("EXIT") == 0)
            flag = false;
        else
            cout<<"Wrong Query"<<endl;
    }

}
int main()
{
    ConsoleCommand();
    return 0;
}
