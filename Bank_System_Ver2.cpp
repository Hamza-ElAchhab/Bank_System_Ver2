#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;


void MainMenu();
void Transactions_Menu();
void ManageUsers_Mune();
void Login();

const string ClientsFile = "FileOfDataClients.txt";
const string UsersFile = "UsersFile.txt";


int ReadNumber(string Message)
{
    int Number = 0;
    cout << Message;
    cin >> Number;

    while (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');
        cout << "Invalid Number , Enter a valid one : " << endl;
        cin >> Number;
    }
    return Number;
}

struct st_ClientInfo
{
    string AcountNumber;
    string PinCode;
    string Name;
    string Phone;
    float Balance;
    bool MarkToDelete = false;
};
vector<st_ClientInfo> LoadData_FromFile_ToVector();

enum en_MainMenuOptions
{
    E_show = 1, E_add = 2, E_delete = 3, E_update = 4, E_find = 5, E_Transactions = 6, E_ManageUsers = 7, E_LogOut = 8
};

// users
struct st_UserInfo
{
    string UserName;
    string PassWord;
    int Permission;
    bool toDelete = false;
};
st_UserInfo CurrentUserInfo;
enum en_Permissions
{
    e_ClientsList = 1, e_AddClient = 2, e_DeleteClient = 4, e_UpDateClient = 8, e_FindClient = 16, e_Transactions = 32, e_ManageUsers = 64, e_FullAccess = -1
};
bool isHasPermission(en_Permissions PermissioNumber);
void AccessDeined();
////

string Read_AcoutNumber()
{
    string AcountNum = "";
    cout << "Enter An Acount Number : ";
    getline(cin >> ws, AcountNum);

    return AcountNum;
}

bool isClientExists(string AcountNum)
{
    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();

    for (st_ClientInfo& s : vData)
    {
        if (s.AcountNumber == AcountNum)
        {
            return true;
        }
    }
    return false;
}

st_ClientInfo Read_Struct_ClinetInfo()
{
    st_ClientInfo sInfo1;

    cout << "\n__________ Read Info : __________\n\n";
    cout << "Enter Acount Number : ";
    getline(cin >> ws, sInfo1.AcountNumber);

    while (isClientExists(sInfo1.AcountNumber))
    {
        cout << "\nClient Exists, Enter New Acount Number : ";
        getline(cin >> ws, sInfo1.AcountNumber);
    }

    cout << "Enter Pin Code      : ";
    getline(cin, sInfo1.PinCode);

    cout << "Enter Your Name     : ";
    getline(cin, sInfo1.Name);

    cout << "Enter Your Phone    : ";
    getline(cin, sInfo1.Phone);

    sInfo1.Balance = ReadNumber("Enter Your Balance  : ");
    cout << "\n---------------------------------\n";

    return sInfo1;
}

string Join_Struct_Info(st_ClientInfo info, string sep = "#//#")
{
    string str = "";

    str += info.AcountNumber + sep;
    str += info.PinCode + sep;
    str += info.Name + sep;
    str += info.Phone + sep;
    str += to_string(info.Balance);

    return str;
}

void Save_OneRecord_ToFile(string FileName, string record)
{
    fstream file;
    file.open(FileName, ios::out | ios::app);

    if (file.is_open())
    {
        file << record << endl;

        file.close();
    }
}

void AddClient()
{
    if (!(isHasPermission(en_Permissions::e_AddClient)))
    {
        AccessDeined();
        return;
    }

    char a = 'n';
    do
    {
        system("cls");
        cout << "\n\tADDING NEW CLIENT : \n";
        cout << "____________________________________\n";

        Save_OneRecord_ToFile(ClientsFile, Join_Struct_Info(Read_Struct_ClinetInfo()));

        cout << "\nAdded Succesfully, Do You Want To Add More [Y/N] : ";
        cin >> a;

    } while (toupper(a) == 'Y');
}

