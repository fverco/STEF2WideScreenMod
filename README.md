# STEF2WideScreenMod

An application that applies the widescreen mod to Star Trek Elite Force II.

## Common problems

---

* ***Problem***: My game's screen is cropped after applying the mod.
* ***Solution***: Go to your game directory. Right click the "EF2.exe" and click properties. Click on the Compatibility tab and click on "Change high DPI settings". Then click on the check box with "Override high DPI scaling behaviour. Scaling performed by:" next to it. Make sure the box below it has "Application" selected. Click OK and apply the changes. This should fix the cropping issue.

## Requirements for compiling code

---

You must use a compiler that supports atleast C++17 and most importantly it must support the File system library. Visit [this](https://en.cppreference.com/w/cpp/compiler_support/17) page to see if your compiler meets these requirements.

I used VSCode with MSVC and the VSCode json files are already setup to use it. Feel free to use wichever IDE/code editor and compiler you wish.

## Before compiling

---

If you decide to use my preset settings, you must create a folder called "compiled" in the root. VSCode will compile the application to this folder.
