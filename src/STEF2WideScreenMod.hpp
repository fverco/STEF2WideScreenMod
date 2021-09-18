#include <string>
#include <vector>

using namespace std;

class STEF2WideScreenMod
{
public:
    STEF2WideScreenMod();

    bool detectConfigFile();
    void getGameDirectory();
    void getModDirectory();
    void getPreferredResolution();
    bool detectOldFiles();
    void applyMod(const bool &backupFiles = true);
    void revertMod();

private:
    string pathToGame;
    string pathToMod;
    string preferredResolution;
    bool assignedPathToGame;
    bool assignedPathToMod;
    bool detectedOldFiles;

    static vector<string> resolutionList;
};