void ScreenWait()
{
    cout << endl << "\n\nPress Any Key To Go Back...";
    system("pause>0");
}

vector<string> ft_Split(string str, string delm = "#//#")
{
    vector<string> vStrings;
    string Word = "";
    int Pos = 0;

    while ((Pos = str.find(delm)) != str.npos)
    {
        Word = str.substr(0, Pos);
        if (Word != "")
        {
            vStrings.push_back(Word);
        }

        str = str.erase(0, (Pos + delm.length()));
    }

    if (str != "")
    {
        vStrings.push_back(str);
    }

    return vStrings;
}

st_ClientInfo Fill_StructInfo(string line)
{
    vector<string> vStrings = ft_Split(line);
    st_ClientInfo info;

    info.AcountNumber = vStrings[0];
    info.PinCode = vStrings[1];
    info.Name = vStrings[2];
    info.Phone = vStrings[3];
    info.Balance = stoi(vStrings[4]);

    return info;
}

vector<st_ClientInfo> LoadData_FromFile_ToVector()
{
    vector<st_ClientInfo> vData;
    st_ClientInfo info;
    fstream file;
    string Line = "";

    file.open(ClientsFile, ios::in);
    if (file.is_open())
    {

        while (getline(file, Line))
        {
            info = Fill_StructInfo(Line);
            vData.push_back(info);
        }

        file.close();
    }

    return vData;
}

void Print_Struct_Info(st_ClientInfo info)
{
    cout << "\t\t|    " << setw(12) << info.AcountNumber << "|" << setw(12) << info.PinCode << "|" << setw(20) << info.Name << "|" <<
        setw(15) << info.Phone << "|" << setw(12) << info.Balance << " |" << endl;
}

void ShowClients()
{
    if (!(isHasPermission(en_Permissions::e_ClientsList)))
    {
        AccessDeined();
        return;
    }

    system("cls");
    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();
    cout << "\n\n\t\t ________________________________________________________________________________\n";
    cout << "\t\t\t\t\t\t  " << vData.size() << "   Client(s).\n";
    cout << "\t\t|________________________________________________________________________________|\n";
    cout << "\t\t|  Acount Number:|   Pin Code:|       Name :       |   Phone :     |  Balance :  |\n";
    cout << "\t\t|________________________________________________________________________________|\n";

    for (st_ClientInfo& i : vData)
    {
        Print_Struct_Info(i);
    }
    cout << "\t\t|________________________________________________________________________________|\n";
}

bool isClientExists_get_it(string AcountNum, st_ClientInfo& info)
{
    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();

    for (st_ClientInfo& s : vData)
    {
        if (s.AcountNumber == AcountNum)
        {
            info = s;
            return true;
        }
    }
    return false;
}

void PrintCardOfClinet(st_ClientInfo info)
{
    cout << "\n\nClinet Info : ";
    cout << "\n_________________________\n";
    cout << "Acount Num : " << info.AcountNumber << endl;
    cout << "Pin Code   : " << info.PinCode << endl;
    cout << "Name       : " << info.Name << endl;
    cout << "Phone      : " << info.Phone << endl;
    cout << "Balance    : " << info.Balance << endl;
    cout << "_________________________\n";
}

void Sava_VectorOfStruct_ToFile(vector<st_ClientInfo> vData)
{
    fstream file;

    file.open(ClientsFile, ios::out);

    if (file.is_open())
    {
        string line = "";

        for (st_ClientInfo& s : vData)
        {
            if (s.MarkToDelete == false)
            {
                line = Join_Struct_Info(s);
                file << line << endl;
            }
        }

        file.close();
    }
}

void Mark_Client_For_Delete(vector<st_ClientInfo>& vData, string AcountNum)
{
    for (st_ClientInfo& s : vData)
    {
        if (s.AcountNumber == AcountNum)
        {
            s.MarkToDelete = true;
            return;
        }
    }
}

