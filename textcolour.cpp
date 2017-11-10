#include <string>
#include <iostream>
#include <stdlib.h>

#ifdef __linux__
  #include <unistd.h>
#elif defined(_WIN32)
  #include <windows.h>
#endif

#include "textcolour.h"

 void TextColourManager::changeTextColour(std::string foreground, std::string background, std::string bright, std::string underline)
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
       * "toggle" toggles brightness
     underline controls underlining of text
       * "on" turns text underlining on
       * "off" turns text underlining off
       * "toggle" toggles underlining

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

        if( (foreground != "") && ( (background != "") || (bright != "") || (underline != "") ) ) ansi += ";";

        if ( background == "black" )      ansi += "40";
        else if (background == "red")     ansi += "41";
        else if (background == "green" )  ansi += "42";
        else if (background == "yellow")  ansi += "43";
        else if (background == "blue")    ansi += "44";
        else if (background == "magenta") ansi += "45";
        else if (background == "cyan")    ansi += "46";
        else if (background == "white")   ansi += "47";

        if( (background != "") && ( (bright != "") || (underline != "") ) ) ansi += ";";

        if(bright == "on")
        {
            ansi += "1";
            this->is_bright = true;
        }
        else if(bright == "off")
        {
            ansi += "21";
            this->is_bright = false;
        }

        if( (bright != "") && ( underline != "" ) ) ansi += ";";

        if(underline == "on")
        {
            ansi += "4";
            this->is_underlined = true;
        }
        else if(underline == "off")
        {
            ansi += "24";
            this->is_underlined = false;
        }

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

void TextColourManager::resetColours()
{
    // restores terminal colours to what they were before the program started. automatically called upon program exit
    #ifdef __linux__
        std::cout << "\033[0m";
    #elif defined(_WIN32)
        SetConsoleTextAttribute(hConsole, initialScreenInfo.wAttributes);
    #endif
    std::cout.flush();
}

void TextColourManager::cleanScreen()
{
    // clears terminal text in a cross-platform way
    #ifdef __linux__
        system("clear");
    #elif defined(_WIN32)
        system("cls");
    #endif
}

void TextColourManager::setTextColour(std::string colour)
{
    /*
     changes text colour
       - possible colours to pass as parameters:
        * "white"
        * "black"
        * "red"
        * "green"
        * "blue"
        * "magenta"
        * "cyan"
        * "yellow"
    */
    this->changeTextColour(colour, "", "", "");
}

void TextColourManager::setBackgroundColour(std::string colour)
{
    /*
     changes colour of text background
       - possible colours to pass as parameters:
        * "white"
        * "black"
        * "red"
        * "green"
        * "blue"
        * "magenta"
        * "cyan"
        * "yellow"
    */
    this->changeTextColour("", colour, "", "");
}

 void TextColourManager::setTextBrightness(std::string on_off)
{
    /*
     changes text colour brightness
       * "on" turns text brightness on
       * "off" turns text brightness off
       * "toggle" toggles brightness
    */
    if(on_off == "toggle")
    {
        if(this->is_bright)
            this->changeTextColour("", "", "off", "");
        else
            this->changeTextColour("", "", "on", "");
    }
    else if((on_off == "on") || (on_off == "off"))
    {
        this->changeTextColour("", "", on_off, "");
    }
}

void TextColourManager::setUnderline(std::string on_off)
{
    /*
     underline controls underlining of text
       * "on" turns text underlining on
       * "off" turns text underlining off
       * "toggle" toggles underlining
    */
    if(on_off == "toggle")
    {
        if(this->is_underlined)
            this->changeTextColour("", "", "", "off");
        else
            this->changeTextColour("", "", "", "on");
    }
    else if((on_off == "on") || (on_off == "off"))
    {
        this->changeTextColour("", "", "", on_off);
    }
}

TextColourManager::TextColourManager()
{
    this->is_bright = false;
    this->is_underlined = false;
    #ifdef _WIN32
        this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo( this->hConsole, &(this->initialScreenInfo) );
    #endif
}

TextColourManager::~TextColourManager()
{
    this->resetColours();
}
