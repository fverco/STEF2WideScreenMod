#pragma comment(lib, "Advapi32.lib")

#include "STEF2WideScreenMod.hpp"

#include <windows.h>
#include <iostream>
#include <winbase.h>
#include <lmcons.h>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<string> STEF2WideScreenMod::resolutionList({"1024x600", "1024x640", "1280x720", "1280x768",
                                                   "1280x800", "1360x768", "1366x768", "1440x900",
                                                   "1600x900", "1680x945", "1680x1050", "1920x1080",
                                                   "1920x1200", "2048x1152", "2560x1080", "2560x1440",
                                                   "2560x1600", "3072x768", "3440x1440", "3840x1024",
                                                   "3840x1600", "3840x2160", "4096x2160", "4800x900",
                                                   "5040x1050", "5760x1080", "5760x1200", "7680x1440"});

STEF2WideScreenMod::STEF2WideScreenMod() : pathToGame("C:\\Program Files (x86)\\GOG Galaxy\\Games\\Star Trek Elite Force II"),
                                           pathToMod(""),
                                           preferredResolution(""),
                                           assignedPathToGame(false),
                                           assignedPathToMod(false),
                                           detectedOldFiles(false),
                                           retrievedWinUserName(false){};

// Checks if a dir.ini file exists and reads from it.
bool STEF2WideScreenMod::detectConfigFile()
{
    if (fs::exists("dirs.ini")) // If the dir.ini file exists read from it.
    {
        ifstream dirsFile("dirs.ini");

        if (dirsFile)
        {
            string line("");            // Represents a single line of text in the file.
            string dir("");             // The type of directory, such as mod or game directory.
            string dirVal("");          // The path of the directory.
            unsigned int posOfColon(0); // The position of the colon seperating dir and dirVal's text.

            while (getline(dirsFile, line)) // Read through the file.
            {
                posOfColon = line.find_first_of(':');
                dir = line.substr(0, posOfColon);
                dirVal = line.substr(posOfColon + 1, line.length() - posOfColon + 1);

                if (dir == "GameDir") // The current line contains the game directory.
                {
                    // Verify that the game is actually there.
                    if (fs::exists(dirVal + "\\EF2.exe"))
                    {
                        pathToGame = dirVal;
                        assignedPathToGame = true;
                    }
                }
                else if (dir == "ModDir") // The current line contains the mod directory.
                {
                    // Verify that the mod is still there.
                    if (fs::exists(dirVal))
                    {
                        pathToMod = dirVal;
                        assignedPathToMod = true;
                    }
                }
                else // The current line is just giberish.
                {
                    continue;
                }
            }

            dirsFile.close();

            if (assignedPathToGame && assignedPathToMod)
            {
                return true;
            }
        }
        else
        {
            cout << "Error. Unable to open the dirs.ini file!" << endl
                 << endl;
        }
    }

    return false;
}

// Asks for the game directory path from the user and records it.
// Note: This will first check if the default value of pathToGame is correct. If it is then the user will not be asked anything.
void STEF2WideScreenMod::getGameDirectory()
{
    if (!assignedPathToGame) // If we haven't asked for the game directory from the user...
    {
        if (!fs::exists(pathToGame + "\\EF2.exe")) // If the default pathToGame is incorrect...
        {
            cout << "Could not find the path to the game directory." << endl;

            while (!fs::exists(pathToGame + "\\EF2.exe")) // Ask the user for the path and check if it is valid. If not, then keep asking.
            {
                cout << "Please provide a valid path to the game directory:" << endl;
                getline(cin, pathToGame);
                cout << endl;
            }
        }

        assignedPathToGame = true;

        // After obtaining a valid game directory, store it in the dirs.ini file.
        ofstream dirsFile("dirs.ini", ios_base::app);

        if (dirsFile)
        {
            dirsFile << "GameDir:" << pathToGame << endl;
            dirsFile.close();
        }
        else
        {
            cout << "Error writing game directory to file! Please run this app with admin privileges" << endl
                 << endl;
        }

        cout << "Game directory found!" << endl
             << endl;
    }
}

