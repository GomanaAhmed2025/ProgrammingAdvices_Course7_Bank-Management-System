#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

const string FileName = "ClientData.txt";

void ShowMainMenu();

enum enMainMenuOption
{
	ShowClientList = 1, AddNewCLient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Exit = 6
};

struct stClient
{
	string AccNum,
		PinCode,
		Name,
		PhoneNum;

	double Balance = 0;

	bool MarkForDelete = false;
};


string ConvertRecordIntoLine(stClient Client, string Separator = "$||$")
{
	string ClientLine = "";

	ClientLine += Client.AccNum + Separator;
	ClientLine += Client.PinCode + Separator;
	ClientLine += Client.Name + Separator;
	ClientLine += Client.PhoneNum + Separator;
	ClientLine += to_string(Client.Balance);

	return ClientLine;
}

vector <string> SplitString(string YourString, string Delimeter)
{
	short Position = 0;
	string  Word;
	vector <string> vString;

	while ((Position = YourString.find(Delimeter)) != std::string::npos)
	{
		Word = YourString.substr(0, Position);

		if (Word != "")
			vString.push_back(Word);

		YourString.erase(0, Position + Delimeter.length());

	}

	if (YourString != "")
		vString.push_back(YourString);

	return vString;

}

stClient ConvertLineIntoRecord(string Line, string Separator = "$||$")
{
	vector <string> vClient= SplitString(Line, Separator);
	stClient Client;

	Client.AccNum = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.PhoneNum = vClient[3];
	Client.Balance = stod(vClient[4]);

	return Client;
}

vector <stClient> LoadClientDataFromFile(string FileName)
{
	fstream MyFile;
	vector <stClient> vClients;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineIntoRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}


void PrintClientRecordLine(stClient C)
{
	cout << "|| " << left << setw(15) << C.AccNum;
	cout << " || " << left << setw(15) << C.PinCode;
	cout << " || " << left << setw(25) << C.Name;
	cout << " || " << left << setw(16) << C.PhoneNum;
	cout << " || " << left << setw(12) << C.Balance << " || ";
}

void ShowAllCLientsData()
{
	vector <stClient> vClients = LoadClientDataFromFile(FileName);

	cout << "\t\t\t\t\tClient List (" << vClients.size() << ") Client(s)." << endl;
	cout << "=========================================================================================================" << endl;
	cout << "|| " << left << setw(15) << "Account Number";
	cout << " || " << left << setw(15) << "PIN Code";
	cout << " || " << left << setw(25) << "Client Name";
	cout << " || " << left << setw(16) << "Phone Number";
	cout << " || " << left << setw(12) << "Balnce" << " || ";
	cout << endl << "=========================================================================================================" << endl;


	if (vClients.size() == 0)
		cout << "\n\t\t\t\tNo Clients Available In The System!...";
	else
	{
		for (stClient C : vClients)
		{
			PrintClientRecordLine(C);
			cout << endl;
		}
	}
	
	cout << "=========================================================================================================" << endl;

}


bool ClientExistByAccountNumber(string AccountNumber, string FileName)
{
	vector<stClient>vClients;
	fstream MyFile;


	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient ClientRecord;

		while (getline(MyFile, Line))
		{
			ClientRecord = ConvertLineIntoRecord(Line);
			if (ClientRecord.AccNum == AccountNumber)
			{
				MyFile.close();
				return true;
			}

			vClients.push_back(ClientRecord);
		}

		MyFile.close();
	}

	return false;
}

stClient ReadNewClient()
{
	stClient NewClient;
	cout << "\nPLEASE, ENTER THE CLIENT DATA\t\n\n";

	cout << "Enter The Account Number : ";
    //Usage of std::ws will extract all whitespaces character
	getline(cin >> ws, NewClient.AccNum);

	while (ClientExistByAccountNumber(NewClient.AccNum, FileName))
	{
		cout << "\nThe Client With [" << NewClient.AccNum << "] already Exist, Please enter Another Account Number\n";
		getline(cin >> ws, NewClient.AccNum);
	}

	cout << "Enter The PIN Code : ";
	getline(cin, NewClient.PinCode);
	cout << "Enter The Client Name : ";
	getline(cin, NewClient.Name);
	cout << "Enter The Phone Number : ";
	getline(cin, NewClient.PhoneNum);
	cout << "Enter The Balance : ";
	cin >> NewClient.Balance;

	return NewClient;

}

char ToUpper(char Letter)
{
	return toupper(Letter);
}

void AddClientLineToFile(string FileName, string ClientLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << ClientLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	stClient NewClient = ReadNewClient();

	AddClientLineToFile(FileName, ConvertRecordIntoLine(NewClient));
}

void AddCLients()
{
	char AddMore;
	do 
	{
		//system("cls");
		cout << "Adding New Client : \n\n";

		AddNewClient();
		cout << "\n\nThe Client Was Added Successfully :-) , \n Do You Want To Add More Clients (Y/N) ? ";
		cin >> AddMore;
	}while (ToUpper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
	cout << "\n=======================================\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n=======================================\n";

	AddCLients();
}


string ReadAccountNumber()
{
	string AccNum;

	cout << "Please, Enter The Account Number : ";
	cin >> AccNum;

	return AccNum;
}

bool FindClientByAccountNumber(string AccNum, stClient &Client ,vector <stClient> &vClients)
{
	for (stClient C : vClients)
	{
		if (C.AccNum == AccNum)
		{
			Client = C;
			return true;
		}
	}
	return false;

}

void PrintClientCard(stClient Client)
{
	cout << "\nHere is the Client Card : \n\n";
	cout << "Account Number : " << Client.AccNum << endl;
	cout << "PIN code       : " << Client.PinCode << endl;
	cout << "Client Name    : " << Client.Name << endl;
	cout << "Phone Number   : " << Client.PhoneNum << endl;
	cout << "Balance        : " << Client.Balance << endl;
	cout << endl;
}

void ShowFindCLientScreen()
{
	cout << "\n=======================================\n";
	cout << "\tFind Clients Screen";
	cout << "\n=======================================\n";

	vector<stClient> vClients = LoadClientDataFromFile(FileName);
	stClient Client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, Client, vClients))
		PrintClientCard(Client);
	else
		cout << "\n The Client Wit Accout Number [" << AccountNumber << "] NotFound !\n";
}

