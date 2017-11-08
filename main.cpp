#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>

// Linux support is here! Yay!
#ifdef __linux__
  #include <unistd.h>
#elif defined(_WIN32)
  #include <windows.h>
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // for changing console text colours
  CONSOLE_SCREEN_BUFFER_INFO initialScreenInfo; // stores initial console colour settings
#endif

std::vector<std::string> thingsToDo;

inline void cleanScreen()
{
    // clears terminal text in a cross-platform way
    #ifdef __linux__
        system("clear");
    #elif defined(_WIN32)
        system("cls");
    #endif
}

inline void resetColours()
{
    // restores terminal colours to what they were before the program started. automatically called upon program exit
    #ifdef __linux__
        std::cout << "\033[0m";
    #elif defined(_WIN32)
        SetConsoleTextAttribute(hConsole, initialScreenInfo.wAttributes);
    #endif
}

inline void changeTextColour(std::string foreground, std::string background = "", std::string bright = "", std::string underline = "")
{
    /*
     foreground changes text colours
     - possible colours to pass as parameters:
       * "white"
       * "black"
       * "red"
       * "green"
       * "blue"
       * "magenta"
       * "cyan"
       * "yellow"
     background changes the text background colours
     - possible colours to pass as parameters:
       * "white"
       * "black"
       * "red"
       * "green"
       * "blue"
       * "magenta"
       * "cyan"
       * "yellow"
     bright changes text colour brightness
       * "on" turns text brightness on
       * "off" turns text brightness off
     underline controls underlining of text
       * "on" turns text underlining on
       * "off" turns text underlining off

     any parameter set/left as "" (empty string) will not be modified
    */
    #ifdef __linux__
        // linux terminals use ANSI colour codes
        std::string ansi = "\033[";

        if (foreground == "black")        ansi += "30";
        else if (foreground == "red" )    ansi += "31";
        else if (foreground == "green" )  ansi += "32";
        else if (foreground == "yellow")  ansi += "33";
        else if (foreground == "blue")    ansi += "34";
        else if (foreground == "magenta") ansi += "35";
        else if (foreground == "cyan")    ansi += "36";
        else if (foreground == "white")   ansi += "37";

        if(foreground != "") ansi += ";";

        if ( background == "black" )      ansi += "40";
        else if (background == "red")     ansi += "41";
        else if (background == "green" )  ansi += "42";
        else if (background == "yellow")  ansi += "43";
        else if (background == "blue")    ansi += "44";
        else if (background == "magenta") ansi += "45";
        else if (background == "cyan")    ansi += "46";
        else if (background == "white")   ansi += "47";

        if(background != "") ansi += ";";

        if(bright == "on") ansi += "1;";
        else if(bright == "off") ansi += "21;";

        if(underline == "on") ansi += "4";
        else if(underline == "off") ansi += "24";

        ansi += "m";
        std::cout << ansi;

    #elif defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO screenInfo;
        GetConsoleScreenBufferInfo(hConsole, &screenInfo); // get console colour config
        unsigned short int prev_attributes = screenInfo.wAttributes; // store current config
        unsigned short int attributes = 0; // will store changes to be made

        if (foreground == "red" )         attributes |= FOREGROUND_RED;
        else if (foreground == "green" )  attributes |= FOREGROUND_GREEN;
        else if (foreground == "yellow")  attributes |= (FOREGROUND_GREEN | FOREGROUND_RED);
        else if (foreground == "blue")    attributes |= FOREGROUND_BLUE;
        else if (foreground == "magenta") attributes |= (FOREGROUND_BLUE | FOREGROUND_RED);
        else if (foreground == "cyan")    attributes |= (FOREGROUND_GREEN | FOREGROUND_BLUE);
        else if (foreground == "white")   attributes |= (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

        if(foreground != "")
        {
            // overwrites current font colour if a new font colour is to be set
            for(int n = 0; n < 4; n++)
            {
                prev_attributes ^= (prev_attributes & (1 << n));
                /*              ~~  ~~~~~~~~~~~~~~~~~  ~~~~~~
                                |   |                  |
                                |   |                  a byte with the nth bit set to 1 and all other bits as 0
                                |   if the nth byte of prev_attributes is 1, only the nth byte is 1, otherwise all bytes are 0
                                XOR with new value means that if the previous value of the nth bit was 1, it is now 0 and if it was 0 it is now 0
                */
            }
        }

        if (background == "red" )         attributes |= BACKGROUND_RED;
        else if (background == "green" )  attributes |= BACKGROUND_GREEN;
        else if (background == "yellow")  attributes |= (BACKGROUND_GREEN | BACKGROUND_RED);
        else if (background == "blue")    attributes |= BACKGROUND_BLUE;
        else if (background == "magenta") attributes |= (BACKGROUND_BLUE | BACKGROUND_RED);
        else if (background == "cyan")    attributes |= (BACKGROUND_GREEN | BACKGROUND_BLUE);
        else if (background == "white")   attributes |= (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

        if(background != "")
        {
            // as above, overwrites current background colour if a new background colour is to be set
            for(int n = 4; n < 8; n++)
            {
                prev_attributes ^= (prev_attributes & (1 << n));
            }
        }

        if(bright == "on") attributes |= FOREGROUND_INTENSITY;
        else if(bright == "off") prev_attributes ^= (prev_attributes & FOREGROUND_INTENSITY);

        if(underline == "on") attributes |= COMMON_LVB_UNDERSCORE;
        else if(underline == "off") prev_attributes ^= (prev_attributes & COMMON_LVB_UNDERSCORE);

        attributes = attributes | prev_attributes; // keeps old attributes that have not been changed in the scope of this function

        SetConsoleTextAttribute(hConsole, attributes); // apply changes
    #endif

    std::cout.flush(); // flush buffer to apply changes
}

bool loadList()
{
    // hardcoding G:\\ is not very portable :(
    // however getting drive letters is a pain, so fair enough
    std::ifstream storageFile("G:\\thingsToDo_ExternalStore.txt");
    if(!storageFile)
    {
        storageFile.close();
        std::ifstream storageFile("thingsToDo.txt");
         if(!storageFile) //Always test the file open.
        {
            std::cout<<"Error: File does not exist in appropriate location."<<std::endl;
            storageFile.close();
            return false;
        }
    }
    std::string line;
    while (std::getline(storageFile, line))
    {
        thingsToDo.push_back(line);
    }
    storageFile.close();
    return true;
}

void writeItem(std::string itemToWrite)
{
    std::ofstream storageFile;
    storageFile.open("thingsToDo.txt");
    storageFile << itemToWrite << "\n";
    storageFile.close();
}

void writeListWhole(std::vector<std::string> vectorToBeWritten)
{
    std::ofstream storageFile;
    storageFile.open("thingsToDo.txt");
    storageFile.clear();
    for(unsigned int limit = 0 ; limit < vectorToBeWritten.size() ; limit++)
    {
        storageFile << vectorToBeWritten[limit] << "\n";
    }
    storageFile.close();
}

void printList()
{
    for(unsigned int limit = 0 ; limit < thingsToDo.size() ; limit++)
    {
        std::cout << "[" << limit + 1 << "]\t\t" << thingsToDo[limit] << std::endl;
    }
}

void assignItemToDo()
{
    std::string valueToBeAssigned;
    std::cout << "So you need to do a thing." << std::endl;
    std::cout << "State that which you need to do: " << std::endl;
    std::cin.ignore();
    std::getline(std::cin, valueToBeAssigned);
    thingsToDo.push_back(valueToBeAssigned);
}

void removeItemToDo()
{
    std::string valueToBeRemoved = "a";
    std::cout << "So you want to remove a thing; good for you." << std::endl;
    std::cout << "State the number of the thing you want to remove." << std::endl;
    std::cin >> valueToBeRemoved;
    int valueToBeRemovedInt;
    std::stringstream convert(valueToBeRemoved);
    if(!(convert >> valueToBeRemovedInt))
    {
        valueToBeRemovedInt = 99;
    }
    thingsToDo.erase(thingsToDo.begin() + (valueToBeRemovedInt - 1));
}

void exportToDoList()
{
    std::ofstream storageFile;
    storageFile.open("G:\\thingsToDo_ExternalStore.txt");
    storageFile.clear();
    for(unsigned int limit = 0 ; limit < thingsToDo.size() ; limit++)
    {
        storageFile << thingsToDo[limit] << "\n";
    }
    storageFile.close();
}

void query()
{
    std::string aResponse;
    while(aResponse != "4")
    {
        cleanScreen();
        aResponse = "a";
        if(thingsToDo.size() == 0)
        {
            std::cout << "You have nothing stored to do! Good for you! Go practice Starcraft II you scrub." << std::endl;
        }
        else
        {
            printList();
        }
        while(aResponse != "1" && aResponse != "2" && aResponse != "3" && aResponse != "4")
        {
            std::cout << std::endl
                << "Please enter a valid response\n" << std::endl
                << " [1] " << "Add a thing to do" << std::endl
                << " [2] " << "Remove an item" << std::endl
                << " [3] " << "Export to USB" << std::endl
                << " [4] " << "Terminate program; get on with life" << std::endl
                << "\nType the appropriate number for the option you wish to pick" << std::endl;
            std::cin >> aResponse;
        }
        if(aResponse == "1")
        {
            assignItemToDo();
        }
        else if(aResponse == "2")
        {
            removeItemToDo();
        }
        else if(aResponse == "3")
        {
            exportToDoList();
        }
        else if(aResponse == "4")
        {
            std::cout << "Really, do you really have nothing to do?" << std::endl;
            std::cout << "Alright, go and procrastinate." << std::endl;
        }
    }
}

void loadingQuery()
{
    if (!loadList())
    {
        std::ofstream storageFile("thingsToDo.txt");
        storageFile.close();
    }
    query();
}

int main()
{
    #ifdef _WIN32
        // gets current colour settings in a windows environment
        GetConsoleScreenBufferInfo(hConsole, &initialScreenInfo);
    #endif // _WIN32
    atexit(resetColours); // sets the resetColours() function to run on program exit
    loadingQuery();
    writeListWhole(thingsToDo);
}
