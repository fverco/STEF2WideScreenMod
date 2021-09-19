#include <string>
#include <vector>

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
    std::string pathToGame;
    std::string pathToMod;
    std::string preferredResolution;
    std::string winUserName;
    bool assignedPathToGame;
    bool assignedPathToMod;
    bool detectedOldFiles;
    bool retrievedWinUserName;

    static std::vector<std::string> resolutionList; // List of recommended resolutions for the user.

    bool modConfigFile(const std::string &configFilePath);
};