void DeleteClient()
{
    if (!(isHasPermission(en_Permissions::e_DeleteClient)))
    {
        AccessDeined();
        return;
    }

    system("cls");
    cout << "\nDeleting a Client :\n";
    cout << "__________________________________\n";

    string AcountNum = Read_AcoutNumber();
    st_ClientInfo info;
    char ans = 'n';

    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();

    if (isClientExists_get_it(AcountNum, info))
    {
        PrintCardOfClinet(info);
        cout << endl << "Are You Sure [Y/N] : ";
        cin >> ans;

        if (toupper(ans) == 'Y')
        {
            Mark_Client_For_Delete(vData, AcountNum);
            Sava_VectorOfStruct_ToFile(vData);
            cout << endl << "Deleted\n";
        }
    }
    else
    {
        cout << "\nAcount Number not found\n";
    }
}

st_ClientInfo Read_New_Client_Info_update(string AN)
{
    st_ClientInfo info;
    info.AcountNumber = AN;

    cout << "\n\n__________ Read New Info : __________\n";
    cout << "Enter Pin Code      : ";
    getline(cin >> ws, info.PinCode);

    cout << "Enter Your Name     : ";
    getline(cin, info.Name);

    cout << "Enter Your Phone    : ";
    getline(cin, info.Phone);

    cout << "Enter Your Balance  : ";
    cin >> info.Balance;
    cout << "\n-------------------------------------\n";

    return info;

}

void UpDateClient()
{
    if (!(isHasPermission(en_Permissions::e_UpDateClient)))
    {
        AccessDeined();
        return;
    }

    system("cls");
    cout << "\n\tUPDATING CLIENT : \n";
    cout << "--------------------------------\n\n";

    string AcountNumber = Read_AcoutNumber();
    st_ClientInfo info;
    char ans = 'v';

    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();

    if (isClientExists_get_it(AcountNumber, info))
    {
        PrintCardOfClinet(info);

        cout << endl << "Are You Sure You Want To UpDate it [Y/N] : ";
        cin >> ans;

        if (toupper(ans) == 'Y')
        {
            for (st_ClientInfo& i : vData)
            {
                if (AcountNumber == i.AcountNumber)
                {
                    i = Read_New_Client_Info_update(i.AcountNumber);
                    break;
                }
            }

            Sava_VectorOfStruct_ToFile(vData);
            cout << "\nUpDated Succesuflly\n";
        }
    }
    else
    {
        cout << endl << "Client Does Noot Exists\n";
    }
}

void FindClient()
{
    if (!(isHasPermission(en_Permissions::e_FindClient)))
    {
        AccessDeined();
        return;
    }

    system("cls");
    cout << "\n\tFinding Clients : \n";
    cout << "__________________________________\n\n";

    string AcoutN = Read_AcoutNumber();
    st_ClientInfo info;

    if (isClientExists_get_it(AcoutN, info))
    {
        PrintCardOfClinet(info);
    }
    else
    {
        cout << endl << endl << "Client Does not Exists.\n";
    }
}

void Exit()
{
    system("cls");
    cout << "\n_____________________________\n\n";
    cout << "\tProgram Ended\n";
    cout << "\n-----------------------------\n";
}

//......
enum en_Transaction_Menu
{
    E_Deposite = 1, E_WithDraw = 2, E_TotalBalances = 3, E_BackMainMenu = 4
};

en_Transaction_Menu Read_TransactionsMenu_Option()
{
    int num = ReadNumber("\n\tEnter Your Option Here [1 -> 4] : ");
    return (en_Transaction_Menu)num;
}

void Increase_And_Decrease_AmountBalance(string Acount, int Amount)
{
    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();
    int NewBalance = 0;

    for (st_ClientInfo& s : vData)
    {
        if (s.AcountNumber == Acount)
        {
            s.Balance += Amount;
            NewBalance = s.Balance;
            break;
        }
    }
    cout << endl << "New Balance is : " << NewBalance << endl;
    Sava_VectorOfStruct_ToFile(vData);
}