// Provide the user with instructions to download the mod and ask for it's path.
void STEF2WideScreenMod::getModDirectory()
{
    if (!assignedPathToMod)
    {
        bool pathIsValid(false);

        // Tell the user to download the mod files.
        cout << "Please download the required mod files from this link:" << endl
             << "https://www.wsgf.org/f/u/contrib/dr/2439/hacks/Star_Trek_Elite_Force_II.7z" << endl
             << endl
             << "Once you have downloaded the archive, extract it anywhere into its own folder" << endl;

        do
        {
            // Ask for the path to the mod directory.
            cout << "Please provide the path to the extracted mod folder:" << endl;

            getline(cin, pathToMod);
            cout << endl;

            // Check if the directory exists.
            while (!fs::exists(pathToMod))
            {
                cout << "The path you provided does not exist. Please provide a valid path:" << endl;
                getline(cin, pathToMod);
                cout << endl;
            }

            pathIsValid = true;

            // Check if the directory contains all of the files and folders.
            if (!fs::exists(pathToMod + "//" + "EF2.exe") || !fs::exists(pathToMod + "//base//" + "gamex86.dll"))
            {
                pathIsValid = false;
                cout << "Error! The path does not contain all the mod files. Please extract the mod archive and DO NOT TOUCH ANYTHING IN IT." << endl;
            }

        } while (!pathIsValid);

        assignedPathToMod = true;

        // After obtaining a valid path to the mod files, store it in the dirs.ini file.
        ofstream dirsFile("dirs.ini", ios_base::app);

        if (dirsFile)
        {
            dirsFile << "ModDir:" << pathToMod << endl;
            dirsFile.close();
        }
        else
        {
            cout << "Error writing mod directory to file! Please run this app with admin privileges" << endl
                 << endl;
        }
    }
}

// Gets the preferred resolution from the user.
void STEF2WideScreenMod::getPreferredResolution()
{
    // Only execute this function if both the game and mod directories are known.
    if (assignedPathToGame && assignedPathToMod)
    {
        string input("");

        // Ask the user what resolution they want.
        cout << "Which resolution would you like your game to have?" << endl;

        // Print the resolution options.
        for (int i(0); i < resolutionList.size(); ++i)
        {
            cout << "[" + to_string(i + 1) + "] - " + resolutionList.at(i) << endl;
        }

        int resolutionNum(0);

        do
        {
            // Get the user's choice.
            cout << endl
                 << "Type the number of the resolution you want: ";
            cin >> input;
            resolutionNum = stoi(input);

        } while (resolutionNum < 1 || resolutionNum > resolutionList.size());

        preferredResolution = resolutionList.at(resolutionNum - 1);

        // Repeat the selected resolution to the user.
        cout << endl
             << "Your resolution: " << preferredResolution << endl
             << endl;
    }
}

// Get the user's Windows username.
// This is used to find the right .cfg file to modify.
bool STEF2WideScreenMod::getWinUserName()
{
    char username[UNLEN + 1];
    DWORD usernameLen = UNLEN + 1;

    if (GetUserNameA(username, &usernameLen))
    {
        winUserName = username;
        retrievedWinUserName = true;
        return true;
    }

    return false;
}

// Check for old files from the last time the user applied the mod.
bool STEF2WideScreenMod::detectOldFiles()
{
    // Only execute this function if both the Windows username, game and mod directories are known.
    if (assignedPathToGame && assignedPathToMod && retrievedWinUserName)
    {
        if (fs::exists(pathToGame + "//EF2.exe.old") ||
            fs::exists(pathToGame + "//base//gamex86.dll.old") ||
            fs::exists(pathToGame + "//base//" + winUserName + ".cfg.old"))
        {
            detectedOldFiles = true;
            return true;
        }
    }

    return false;
}