bool MarkClientForDeleting(vector <stClient> &vClients, string AccNum)
{
	for (stClient& C : vClients)
	{
		if (C.AccNum == AccNum)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

vector <stClient> SaveClientDataToFile(vector <stClient>& vClient)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stClient& C : vClient)
		{
			if (C.MarkForDelete == false)
			{
				Line = ConvertRecordIntoLine(C);
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vClient;
}

bool DeleteClientByAccountNumber(string AccNum, vector <stClient>& vClients)
{
	stClient client;
	char YourAns;

	if (FindClientByAccountNumber(AccNum, client, vClients))
	{
		PrintClientCard(client);
		cout << "\n\nAre You Sure You Want To Delete This Client (Y/N) ? ";
		cin >> YourAns;

		if (YourAns == 'Y' || YourAns == 'y')
		{
			MarkClientForDeleting(vClients, AccNum);

			SaveClientDataToFile(vClients);

			//Refreash vector
			vClients = LoadClientDataFromFile(FileName);
			cout << "\n\nThe Client Was Deleted Successfully\n";

			return true;

		}
	}
	else
		cout << "\n The Client With Accout Number [" << AccNum << "] Not Found !\n";

	return false;
}

void ShowDeleteCLientScreen()
{
	cout << "\n=======================================\n";
	cout << "\tDelete Clients Screen";
	cout << "\n=======================================\n";

	vector<stClient> vClients =  LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}


stClient ChangeClientRecord(string AccNum)
{
	stClient Updated;

	cout << "\nPLEASE, ENTER THE CLIENT DATA\t\n\n";

	Updated.AccNum = AccNum;

	cout << "Enter The PIN Code : ";
	getline(cin>>ws, Updated.PinCode);
	cout << "Enter The Client Name : ";
	getline(cin, Updated.Name);
	cout << "Enter The Phone Number : ";
	getline(cin, Updated.PhoneNum);
	cout << "Enter The Balance : ";
	cin >> Updated.Balance;

	return Updated;

}

bool UpdateClientByAccountNumber(string AccNum,vector <stClient>& vClients)
{
	char YourAns;
	stClient client;

	if (FindClientByAccountNumber(AccNum, client, vClients))
	{
		PrintClientCard(client);
		cout << "\n\nAre You Sure You Want To Update This Client (Y/N) ? ";
		cin >> YourAns;

		if (ToUpper(YourAns) == 'Y')
		{
			for (stClient& C : vClients)
				if (C.AccNum == AccNum)
				{
					C = ChangeClientRecord(AccNum);
					break;
				}

			SaveClientDataToFile(vClients);

			//Refreash vector
			vClients = LoadClientDataFromFile(FileName);
			cout << "\n\nThe Client Was Updated Successfully\n";

			return true;
		}
	}
	else
		cout << "\n The Client With Accout Number [" << AccNum << "] Not Found !\n";

	return false;
}

void ShowUpdateCLientScreen()
{
	cout << "\n=======================================\n";
	cout << "\tUpdate Clients Screen";
	cout << "\n=======================================\n";

	vector<stClient> vClients = LoadClientDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber,vClients);
}


void ShowEndScreen()
{

	cout << "\n=======================================\n";
	cout << "\tProgram End :-) ...";
	cout << "\n=======================================\n";

}


void GoBackToMainMenu()
{
	cout << "\n\nPress any key to Go Back To Main Menu...";
	system("pause>0");
	ShowMainMenu();
}

short ReadMainMenuOption()
{
	short YourChoice = 0;

	cout << "\nChoose What Do You Want To Do [ 1 - 6 ] ? ";
	cin >> YourChoice;

	return YourChoice;
}

void PerformMainMenuOption(enMainMenuOption MainMenuOption)
{
	switch (MainMenuOption)
	{
	case ShowClientList:
	{
		system("cls");
		ShowAllCLientsData();
		GoBackToMainMenu();
		break;
	}
	case AddNewCLient:
	{
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	}

	case DeleteClient:
	{
		system("cls");
		ShowDeleteCLientScreen();
		GoBackToMainMenu();
		break;
	}
	case UpdateClient:
	{
		system("cls");
		ShowUpdateCLientScreen();
		GoBackToMainMenu();
		break;
	}

	case FindClient:
	{
		system("cls");
		ShowFindCLientScreen();
		GoBackToMainMenu();
		break;
	}

	case Exit:
	{
		system("cls");
		ShowEndScreen();
		break;
	}

	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";

	cout << "\t[1] Show Client List\n";
	cout << "\t[2] Add New Client\n";
	cout << "\t[3] Delete Client\n";
	cout << "\t[4] Update Client Information\n";
	cout << "\t[5] Find Client\n";
	cout << "\t[6] Exit\t";

	PerformMainMenuOption((enMainMenuOption)ReadMainMenuOption());
}


int main()
{
	ShowMainMenu();
	return 0;
}