void Deposite()
{
    system("cls");
    cout << "\n---------------------------------\n";
    cout << "\t Deposite Screen :\n";
    cout << "---------------------------------\n\n";

    string AcountNum = Read_AcoutNumber();
    st_ClientInfo info;

    while (!isClientExists_get_it(AcountNum, info))
    {
        cout << "\nClient With Acount Number " << AcountNum << " Not Found, Enter Again : ";
        AcountNum = Read_AcoutNumber();
    }

    PrintCardOfClinet(info);
    int Amount = ReadNumber("\nEnter Deposite Amount : ");

    char ans = 'n';
    cout << endl << "Are You Sure [Y/N] : ";
    cin >> ans;

    if (toupper(ans) == 'Y')
    {
        Increase_And_Decrease_AmountBalance(AcountNum, Amount);
        cout << "\nAdded Successfull\n";
    }
}

void WithDraw()
{
    system("cls");
    cout << "\n---------------------------------\n";
    cout << "\tWith Draw Screen :\n";
    cout << "---------------------------------\n\n";

    string AcountNum = Read_AcoutNumber();
    st_ClientInfo info;

    while (!isClientExists_get_it(AcountNum, info))
    {
        cout << "\nClient With Acount : " << AcountNum << " Not Found, : ";
        AcountNum = Read_AcoutNumber();
    }

    PrintCardOfClinet(info);
    int Amount = ReadNumber("\nEnter With Draw Amount : ");

    while (Amount > info.Balance)
    {
        cout << endl << "Balance is Less than Amount, Enter Again : ";
        Amount = ReadNumber("");
    }

    char ans = 'n';
    cout << endl << "Are You Sure [Y/N] : ";
    cin >> ans;

    if (toupper(ans) == 'Y')
    {
        Increase_And_Decrease_AmountBalance(AcountNum, (Amount * (-1)));
        cout << "\nWith Draw Successfull\n";
    }
}

void ShowTotalBalances()
{
    system("cls");
    vector<st_ClientInfo> vData = LoadData_FromFile_ToVector();
    cout << "\n\n\t\t ________________________________________________________________________________\n";
    cout << "\t\t\t\t\t\t" << vData.size() << "   Client(s).\n";
    cout << "\t\t|________________________________________________________________________________|\n";
    cout << "\t\t|  Acount Number:|   Pin Code:|       Name :       |   Phone :     |  Balance :  |\n";
    cout << "\t\t|________________________________________________________________________________|\n";

    for (st_ClientInfo& i : vData)
    {
        Print_Struct_Info(i);
    }
    cout << "\t\t|________________________________________________________________________________|\n";


    int TotalBalances = 0;
    for (st_ClientInfo& s : vData)
    {
        TotalBalances += s.Balance;
    }

    cout << "\t\t\t\t\t   TOTAL BALANCES : " << TotalBalances << endl;
    cout << "\t\t|________________________________________________________________________________|\n";
}

void Working_TransactionsMenu(en_Transaction_Menu Option)
{
    switch (Option)
    {
    case en_Transaction_Menu::E_BackMainMenu:
        MainMenu();
        break;


    case en_Transaction_Menu::E_Deposite:
        Deposite();
        ScreenWait();
        Transactions_Menu();
        break;


    case en_Transaction_Menu::E_WithDraw:
        WithDraw();
        ScreenWait();
        Transactions_Menu();
        break;


    case en_Transaction_Menu::E_TotalBalances:
        ShowTotalBalances();
        ScreenWait();
        Transactions_Menu();
        break;


    default:
        cout << "\nWrong Choice, maaaaan!!!\n";
        break;
    }
}

