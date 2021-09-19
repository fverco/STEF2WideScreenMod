// This project must be compiled with C++17
#include "STEF2WideScreenMod.hpp"

#include <iostream>

using namespace std;

int main()
{
	STEF2WideScreenMod modder;

	cout << "Welcome to the Star Trek Voyager Elite Force II Wide Screen Modder. (v1.0)" << endl
		 << endl;

	modder.detectConfigFile();
	modder.getGameDirectory();
	modder.getModDirectory();
	modder.getWinUserName();

	if (!modder.detectOldFiles())
	{
		modder.getPreferredResolution();
		system("pause");
		cout << endl;
		modder.applyMod();
	}
	else
	{
		string input("");

		cout << "I have detected old files from a previous mod installation." << endl
			 << endl
			 << "Would you like to uninstall the mod? (This will restore your game to it's original state) " << endl;

		do
		{
			cout << "Answer <Y> for yes or <N> for no: ";
			cin >> input;
		} while (input != "Y" && input != "y" && input != "N" && input != "n");

		cout << endl;

		if (input == "Y" || input == "y")
		{
			modder.revertMod();
		}
		else
		{
			modder.getPreferredResolution();
			system("pause");
			cout << endl;
			modder.applyMod(false);
		}
	}

	cout << "Have fun!" << endl
		 << endl;

	cout << "PS. If you encounter any problems with the mod. Remember to visit this link for solutions:" << endl
		 << "https://github.com/fverco/STEF2WideScreenMod#common-problems" << endl
		 << endl;

	system("pause");

	return 0;
}