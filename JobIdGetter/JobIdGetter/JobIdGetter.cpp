// You don't wanna read my goofy ahh code lol

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sys/stat.h>
#include <time.h>
#include <Windows.h>
#include <cstdio>

using namespace std;

string getProfile()
{
    // Get Profile Name
    char* user_name = getenv("USERPROFILE");
    string userString = user_name;

    // A Complete Normal String
    string userProfile = userString.replace(0, 9, "");

    return userProfile;
}

static time_t getFileModifiedTime(const char* path)
{
    struct stat attr;
    if (stat(path, &attr) == 0)
    {
        return attr.st_mtime;
    }
    return 0;
}

string init()
{
    // Directory
    string path = "C:\\Users\\" + getProfile() + "\\AppData\\Local\\Roblox\\logs";
    string curFile = "none";
    time_t time = 9999999999;

    // Get Directory File
    for (const auto& entry : filesystem::directory_iterator(path))
    {
        // Convert String
        string fileName = entry.path().string();
        if (fileName != path + "\\archive")
        {
            time_t fileTime = getFileModifiedTime(fileName.c_str());

            if (difftime(time, fileTime))
            {
                curFile = fileName;
                time = fileTime;
            }
        }
    }

    return curFile;
}

string fetch(string input, string start, string end)
{
    size_t startPos = input.find(start);
    if (startPos == string::npos)
    {
        return "";
    }

    startPos += start.length();

    size_t endPos = input.find(end);
    if (endPos == string::npos)
    {
        return "";
    }

    return input.substr(startPos, endPos - startPos);
}

string fetchLength(string input, string start, int length)
{
    size_t startPos = input.find(start);
    if (startPos == string::npos)
    {
        return "";
    }

    startPos += start.length();

    return input.substr(startPos, length);
}

int main()
{
    SetConsoleTitleA("Roblox Server Info");

    if (FindWindowA(nullptr, "Roblox") == NULL)
    {
        printf("Can't find Roblox instance.");
        cin.get();
    }

    // Get File Path
    string p = init();
    ifstream f;

    if (p == "none")
    {
        printf("No Game Avaliable!");
        cin.get();
    }

    f.open(p);

    if (f.is_open())
    {
        string getText = "";
        string getter;
        while (getline(f, getter)) {
            getText += getter;
        }
        f.close();

        string getJobid = fetch(getText, "Joining game '", "' p");
        string getPlaceId = fetch(getText, "placeId\":", ",\"");
        string getUDMUX = fetch(getText, "Server Address = ", "2023-");

        printf("[Server-JobId] ==> %s\n", getJobid.c_str());
        printf("[Server-PlaceId] ==> %s\n", getPlaceId.c_str());
        // printf("[Server-Network] ==> %s\n", getUDMUX.c_str());

        string getFinalizeLink = "Roblox.GameLauncher.joinGameInstance('" + getPlaceId + "', '" + getJobid + "')";
        printf("[Shared-Link]: %s\n\n", getFinalizeLink.c_str());

        if (getText.find("initiateTeleportToPlace"))
        {
            string splitData = fetchLength(getText, "initiateTeleportToPlace", getText.length());

            string getJobid = fetch(splitData, "Joining game '", "' p");
            string getPlaceId = fetch(getText, "initiateTeleportToPlace: URL: https://gamejoin.roblox.com/v1/join-game POST Body: {\"placeId\":", ",\"i");
            string getUDMUX = fetch(splitData, "UDMUX server ", ", and RCC server");

            if (getPlaceId != "" && getJobid != "")
            {
                printf("[Teleported Game (Unsupported)]::\n");
                printf("[Server-JobId] ==> %s\n", getJobid.c_str());
                printf("[Server-PlaceId] ==> %s\n", getPlaceId.c_str());
                // printf("[Server-Network] ==> %s\n", getUDMUX.c_str());

                string getFinalizeLink = "Roblox.GameLauncher.joinGameInstance('" + getPlaceId + "', '" + getJobid + "')";
                printf("[Shared-Link]: %s", getFinalizeLink.c_str());
            }
        }

        cin.get();
    }

    cin.get();

    return 0;
}