void Transactions_Menu()
{
    if (!(isHasPermission(en_Permissions::e_ManageUsers)))
    {
        AccessDeined();
        MainMenu();
    }

    system("cls");
    cout << "\t\t\tVersion 2:\n\tby HAMZA EL ACHHAB learning from PROGRAMMING ADVICES \n\n";
    cout << "\t=========================================\n";
    cout << "\t\t TRANSACTIONS MAIN MENU : \n";
    cout << "\t=========================================\n";
    cout << "\t\t   [1] Deposite.\n";
    cout << "\t\t   [2] With Draw.\n";
    cout << "\t\t   [3] Total Balances.\n";
    cout << "\t\t   [4] Go Back To Main Menu.\n";
    cout << "\t=========================================\n";

    Working_TransactionsMenu(Read_TransactionsMenu_Option());
}
//......

//......

int Read_Permissions_Of_User()
{
    int Total = 0;
    char ans = ' n';

    cout << endl << "Reading Permissions : \n";
    cout << "___________________________________________\n";
    cout << "\NUse [Y/N] :";
    cout << endl << "Do you Want To Give him Access To Show Clients : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_ClientsList;
    }

    cout << endl << "Do you Want To Give him Access To Adding Clients : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_AddClient;
    }

    cout << endl << "Do you Want To Give him Access To Deleting Clients : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_DeleteClient;
    }

    cout << endl << "Do you Want To Give him Access To UpDating Clients : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_UpDateClient;
    }

    cout << endl << "Do you Want To Give him Access To Find Clients : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_FindClient;
    }

    cout << endl << "Do you Want To Give him Access To Transactions Menu : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_Transactions;
    }

    cout << endl << "Do you Want To Give him Access To Manage Users :";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        Total += en_Permissions::e_ManageUsers;
    }

    return Total;
}

void AccessDeined()
{
    system("cls");
    cout << "\n\t----------------------------\n";
    cout << "\t You Don't Have Permission\n";
    cout << "\t----------------------------\n";
    system("pause>0");
}

