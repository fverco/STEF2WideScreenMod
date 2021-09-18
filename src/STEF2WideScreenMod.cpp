#pragma comment(lib, "Advapi32.lib")

#include "STEF2WideScreenMod.hpp"

#include <windows.h>
#include <iostream>
#include <winbase.h>
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
                                           detectedOldFiles(false){};