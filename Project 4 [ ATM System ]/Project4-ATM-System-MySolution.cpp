

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowATMMainMenue();
void ShowQuickWithdrawScreen();
void Login();


struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;
vector<sClient> vClients;

enum enATMMainMenueOptions {
    eQuickWithdraw = 1, eNormalWithdraw = 2,
    eDeposit = 3, eCheckBalance = 4,
    eChangePinCode = 5, eLogout = 6
};

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

    return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{



    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }


    }
    return false;

}
//Change PinCode

void PerformChangePinCode(string NewPinCode)
{
    char Answer = 'n';
    cout << "\nAre you sure you want to change your PinCode? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        // 1. تحديث العميل الحالي
        CurrentClient.PinCode = NewPinCode;

        // 2. تحديث الـ vector (لأنه سيُحفظ في الملف)
        for (sClient& C : vClients)
        {
            if (C.AccountNumber == CurrentClient.AccountNumber)
            {
                C.PinCode = NewPinCode;
                break;
            }
        }

        // 3. الحفظ في الملف
        SaveCleintsDataToFile(ClientsFileName, vClients);
        cout << "\nPinCode Changed Successfully.";
    }
}


void ShowChangePinCodeScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tChange PinCode Screen\n";
    cout << "===========================================\n";

    string CurrentPinCode;
    cout << "Enter Your Current PinCode: ";
    cin >> CurrentPinCode;

    // التأكد من أن الرقم المدخل يطابق رقم العميل المسجل دخوله حالياً
    if (CurrentPinCode != CurrentClient.PinCode)
    {
        cout << "\nWrong PinCode! Access Denied.";
        return; // الخروج من الدالة فوراً ومنع التغيير
    }

    // إذا كان الرقم صحيحاً، ننتقل لطلب الرقم الجديد
    string NewPinCode;
    cout << "\nEnter New PinCode: ";
    cin >> NewPinCode;

    string ConfirmPinCode;
    cout << "Confirm New PinCode: ";
    cin >> ConfirmPinCode;

    if (NewPinCode == ConfirmPinCode)
    {
        PerformChangePinCode(NewPinCode);
    }
    else
    {
        cout << "\nPinCodes do not match. Operation cancelled.";
    }
}




// Deposit Screen

void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";

    double Amount = 0;
    cout << "\nEnter a positive Deposit Amount? ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "Invalid amount, Enter a positive Deposit Amount? ";
        cin >> Amount;
    }
    if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount, vClients))
    {
        // تحديث الكائن الحالي ليعكس الرصيد الجديد في الشاشة فوراً
        CurrentClient.AccountBalance += Amount;
    }
}


//Normal With draw

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";

    int Amount = 0;
    cout << "Enter an amount multiple of 5? ";
    cin >> Amount;
    while (Amount % 5 != 0 || Amount <= 0)
    {
        cout << "Invalid amount. Enter an amount multiple of 5? ";
        cin >> Amount;
    }

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }
    if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients))
    {
        // تحديث الكائن الحالي ليعكس الرصيد الجديد في الشاشة فوراً
        CurrentClient.AccountBalance -= Amount;
    }
}

//Check Balance

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;
}

//Quick With draw

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "Choose what do you want to do? [1 to 9]? ";
        cin >> Choice;
    }
    return Choice;
}

short getQuickWithdrawAmount(short Option) {
    short Amnts[] = { 0, 20, 50, 100, 200, 400, 600, 800, 1000 };
    return Amnts[Option];
}

void PerfromQuickWithdrawOption(short Option)
{
    if (Option == 9) return;


    short Amount = getQuickWithdrawAmount(Option);

    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }


    if (DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients))
    {
        // تحديث الكائن الحالي ليعكس الرصيد الجديد في الشاشة فوراً
        CurrentClient.AccountBalance -= Amount;
    }
}


void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}


void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowATMMainMenue();
}


short ReadATMMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromATMMainMenueOption(enATMMainMenueOptions ATMMainMenueOption)
{
    switch (ATMMainMenueOption)
    {
    case enATMMainMenueOptions::eQuickWithdraw:
        ShowQuickWithdrawScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenueOptions::eNormalWithdraw:

        ShowNormalWithdrawScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenueOptions::eDeposit:

        ShowDepositScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenueOptions::eCheckBalance:
        ShowCheckBalanceScreen();
        GoBackToATMMainMenue();
        break;
    case enATMMainMenueOptions::eChangePinCode:
        ShowChangePinCodeScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenueOptions::eLogout:

        Login();
        break;
    }
}


void ShowATMMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] ChangePinCode.\n";
    cout << "\t[6] Logout.\n";
    cout << "===========================================\n";

    PerfromATMMainMenueOption((enATMMainMenueOptions)ReadATMMainMenueOption());
}


bool  LoadClientInfo(string AccountNumber, string PinCode)
{

    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;

}

void Login()
{
    bool LoginFaild = false;
    short Trials = 3; // تحديد عدد المحاولات
    string AccountNumber, PinCode;

    vClients = LoadCleintsDataFromFile(ClientsFileName);

    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Account Number/PinCode!\n";
            cout << "You have " << Trials << " trial(s) left.\n\n";
        }

        // إذا انتهت المحاولات نخرج من البرنامج
        if (Trials == 0)
        {
            cout << "Your account is locked. Contact the bank.\n";
            system("pause>0");
            exit(0);
        }

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        if (LoadClientInfo(AccountNumber, PinCode))
        {
            // في حال النجاح ننتقل للقائمة وننهي الدالة
            ShowATMMainMenue();
            return;
        }
        else
        {
            LoginFaild = true;
            Trials--; // إنقاص المحاولات
        }

    } while (true); // الحلقة تستمر حتى النجاح أو استنفاذ المحاولات
}


int main()

{
    Login();

    system("pause>0");
    return 0;
}