bool isHasPermission(en_Permissions PermissioNumber)
{
    if (CurrentUserInfo.Permission == en_Permissions::e_FullAccess)
    {
        return true;
    }

    if ((PermissioNumber & CurrentUserInfo.Permission) == PermissioNumber)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool FindUserBy_UserName(string UserName, st_UserInfo& info);
void PrintCardOf_User(st_UserInfo info);

enum en_ManageUsers_Option
{
    E_showUser = 1, E_addUser = 2, E_deleteUser = 3, E_updateUser = 4, E_findUser = 5, E_MainMenu = 6
};

en_ManageUsers_Option ReadManageUser_Option()
{
    int num = ReadNumber("\n\tEnter You Choice [1 -> 6] : ");
    return (en_ManageUsers_Option)num;
}

st_UserInfo Read_Struct_UserInfo()
{
    st_UserInfo info;
    cout << "\n\n__________ Read Info : __________\n\n";
    cout << "Enter User Name : ";
    getline(cin >> ws, info.UserName);

    st_UserInfo infoo;
    while (FindUserBy_UserName(info.UserName, infoo))
    {
        cout << "User Aleady Exists :\n";
        PrintCardOf_User(infoo);

        cout << "\n\nEnter User Name : ";
        getline(cin, info.UserName);
    }

    cout << "Enter PassWord  : ";
    getline(cin, info.PassWord);

    char ans = 'n';
    cout << "\nDo You Want To Give Him Full Access [Y/N] : ";
    cin >> ans;

    if (toupper(ans) == 'Y')
    {
        info.Permission = en_Permissions::e_FullAccess;
    }
    else
    {
        info.Permission = Read_Permissions_Of_User();
    }
    cout << "\n---------------------------------\n";

    return info;
}

string Join_UserInfo_toString(st_UserInfo info, string sep = "#//#")
{
    string str = "";
    str += info.UserName + sep;
    str += info.PassWord + sep;
    str += to_string(info.Permission);

    return str;
}

void Save_Record_UserInfoString_ToFile(string Record)
{
    fstream file;
    file.open(UsersFile, ios::out | ios::app);

    if (file.is_open())
    {
        file << Record << endl;

        file.close();
    }

    cout << endl << "Saved Successfully.\n";
}

void AddUser()
{
    char ans = 'n';
    do
    {
        system("cls");
        cout << "_____________________________________\n";
        cout << " Adding New User Screen :\n";
        cout << "_____________________________________\n";

        Save_Record_UserInfoString_ToFile(Join_UserInfo_toString(Read_Struct_UserInfo()));

        cout << endl << "Do You Want To Add More [Y/N] : ";
        cin >> ans;

    } while (toupper(ans) == 'Y');
}

string ReadUserName()
{
    string User;
    cout << endl << "Enter User Name : ";
    cin >> User;
    return User;
}

st_UserInfo Fill_Struct_UserInfo_UsingRecordLine(string Record)
{
    st_UserInfo info;
    vector<string> vStrings = ft_Split(Record);

    info.UserName = vStrings[0];
    info.PassWord = vStrings[1];
    info.Permission = stoi(vStrings[2]);

    return info;
}

vector<st_UserInfo> Load_Data_Of_Users()
{
    vector<st_UserInfo> vData;
    st_UserInfo Sinfo;
    string Line;

    fstream File;
    File.open(UsersFile, ios::in);

    if (File.is_open())
    {
        while (getline(File, Line))
        {
            Sinfo = Fill_Struct_UserInfo_UsingRecordLine(Line);
            vData.push_back(Sinfo);
        }

        File.close();
    }

    return vData;
}

void PrintCardOf_User(st_UserInfo info)
{
    cout << "\n\nUser Info : ";
    cout << "\n_________________________\n";
    cout << "User Name  : " << info.UserName << endl;
    cout << "Pass Word  : " << info.PassWord << endl;
    cout << "Permissions: " << info.Permission << endl;
    cout << "_________________________\n";
}

bool FindUserBy_UserName(string UserName, st_UserInfo &info)
{
    vector<st_UserInfo> vData = Load_Data_Of_Users();

    for (st_UserInfo& s : vData)
    {
        if (s.UserName == UserName)
        {
            info = s;
            return true;
        }
    }
    return false;
}

void Mark_User_For_Delete(vector<st_UserInfo> &vData, string UserName)
{
    for (st_UserInfo& s : vData)
    {
        if (s.UserName == UserName)
        {
            s.toDelete = true;
            return;
        }
    }
}

void SaveData_FromVector_ToFile(vector<st_UserInfo> vData)
{
    fstream file;
    file.open(UsersFile, ios::out);

    if (file.is_open())
    {
        string Line;
        
        for (st_UserInfo& s : vData)
        {
            if (s.toDelete == false)
            {
                Line = Join_UserInfo_toString(s);
                file << Line << endl;
            }
        }
        file.close();
    }
}

void DeleteUser()
{
    system("cls");
    cout << "_____________________________________\n";
    cout << "\t    Delete User Screen :\n";
    cout << "_____________________________________\n";

    vector<st_UserInfo> vData = Load_Data_Of_Users();
    string UserName = ReadUserName();
    st_UserInfo info;
    char ans = 'n';

    if (FindUserBy_UserName(UserName, info))
    {
        PrintCardOf_User(info);
        cout << endl << "Are You Sure [Y/N] : ";
        cin >> ans;
        
        if (toupper(ans) == 'Y')
        {
            if (info.UserName == "Admin")
            {
                cout << endl << "\tYou Can't Delete This User.\n";
            }
            else
            {
                Mark_User_For_Delete(vData, UserName);
                SaveData_FromVector_ToFile(vData);
                cout << endl << "Deleted Successfully.\n";
            }
        }
    }
    else
    {
        cout << endl << "\nUser Name With " << UserName << " Does not Exists.\n";
    }

}

st_UserInfo Read_UpdateInfo_OfUser(string UserName)
{
    st_UserInfo INFO;
    INFO.UserName = UserName;

    cout << endl << "\n-------------------------\n";
    cout << "Enter PassWord    : ";
    getline(cin >>ws, INFO.PassWord);

    char ans = 'n';
    cout << "\nGive Him Full Access [Y/N] : ";
    cin >> ans;
    if (toupper(ans) == 'Y')
    {
        INFO.Permission = en_Permissions::e_FullAccess;
    }
    else
    {
        INFO.Permission = Read_Permissions_Of_User();
        cout << "\n-------------------------\n";
    }
    return INFO;
}

void UpDateUser()
{
    system("cls");
    cout << "_____________________________________\n";
    cout << "\t   UpDate User Screen :\n";
    cout << "_____________________________________\n";

    vector<st_UserInfo> vData = Load_Data_Of_Users();
    string UserName = ReadUserName();
    st_UserInfo info;
    char ans = 'n';

    if (FindUserBy_UserName(UserName, info))
    {
        PrintCardOf_User(info);
        cout << endl << "Are You Sure [Y/N] : ";
        cin >> ans;

        for (st_UserInfo& s : vData)
        {
            if (s.UserName == UserName)
            {
                s = Read_UpdateInfo_OfUser(s.UserName);
                break;
            }
        }

        SaveData_FromVector_ToFile(vData);
        cout << endl << "UpDated Succesfully.\n";
    }

    else
    {
        cout << endl << "User With {" << UserName << "} Does Not Exists\n";
    }
}

void FindUser()
{
    system("cls");
    cout << "_____________________________________\n";
    cout << "\t    Find User Screen :\n";
    cout << "_____________________________________\n";

    string UserName = ReadUserName();
    st_UserInfo info;

    if (FindUserBy_UserName(UserName, info))
    {
        PrintCardOf_User(info);
    }
    else
    {
        cout <<endl << "User With {" << UserName << "} Does Not Exists\n";
    }
}

void Print_User_RecordInfo(st_UserInfo info)
{
    cout << "\t" << setw(20) << info.UserName << "      |" << setw(25) << info.PassWord << "   |" << setw(20) << info.Permission << "   |\n";
}

void UsersList()
{
    system("cls");
    vector<st_UserInfo> vData = Load_Data_Of_Users();

    cout << "\n\t\t\t\t\t{ " << vData.size() << " } User(s).\n\n";
    cout << "\t_______________________________________________________________________________________\n";
    cout << "\t\tUser Name :       |           Pass Word :      |        Permissions :  |\n";
    cout << "\t_______________________________________________________________________________________\n";

    for (st_UserInfo& s : vData)
    {
        Print_User_RecordInfo(s);
    }
    cout << "\n\t_______________________________________________________________________________________\n";
}

void Workin_ManageUsers(en_ManageUsers_Option option)
{
    switch (option)
    {
    case en_ManageUsers_Option::E_addUser:
        AddUser();
        ScreenWait();
        ManageUsers_Mune();
        break;


    case en_ManageUsers_Option::E_MainMenu:
        MainMenu();
        break;


    case en_ManageUsers_Option::E_deleteUser:
        DeleteUser();
        ScreenWait();
        ManageUsers_Mune();
        break;
    

    case en_ManageUsers_Option::E_updateUser:
        UpDateUser();
        ScreenWait();
        ManageUsers_Mune();
        break;


    case en_ManageUsers_Option::E_findUser:
        FindUser();
        ScreenWait();
        ManageUsers_Mune();
        break;


    case en_ManageUsers_Option::E_showUser:
        UsersList();
        ScreenWait();
        ManageUsers_Mune();
        break;


    default :
        cout << endl << "Wrong Choice Maan !\n";
    }
}

void ManageUsers_Mune()
{
    if (!(isHasPermission(en_Permissions::e_ManageUsers)))
    {
        AccessDeined();
        MainMenu();
    }

    system("cls");
    cout << "\t\t\tVersion 2:\n\tby HAMZA EL ACHHAB learning from PROGRAMMING ADVICES \n\n";
    cout << "\t=========================================\n";
    cout << "\t\t   MANAGE USERS MENU : \n";
    cout << "\t=========================================\n";
    cout << "\t\t   [1] Show Users.\n";
    cout << "\t\t   [2] Add New User.\n";
    cout << "\t\t   [3] Delete User.\n";
    cout << "\t\t   [4] UpDate User.\n";
    cout << "\t\t   [5] Find User.\n";
    cout << "\t\t   [6] Back To Main Menu.\n";
    cout << "\t=========================================\n";
    Workin_ManageUsers(ReadManageUser_Option());
}
//......

en_MainMenuOptions Read_Option_MainMenu()
{
    int num = ReadNumber("\n\tEnter Your Option Here [1 -> 8] : ");
    return (en_MainMenuOptions)num;
}

void Wording_MainMenu(en_MainMenuOptions MenuOption)
{
    switch (MenuOption)
    {
    case en_MainMenuOptions::E_add:
        AddClient();
        ScreenWait();
        MainMenu();
        break;


    case en_MainMenuOptions::E_delete:
        DeleteClient();
        ScreenWait();
        MainMenu();
        break;


    case en_MainMenuOptions::E_show:
        ShowClients();
        ScreenWait();
        MainMenu();
        break;


    case en_MainMenuOptions::E_update:
        UpDateClient();
        ScreenWait();
        MainMenu();
        break;


    case en_MainMenuOptions::E_find:
        FindClient();
        ScreenWait();
        MainMenu();
        break;

    case en_MainMenuOptions::E_Transactions:
        Transactions_Menu();
        break;


    case en_MainMenuOptions::E_LogOut:
        Login();
        break;


    case en_MainMenuOptions::E_ManageUsers:
        ManageUsers_Mune();
        break;


    default:
        cout << "\nWrong Choice, maaaaan!!!\n";
        break;
    }
}

void MainMenu()
{
    system("cls");
    cout << "\t\t\tVersion 2:\n\tby HAMZA EL ACHHAB learning from PROGRAMMING ADVICES \n\n";
    cout << "\t=========================================\n";
    cout << "\t\t     MAIN MENU SCREEN : \n";
    cout << "\t=========================================\n";
    cout << "\t\t   [1] Show Clients.\n";
    cout << "\t\t   [2] Add New Client.\n";
    cout << "\t\t   [3] Delete Client.\n";
    cout << "\t\t   [4] UpDate Client.\n";
    cout << "\t\t   [5] Find Client.\n";
    cout << "\t\t   [6] Transactions Menu.\n";
    cout << "\t\t   [7] Manage Users.\n";
    cout << "\t\t   [8] LogOut.\n";
    cout << "\t=========================================\n";

    Wording_MainMenu(Read_Option_MainMenu());
}

bool FindUserBy_UserNameAndPassWord(string UserName, string PassWord)
{
    vector<st_UserInfo> vData = Load_Data_Of_Users();

    for (st_UserInfo& s : vData)
    {
        if (s.UserName == UserName && s.PassWord == PassWord)
        {
            CurrentUserInfo = s;
            return true;
        }
    }
    return false;
}


void Login()
{
    system("cls");
    cout << "____________________________\n";
    cout << "\tLogIn Screen : \n";
    cout << "____________________________\n";

    string UserName, PassWord;
    cout << endl << "Enter User Name  : ";
    getline(cin >> ws, UserName);
    cout << "Enter Pass Word  : ";
    getline(cin, PassWord);

    if (FindUserBy_UserNameAndPassWord(UserName, PassWord))
    {
        MainMenu();
    }
    else
    {
        cout << "\nInValid, Error\n";
        ScreenWait();
        Login();
    }
}


int main()
{
 
    
     Login();



   
   return 0;
}