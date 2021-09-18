#include <string>
#include <vector>

using namespace std;

// Class used to apply the wide screen mod.
class STEF2WideScreenMod
{
public:
    STEF2WideScreenMod();

    bool detectConfigFile();
    void getGameDirectory();
    void getModDirectory();
    void getPreferredResolution();
    bool getWinUserName();
    bool detectOldFiles();
    void applyMod(const bool &backupFiles = true);
    void revertMod();

private:
    string pathToGame;
    string pathToMod;
    string preferredResolution;
    string winUserName;
    bool assignedPathToGame;
    bool assignedPathToMod;
    bool detectedOldFiles;
    bool retrievedWinUserName;

    static vector<string> resolutionList; // List of recommended resolutions for the user.
};