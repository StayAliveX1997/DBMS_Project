#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <direct.h>
#include <dirent.h>
#include <sstream>

using namespace std;

// #---------------------------->Utility Functions
void Heading()
{
    system("cls");
    cout<<"\t\t\t\t\t      ****************************"<<endl;
    cout<<"------------------------------------------------------DBMS CONSOLE------------------------------------------------------\n"<<endl;
    cout<<"     - WRITE QUERY"<<endl;
    cout<<"     - HELP"<<endl;
    cout<<"     - EXIT"<<endl<<endl;
}
void HelpMenu()
{
    system("cls");
    cout<<"\n-------------------------------------------QUERY SYNTAX-------------------------------------------\n"<<endl;

    cout<<"-------------------->Database Query"<<endl;
    cout<<"To Create Database               :  \'CREATE DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Drop Database                 :  \'DROP DATABASE [DATABASE NAME] \'"<<endl;
    cout<<"To Display Database              :  \'SHOW DATABASES\'"<<endl;
    cout<<endl;
    cout<<"-------------------->Table Query"<<endl;
    cout<<"To Create Table of Database      :    \'USING DATABASE [DATABASE NAME] CREATE TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Display Tables Of Database    :    \'USING DATABASE [DATABASE NAME] SHOW TABLES \'"<<endl;
    cout<<"To Drop Table  of Database       :    \'USING DATABASE [DATABASE NAME] DROP TABLE [TABLE NAME] \'"<<endl;
    cout<<"To Add Column to Table           :    \'USING DATABASE [DATABASE NAME] ALTER TABLE [TABLE NAME] ADD COLUMN [NAME]\'"<<endl;
    cout<<"To DELETE Column to Table        :    \'USING DATABASE [DATABASE NAME] ALTER TABLE [TABLE NAME] DELETE COLUMN [NAME]\'"<<endl;
    cout<<endl;
    cout<<"To SELECT FIELDS                 :    \'USING DATABASE [DATABASE NAME] SELECT [* | COLUMN NAME,...] FROM [TABLE NAME] WHERE [OPTIONAL] COLUMN=VALUE \'"<<endl;
    cout<<"To INSERT VALUES into Columns    :    \'USING DATABASE [DATABASE NAME] INSERT INTO [TABLE NAME] (COLUMNS NAMES,...) VALUES [VALUES,...]\'"<<endl;
    cout<<"To UPDATE VALUES into Columns    :    \'USING DATABASE [DATABASE NAME] UPDATE TABLE [TABLE NAME] SET [COLUMN=VALUE,..] WHERE COLUMN=VALUE \'"<<endl;
    cout<<"To DELETE Record to Table        :    \'USING DATABASE [DATABASE NAME] DELETE FROM [TABLE NAME] WHERE COLUMN=VALUE\'"<<endl;
    cout<<endl;
    cout<<"To SAVE DATABASE                 :    \'SAVE DATABASE [DATABASE NAME]\'"<<endl;
    cout<<"To LOAD DATABASE                 :    \'LOAD DATABASE [DATABASE NAME]\'"<<endl;
    cout<<endl;
    cout<<"===================================================="<<endl;
    cout<<"PRESS ANY KEY TO RETURN..."<<endl;
    getch();
}
void ErrorScreen(string errorMessage)
{
    system("cls");
    cout<<errorMessage<<endl;
    cout<<"PRESS ANY KEY TO RETURN..."<<endl;
    getch();
}
void gotoxy(int x, int y)
{
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}
int WordsCounting(string statement)
{
    int count = 0;
    for (int i = 0; statement[i] != '\0';i++)
    {
        if (statement[i] == ' ' )
            if(i > 0 && statement[i-1] != ' ')
                count++;
    }
    return ++count;
}
vector<string> StringSplit(string input,string _delimiter)
{
    vector<string> wordsArray;
    int pos = 0; //point of start
    string token;
    string delimiter = _delimiter;
    while ((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        wordsArray.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    wordsArray.push_back(input);
    return wordsArray;
}
vector<vector<string>> SplitingIntoColumnNamesAndValues(string columnNameToValues)
{
    vector<string> seperatedColumnNameToValues = StringSplit(columnNameToValues,",");
    vector<string> columnNames;
    vector<string> values;

    for(int i = 0; i < seperatedColumnNameToValues.size(); i++)
    {
        vector<string> tempVec = StringSplit(seperatedColumnNameToValues[i],"=");
        columnNames.push_back(tempVec[0]);
        values.push_back(tempVec[1]);
    }
    vector<vector<string>> result;
    result.push_back(columnNames);
    result.push_back(values);
    return result;
}

vector<string> ListingDirectery(string _relativePath)
{
    vector<string> filesName;
    DIR *directory;
    struct dirent *entry;
    char path[_relativePath.length()+3] = "./"; //     ./ emp
    strcat(path,_relativePath.c_str());
    directory = opendir(path);
    if(directory != NULL)
    {
        int i = 0;
        while((entry = readdir(directory)) != NULL)
        {
         if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 )
            continue;
         else
         {
            string _fileName(entry->d_name);
            if(_fileName.find(".txt") != string::npos)
                filesName.push_back(_fileName);
         }
        }
        closedir(directory);
    }
    else
        ErrorScreen("ERROR Couldn't Open Folder!!");
    return filesName;
}


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
        if(columnArray.size() >= 2 && columnArray[0].size() > 1)
        {
           for(int i = 0 ; i < columnArray[0].size() - 1; i++)
            {
               columnArray[columnArray.size() - 1].push_back(" ");
            }
        }
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
            if(_name.compare(this->columnArray[i][0]) == 0)
                return true;
        }
        return false;
    }
    int ColumnsNameSearching(vector<string> columnsName)  //For Selection Of Columns
    {
        int count = 0;
        for(int i = 0 ; i < columnsName.size() ; i++ )
        {
            for(int j = 0 ; j < columnArray.size() ; j++)
            {
                if(columnsName[i].compare(columnArray[j][0]) == 0)
                    count++;
            }
        }
        return count;
    }
    void SelectAllFromTable(string tableName)
    {
        system("cls");
        cout<<"CONTENT OF TABLE : "<<tableName<<endl;
        cout<<"======================="<<endl;
        int posx = 2;
        int posy = 2;
        for(int i = 0; i < columnArray.size() ; i++)
        {
            for(int j = 0;j < columnArray[i].size();j++)
            {
                gotoxy(posx,posy);
                cout<<this->columnArray[i][j];
                posy += 1;
            }
            posx += 10;
            posy = 2;
        }
        cout<<endl<<"-----------------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
        getch();
    }
    void SelectColumnFromTable(vector<string> columns)
    {
        system("cls");
        cout<<"TABLE CONTENT : "<<endl;
        cout<<"====================="<<endl;
        int posx = 2;
        int posy = 2;
        for(int i = 0; i < columnArray.size() ; i++)
        {
            for(int n = 0; n < columns.size(); n++)
            {
                if(columns[n] == columnArray[i][0])
                {
                    for(int j = 0;j < columnArray[i].size() ; j++)
                    {
                        gotoxy(posx,posy);
                        cout<<columnArray[i][j];
                        posy += 1;
                    }
                    posx += 20;
                    posy = 2;
                }
            }
        }
        cout<<endl<<"-----------------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
        getch();
    }
    void SelectColumnFromTableWhere(vector<string> columns,vector<string> conditionArray)
    {
        int index = getIndex(conditionArray[0],conditionArray[1]);
        system("cls");
        cout<<"TABLE CONTENT : "<<endl;
        cout<<"====================="<<endl;
        int posx = 2;
        int posy = 2;
        for(int i = 0; i < columnArray.size() ; i++)
        {
            for(int n = 0; n < columns.size(); n++)
            {
                if(columns[n] == columnArray[i][0])
                {
                    for(int j = 0;j < columnArray[i].size();j++)
                    {
                        if(index == j || j == 0)
                        {
                            gotoxy(posx,posy);
                            cout<<columnArray[i][j];
                            posy += 1;
                        }
                    }
                    posx += 20;
                    posy = 2;
                }
            }
        }
        cout<<endl<<"-----------------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
        getch();
    }
    void SelectRowFromTable(vector<string> conditionArray)
    {
        int index = getIndex(conditionArray[0],conditionArray[1]);
        system("cls");
        cout<<"RECORD : "<<endl;
        cout<<"=============="<<endl;
        int posx = 2;
        int posy = 2;
        for(int i = 0; i < (this->columnArray.size()) ; i++)
        {
            for(int j = 0 ; j < (this->columnArray[i].size()) ; j++)
            {
                if(index == j || j == 0)
                {
                    gotoxy(posx,posy);
                    cout<<this->columnArray[i][j];
                    posy += 1;
                }
            }
            posx += 10;
            posy = 2;
        }
        cout<<endl<<"-----------------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
        getch();
    }
    void InsertValues(vector<string> colsNamesArr , vector<string> valuesArray)
    {
        for(int j = 0 ; j < columnArray.size(); j++ )
        {
            bool flag = false;
            for(int i = 0; i < colsNamesArr.size() ; i++)
            {
                if(columnArray[j][0].compare(colsNamesArr[i]) == 0)
                {
                    columnArray[j].push_back(valuesArray[i]);
                    flag = true;
                }
            }
            if(flag == false)
                columnArray[j].push_back(" ");
        }
    }

    int getIndex(string colName,string colValue)
    {
        for(int i = 0; i < columnArray.size(); i++)
        {
            if(colName.compare(columnArray[i][0]) == 0)
            {
                for(int k = 1; k < columnArray[i].size(); k++)
                {
                    auto it = find(columnArray[i].begin(),columnArray[i].end(),colValue);
                    if(it != columnArray[i].cend())
                    {
                        return distance(columnArray[i].begin(),it);
                    }
                    else
                        return -1;
                }
            }
        }
    }
    void update(vector<string> colmsNames,vector<string> colmsValues,string whrColmName,string whrColmValue)
    {
        int searchedIndex = getIndex(whrColmName,whrColmValue);

        for(int i = 0; i < columnArray.size(); i++)
        {
            for (int j=0 ; i< colmsNames.size() ; j++)
            {
                if(columnArray[i][0].compare(colmsNames[j]) == 0)
                {
                    columnArray[i][searchedIndex].replace(0 , columnArray[i][searchedIndex].length() , colmsValues[i]);
                    break;
                }
            }
        }
    }
    void DeleteRow(string whrColmName,string whrColmValue)
    {
        int searchedIndex = getIndex(whrColmName,whrColmValue);

        for(int i = 0; i < columnArray.size(); i++)
        {
            columnArray[i].erase(columnArray[i].begin()+searchedIndex);
        }
    }
    void WriteData(ofstream & outFile) // Belongs To Saving Data To File
    {
        for(int i = 0 ; i < columnArray.size(); i++)
        {
            for(int j = 0 ; j < columnArray[i].size(); i++)
            {
                outFile << columnArray[i][j] << "\t";
            }
            outFile<<endl;
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
        cout<<"DATABASE LIST : "<<endl;
        cout<<"================="<<endl;
        for(int  i = 0;i < databaseList.size() ; i++)
            cout<<databaseList[i].name<<endl;
        cout<<"------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
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
            {
                databaseList.erase(it);
                break;
            }
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
        system("cls");
        cout<<"DATABASE LIST : "<<this->name<<endl;
        cout<<"================"<<endl;
        for(int i = 0; i < tableList.size(); i++)
            cout<<tableList[i].name<<endl;
        cout<<"------------------"<<endl;
        cout<<"PRESS ANY KEY TO RETURN..."<<endl;
        getch();
    }
    void DeleteTable(string _name)
    {
        auto it = this->tableList.begin();
        for(int i = 0 ;i < this->tableList.size(); i++)
        {
            if(_name.compare(it->name) == 0)
            {
                this->tableList.erase(it);
                break;
            }
            else
                it++;
        }
    }
};


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
                ErrorScreen("DUPLICATED DATABASE NAME IS NOT ALLOWED...");
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
                ErrorScreen("NO DATABASE EXISTS!!");
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
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("CREATE TABLE") != string::npos && WordsCounting(input) == 6)
        {
            string databaseName = originalInput.substr(15,input.find("CREATE TABLE") - 16);
            string tableName = originalInput.substr(input.find("CREATE TABLE") + 13);
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                if(!currentDatabase->TableNameSearching(tableName))
                {
                    Table table(tableName);
                    currentDatabase->tableList.push_back(table);
                }
                else
                {
                    ErrorScreen("DUPLICATED TABLE NAME IS NOT ALLOWED....");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("SHOW TABLES") != string::npos && WordsCounting(input) == 5)
        {

            string databaseName = originalInput.substr(15,input.find("SHOW TABLE") - 16);
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                currentDatabase->ShowTable();
                Heading();
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
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
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
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
                        ErrorScreen("DUPLICATED COLUMN NAME IS NOT ALLOWED...");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
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
                string cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                tableName.erase(cuttedText.find("DELETE COLUMN") - 1);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    string columnName = originalInput.substr(input.find("DELETE COLUMN") + 14);
                    if(currentTable->ColumnNameSearching(columnName))
                    {
                        currentTable->DeleteColumn(columnName);
                    }
                    else
                    {
                        ErrorScreen("NO COLUMN WITH THIS NAME EXISTS!!");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("INSERT INTO") != string::npos
                && input.find("VALUES") != string::npos && input.find("(") != string::npos && input.find(")") != string::npos
                 && ( WordsCounting(input) == 9 || WordsCounting(input) == 8 ) )
        {
          string databaseName = originalInput.substr(15);
            string cuttedText = originalInput.substr(input.find("INSERT INTO"));
            databaseName.erase(databaseName.find(cuttedText) - 1);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                string tableName = originalInput.substr(input.find("INSERT INTO") + 12);
                cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);

                string colName=originalInput.substr(input.find("(")+1);
                colName.erase(colName.find(")"));
                tableName.erase(cuttedText.find("(") - 1);

                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    string values = originalInput.substr(input.find("VALUES") + 7);
                    vector<string> valuesArray = StringSplit(values,",");
                    vector<string> colsNamesArray= StringSplit(colName,",");

                    if( currentTable->ColumnsNameSearching(colsNamesArray) == colsNamesArray.size() && colsNamesArray.size() >= 1)
                    {
                        currentTable->InsertValues(colsNamesArray,valuesArray);
                    }
                    else
                    {
                        ErrorScreen("ONE COLUMN OR MORE DOESN'T EXIST");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }

        }
        else if(input.find("USING DATABASE") == 0 && input.find("UPDATE TABLE") != string::npos
               && input.find("SET") != string::npos && input.find("WHERE") != string::npos )
        {
            string databaseName = originalInput.substr(15);
            string cuttedText = originalInput.substr(input.find("UPDATE TABLE"));
            databaseName.erase(databaseName.find(cuttedText) - 1);
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                string tableName = originalInput.substr(input.find("UPDATE TABLE") + 13);
                cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                tableName.erase(cuttedText.find("SET") - 1);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);

                    string columnNameToValues = originalInput.substr(input.find("SET") + 4);
                    cuttedText = columnNameToValues;
                    transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                    columnNameToValues.erase(cuttedText.find("WHERE") - 1);

                    vector<vector<string>> gettingResult = SplitingIntoColumnNamesAndValues(columnNameToValues);
                    vector<string> columnName = gettingResult[0];
                    vector<string> values = gettingResult[1];

                    string condition = originalInput.substr(input.find("WHERE") + 6);
                    vector<string> conditionArray = StringSplit(condition,"=");
                    if(currentTable->ColumnNameSearching(conditionArray[0]) && currentTable->ColumnsNameSearching(columnName) == columnName.size()
                       && currentTable->getIndex(conditionArray[0],conditionArray[1]) != -1)
                        currentTable->update(columnName,values,conditionArray[0],conditionArray[1]);
                    else
                    {
                        ErrorScreen("ONE COLUMN OR MORE DOESN'T EXIST OR CONDITION PARAMTERS!!");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("DELETE") != string::npos
                && input.find("FROM") != string::npos && input.find("WHERE") != string::npos && WordsCounting(input) == 8)
        {
            string databaseName = originalInput.substr(15);
            string cuttedText = originalInput.substr(input.find("DELETE"));
            databaseName.erase(databaseName.find(cuttedText) - 1);
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                string tableName = originalInput.substr(input.find("FROM") + 5);
                cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                tableName.erase(cuttedText.find("WHERE") - 1);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);

                    string condition = originalInput.substr(input.find("WHERE") + 6);
                    vector<string> conditionArray = StringSplit(condition,"=");
                    if(currentTable->ColumnNameSearching(conditionArray[0]) && currentTable->getIndex(conditionArray[0],conditionArray[1]) != -1)
                        currentTable->DeleteRow(conditionArray[0],conditionArray[1]);
                    else
                    {
                        ErrorScreen("ONE COLUMN OR MORE DOESN'T EXIST");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("SELECT * FROM") != string::npos && WordsCounting(input) == 7)
        {
            string databaseName = originalInput.substr(15,input.find("SELECT * FROM") - 16);
            string tableName = originalInput.substr(input.find("SELECT * FROM") + 14);
            Database *currentDatabase = Database::SearchedDatabase(databaseName);
            if(Database::DatabaseNameSearching(databaseName))
            {
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    currentTable->SelectAllFromTable(tableName);
                    Heading();
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("SELECT") != string::npos
               && input.find("FROM") != string::npos && WordsCounting(input) == 7)
        {
            string databaseName = originalInput.substr(15,input.find("SELECT") - 16);
            string tableName = originalInput.substr(input.find("FROM") + 5);

            string columnsName = originalInput.substr(input.find("SELECT") + 7);
            string cuttedText = columnsName ;
            transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
            columnsName.erase(cuttedText.find("FROM") - 1);

            vector <string> columnsArray = StringSplit(columnsName,",");
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    if(currentTable->ColumnsNameSearching(columnsArray) == columnsArray.size())
                    {
                        currentTable->SelectColumnFromTable(columnsArray);
                        Heading();
                    }
                    else
                    {
                        ErrorScreen("ONE COLUMN OR MORE DOESN'T EXIST");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("SELECT * FROM") != string::npos
                && input.find("WHERE") != string::npos && WordsCounting(input) ==  9 )
        {
            string databaseName = originalInput.substr(15);
            string cuttedText = originalInput.substr(input.find("SELECT * FROM"));
            databaseName.erase(databaseName.find(cuttedText) - 1);

            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);

                string tableName = originalInput.substr(input.find("SELECT * FROM") + 14);
                cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                tableName.erase(cuttedText.find("WHERE") - 1);

                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);

                    string condition = originalInput.substr(input.find("WHERE") + 6);
                    vector<string> conditionArray = StringSplit(condition,"=");
                    if(currentTable->ColumnNameSearching(conditionArray[0]))
                    {
                        currentTable->SelectRowFromTable(conditionArray);
                        Heading();
                    }
                    else
                    {
                        ErrorScreen("COLUMN DOESN'T EXIST!!");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("USING DATABASE") == 0 && input.find("SELECT") != string::npos
               && input.find("FROM") != string::npos && input.find("WHERE") != string::npos && WordsCounting(input) ==  9 )
        {
            string databaseName = originalInput.substr(15,input.find("SELECT") - 16);
            string columnsName = originalInput.substr(input.find("SELECT") + 7);
            string cuttedText = columnsName ;
            transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
            columnsName.erase(cuttedText.find("FROM") - 1);

            vector <string> columnsArray = StringSplit(columnsName,",");
            if(Database::DatabaseNameSearching(databaseName))
            {
                Database *currentDatabase = Database::SearchedDatabase(databaseName);

                string tableName = originalInput.substr(input.find("FROM") + 5);
                cuttedText = tableName;
                transform(cuttedText.begin(),cuttedText.end(),cuttedText.begin(),::toupper);
                tableName.erase(cuttedText.find("WHERE") - 1);
                string condition = originalInput.substr(input.find("WHERE") + 6);
                vector<string> conditionArray = StringSplit(condition,"=");
                if(currentDatabase->TableNameSearching(tableName))
                {
                    Table *currentTable = currentDatabase->SearchedTable(tableName);
                    if(currentTable->ColumnsNameSearching(columnsArray) == columnsArray.size())
                    {
                        if(currentTable->ColumnNameSearching(conditionArray[0]))
                        {
                            currentTable->SelectColumnFromTableWhere(columnsArray,conditionArray);
                            Heading();
                        }
                        else
                        {
                            ErrorScreen("COLUMN DOESN'T EXIST!!");
                            Heading();
                        }
                    }
                    else
                    {
                        ErrorScreen("ONE COLUMN OR MORE DOESN'T EXIST");
                        Heading();
                    }
                }
                else
                {
                    ErrorScreen("NO TABLE WITH THIS NAME EXISTS!!");
                    Heading();
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("SAVE DATABASE") == 0 && WordsCounting(input) == 3)
        {
            string databaseName = originalInput.substr(14);
            if(Database::DatabaseNameSearching(databaseName))
            {
                char _databaseName[databaseName.length() - 1];
                strcpy(_databaseName,databaseName.c_str());
                _mkdir(_databaseName);
                Database *currentDatabase = Database::SearchedDatabase(databaseName);
                if(!currentDatabase->tableList.empty())
                {
                    for(int i = 0 ; i < currentDatabase->tableList.size(); i++)
                    {
                        ofstream outFile;
                        string tableName = currentDatabase->tableList[i].name;
                        outFile.open("./"+databaseName+"/"+tableName+".txt");
                        Table *currentTable = currentDatabase->SearchedTable(tableName);
                        if(outFile.is_open())
                        {
                            for(int i = 0 ; i < (currentTable->columnArray.size()); i++)
                            {
                                for(int j = 0 ; j < (currentTable->columnArray[i].size()); j++)
                                {
                                    outFile << (currentTable->columnArray[i][j]) << "\t";
                                }
                                outFile<<"\n";
                            }
                        }
                        else
                        {
                            ErrorScreen("ERROR WHILE OPENING FILE...");
                        }
                        outFile.close();
                    }
                }
            }
            else
            {
                ErrorScreen("NO DATABASE WITH THIS NAME EXISTS!!");
                Heading();
            }
        }
        else if(input.find("LOAD DATABASE") == 0 && WordsCounting(input) == 3)
        {
            string databaseName = originalInput.substr(input.find("LOAD DATABASE")+ 14);
            Database loadedDatabase(databaseName);
            vector<string> filesName = ListingDirectery(databaseName);
            for(int i = 0 ; i < filesName.size() ; i++)
            {
                filesName[i].erase(filesName[i].find(".txt"));
                Table loadedTable(filesName[i]);
                ifstream inFile;
                inFile.open("./"+databaseName+"/"+filesName[i]+".txt");
                if(inFile.is_open())
                {
                    string line;
                    while(getline(inFile,line))
                    {
                        stringstream records(line);
                        string record;
                        vector<string> columnRecords;
                        while(records >> record)
                        {
                            columnRecords.push_back(record);
                        }
                        loadedTable.columnArray.push_back(columnRecords);
                    }
                    inFile.close();
                    loadedDatabase.tableList.push_back(loadedTable);
                }
                else
                {
                    ErrorScreen("COULDN'T OPEN FILE!!");
                    Heading();
                }
            }
            Database::databaseList.push_back(loadedDatabase);
        }
        else if(input.compare("HELP") == 0 && WordsCounting(input) == 1)
        {
            HelpMenu();
            Heading();
        }
        else if(input.compare("EXIT") == 0 && WordsCounting(input) == 1)
            flag = false;
        else
            cout<<"WRONG QUERY!!"<<endl;
    }
}
vector <Database> Database::databaseList;
int main()
{
    ConsoleCommand();
    return 0;
}
