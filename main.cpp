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
        cout<<"Database Names"<<endl;
        cout<<"==============================="<<endl;
        for(int  i = 0;i < databaseList.size() ; i++)
            cout<<databaseList[i].name<<endl;
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
    static Database SearchedDatabase(string _name)
    {
        for(int i = 0; i < databaseList.size();i++)
        {
            if(_name.compare(databaseList[i].name) == 0)
                return databaseList[i];
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
    static Table SearchedTable(string tableName,Database currentDatabase)
    {
        for(int i = 0; i < currentDatabase.tableList.size();i++)
        {
            if(tableName.compare(currentDatabase.tableList[i].name) == 0)
                return currentDatabase.tableList[i];
        }
    }
    void ShowTable()
    {
        cout<<"Tables Name of : "<<this->name<<endl;
        cout<<"==============================="<<endl;
        for(int i = 0; i < this->tableList.size(); i++)
            cout<<this->tableList[i].name<<endl;
        cout<<"Press any key to return"<<endl;
    }
    static void DeleteTable(string _name,Database currentDatabase)
    {
        auto it = currentDatabase.tableList.begin();
        for(int i = 0 ;i < currentDatabase.tableList.size(); i++)
        {
            if(_name.compare(currentDatabase.tableList[i].name) == 0)
            {
                currentDatabase.tableList.erase(it);
                break;
            }
            else
                it++;
        }
    }
};


void HelpMenu();
void DatabaseCommand(string inputToUpper);
int WordsCounting(string statement);
void TableCommand(Database currentDatabase);

vector <Database> Database::databaseList;
int main()
{

    bool flag = true;
    while(flag)
    {
    cout<<"\n-----------------------------------------------DBMS Console----------------------------------------------\n"<<endl;
    cout<<"     - Start Querying"<<endl;
    cout<<"     - Help"<<endl<<endl;;
    string input;
    getline(cin,input);
    string inputToUpper;
    for(int i = 0; i <input.length();i++)
    {
        string ch;
        ch += toupper(input[i]);
         inputToUpper.append(ch);
    }
    DatabaseCommand(inputToUpper);
    system("cls");
    }


    return 0;
}
void HelpMenu()
{
    system("cls");
    cout<<"\n---------------------------------Query Options-------------------------------------------\n"<<endl;

    cout<<"----------->Database Query"<<endl;
    cout<<"To Create Database  :  \'CREATE DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Drop Database    :  \'DROP DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Alter Database   :  \'ALTER DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Display Database :  \'SELECT DATABASE\'"<<endl;
    cout<<endl;

    cout<<"----------->Table Query"<<endl;
    cout<<"To Create Table  :    \' CREATE TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Drop Table    :    \' DROP TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Alter Table   :    \' ALTER TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Display Table :    \' SELECT TABLE \'"<<endl;
    cout<<"===================================================="<<endl;
    cout<<"Press any key to return"<<endl;
    getch();
}
void DatabaseCommand(string inputToUpper)
{
    if(inputToUpper.find("CREATE DATABASE") == 0 && WordsCounting(inputToUpper) == 3)
    {
        string _name = inputToUpper.substr(16);
        if(Database::DatabaseNameSearching(_name) == false)
        {
            system("cls");
            Database newDatabase (_name) ;
            Database::databaseList.push_back(newDatabase);
        }
        else
        {
            system("cls");
            cout<<"There is Database with the same name"<<endl;
            cout<<"Enter any key to return"<<endl;
            getch();
        }
    }
    else if(inputToUpper.find("SELECT DATABASE") == 0 && WordsCounting(inputToUpper) == 2)
    {
        if(!Database::databaseList.empty())
        {
            Database::ShowDatabase();
            cout<<"\n\n---------------------------"<<endl;
            cout<<"Enter any key to return"<<endl;
            getch();
        }
        else
        {
            system("cls");
            cout<<"No Database Exists"<<endl;
            cout<<"Enter any key to return"<<endl;
            getch();
        }

    }
    else if(inputToUpper.find("ALTER DATABASE") == 0 && WordsCounting(inputToUpper) == 3)
    {
        string databaseName;
        databaseName = inputToUpper.substr(15);
        if(Database::DatabaseNameSearching(databaseName))
        {
            Database currentDatabase = Database::SearchedDatabase(databaseName);
            TableCommand(currentDatabase);
            getch();
        }
        else
        {
            system("cls");
            cout<<"There is no Database with name : "<<databaseName<<endl;
            cout<<"Enter any key to return"<<endl;
            getch();
        }
    }
    else if(inputToUpper.find("DROP DATABASE") == 0 && WordsCounting(inputToUpper) == 3 )
    {
        string databaseName;
        databaseName = inputToUpper.substr(14);
        if(Database::DatabaseNameSearching(databaseName))
        {
            Database::DeleteDatabase(databaseName);
        }
        else
        {
            system("cls");
            cout<<"There is no Database with name : "<<databaseName<<endl;
            cout<<"Enter any key to return"<<endl;
            getch();
        }
    }
    else if(inputToUpper.compare("HELP") == 0)
            HelpMenu();
    else
    {
        cout<<"Invalid Command"<<endl;
        getch();
    }
}
void TableCommand(Database currentDatabase)
{
    bool flag = true;
    while(flag)
    {
        system("cls");
        cout<<"\n-----------------------------------------------Table Command Prompt----------------------------------------------\n"<<endl;
        cout<<"     -Enter Query "<<endl;
        cout<<"     -Return "<<endl;
        string input;
        getline(cin,input);
        string inputToUpper;
        for(int i = 0; i <input.length();i++)
        {
            string ch;
            ch += toupper(input[i]);
                inputToUpper.append(ch);
        }
        if(inputToUpper.find("CREATE TABLE") == 0 && WordsCounting(inputToUpper) == 3)
        {
            string tableName = inputToUpper.substr(12);
            // if there is a table with same name
            Table table (tableName);
            currentDatabase.tableList.push_back(table);
            getch();
        }
        else if(inputToUpper.find("SELECT TABLE") == 0 && WordsCounting(inputToUpper) == 2)
        {
            system("cls");
            string tableName = inputToUpper.substr(12);
            currentDatabase.ShowTable();
            getch();
        }
        else if(inputToUpper.find("ALTER TABLE") == 0 && WordsCounting(inputToUpper) == 3)
        {
            string tableName = inputToUpper.substr(11);
            Table table = Database::SearchedTable(tableName,currentDatabase);

            if(inputToUpper.find("ADD COLUMN") == 0 && WordsCounting(inputToUpper) == 3)
            {
                string columnName = inputToUpper.substr(10);
                table.AddingColumn(columnName);
            }
            else if(inputToUpper.find("DROP COLUMN") == 0 && WordsCounting(inputToUpper) == 3)
            {
                string columnName = inputToUpper.substr(11);
                table.DeleteColumn(columnName);
            }
        }

        else if(inputToUpper.find("DROP TABLE") == 0 && WordsCounting(inputToUpper) == 3)
        {
            system("cls");
            string tableName = inputToUpper.substr(10);
            if(currentDatabase.TableNameSearching(tableName))
            {
                Database::DeleteTable(tableName,currentDatabase);
                getch();
            }
            else
            {
                system("cls");
                cout<<"There is no Table with name : "<<tableName<<endl;
                cout<<"Enter any key to return"<<endl;
                getch();
            }

        }
        else if(inputToUpper.compare("RETURN"))
            flag = false;
        else
            cout<<"Invalid Query"<<endl;
        }


}
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