// Applies the mod to the game files.
void STEF2WideScreenMod::applyMod(const bool &backupFiles)
{
    // Only execute this function if both the Windows username, game and mod directories are known.
    if (assignedPathToGame && assignedPathToMod && retrievedWinUserName)
    {
        // A list of all files to mod.
        vector<string> filesToMod({"EF2.exe",
                                   "base//gamex86.dll"});
        string userConfigFile("base//" + winUserName + ".cfg");

        // Backup old files before modding (if required).
        if (backupFiles)
        {
            for (int i(0); i < filesToMod.size(); ++i)
            {
                fs::rename(pathToGame + "//" + filesToMod.at(i),
                           pathToGame + "//" + filesToMod.at(i) + ".old");
            }

            fs::copy(pathToGame + "//" + userConfigFile,
                     pathToGame + "//" + userConfigFile + ".old");
        }

        // Copy over mod files.
        for (int i(0); i < filesToMod.size(); ++i)
        {
            fs::copy(pathToMod + "//" + filesToMod.at(i),
                     pathToGame + "//" + filesToMod.at(i),
                     fs::copy_options::overwrite_existing);
        }

        // Write the new resolution to the user's config file.
        if (!modConfigFile(pathToGame + "//" + userConfigFile))
        {
            cout << "Error writing mod directory to file! Please run this app with admin privileges" << endl
                 << endl;
        }
        else
        {
            cout << "Successfully installed the mod." << endl
                 << endl;
        }
    }
}

// Modifies the config file to use the new resolution.
// NOTE: This will create a temporary file before deleting the original,
// then the temporary file will be renamed as the original.
// NOTE2: This function does not make a backup copy, so remember to make one before calling it.
bool STEF2WideScreenMod::modConfigFile(const string &configFilePath)
{
    // Only execute this function if both the Windows username, game and mod directories are known.
    if (assignedPathToGame && retrievedWinUserName)
    {
        ifstream configFile(configFilePath, ios_base::in);              // The user's original config file.
        ofstream tmpConfigFile(configFilePath + ".tmp", ios_base::out); // The new config file.
        bool rModeFound(false);                                         // Was the "seta r_mode" line found?
        bool customResAdded(false);                                     // Has the two lines for custom resolution been added yet?

        if (configFile && tmpConfigFile) // Check if the original and temp config files are open.
        {
            string line("");

            // Read through every line in the original config file
            // and write them to the temp config file.
            // Also add the new lines to the temp config file.
            while (getline(configFile, line))
            {
                if (!rModeFound && line.find("seta r_mode") != string::npos) // If we found the line with "seta r_mode"...
                {
                    line = "seta r_mode \"-1\""; // Change the line to the new "r_mode".
                    rModeFound = true;
                }
                else if (rModeFound && !customResAdded) // If we have found the "r_mode" but haven't set the custom resolution...
                {
                    // Extract the width and height from the preferredResolution.
                    unsigned int posOfX(preferredResolution.find_first_of('x'));
                    string width(preferredResolution.substr(0, posOfX));
                    string height(preferredResolution.substr(posOfX + 1, preferredResolution.length() - posOfX + 1));

                    // Add two new lines to assign the preferresResolution.
                    tmpConfigFile << "seta r_customwidth \"" + width + "\"" << endl
                                  << "seta r_customheight \"" + height + "\"" << endl;
                    customResAdded = true;
                }

                // Write the line from the original config file over to the temp config file.
                tmpConfigFile << line << endl;
            }

            configFile.close();
            tmpConfigFile.close();

            // Delete the original config file and rename the temp file to be the new original.
            fs::remove(configFilePath);
            fs::rename(configFilePath + ".tmp", configFilePath);

            if (rModeFound && customResAdded) {
                return true;
            }
        }
    }

    // Delete the temp config file if it remained.
    if (fs::exists(configFilePath + ".tmp"))
    {
        fs::remove(configFilePath + ".tmp");
    }

    return false;
}

// Reverts the game back to before the mod was installed.
void STEF2WideScreenMod::revertMod()
{
    // Only execute this function if old files were detected and both the Windows username,
    // game and mod directories are known.
    if (detectedOldFiles && assignedPathToGame && assignedPathToMod && retrievedWinUserName)
    {
        vector<string> filesToRevert({"EF2.exe",
                                      "base//gamex86.dll",
                                      "base//" + winUserName + ".cfg"});

        for (int i(0); i < filesToRevert.size(); ++i)
        {
            if (fs::exists(pathToGame + "//" + filesToRevert.at(i) + ".old"))
            {
                fs::remove(pathToGame + "//" + filesToRevert.at(i));
                fs::rename(pathToGame + "//" + filesToRevert.at(i) + ".old",
                           pathToGame + "//" + filesToRevert.at(i));
            }
        }

        cout << endl
             << "Successfully uninstalled the mod." << endl
             << endl;
    }
}