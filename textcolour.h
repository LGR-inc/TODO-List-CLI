#ifndef TEXT_COLOUR_MANAGER_HEADER
#define TEXT_COLOUR_MANAGER_HEADER

#ifdef __linux__
  #include <unistd.h>
#elif defined(_WIN32)
  #include <windows.h>
#endif

class TextColourManager
{
// to use this class, declare a GLOBAL object of this class
// Its methods can then be used anywhere in the program
// Will automatically reset console colours when the program exits
private:
    #ifdef _WIN32
        HANDLE hConsole; // for changing console text colours
        CONSOLE_SCREEN_BUFFER_INFO initialScreenInfo; // stores initial console colour settings
    #endif
    bool is_bright;
    bool is_underlined;
    void changeTextColour(std::string foreground, std::string background, std::string bright, std::string underline);
public:
    TextColourManager();
    ~TextColourManager();
    // changes all colours back to what they were before
    void resetColours ();
    // clears the screen of text
    void cleanScreen ();
    // set the colour of text
    // COLOURS: red, green, blue, magenta, yellow, cyan, black, white
    void setTextColour ( std::string colour );
    // set the colour of the text background
    // COLOURS: red, green, blue, magenta, yellow, cyan, black, white
    void setBackgroundColour ( std::string colour );
    // turns underlining of text on/off
    // PARAMETERS: on, off, toggle
    void setUnderline ( std::string on_off );
    // toggles text brightness
    // PARAMETERS: on, off, toggle
     void setTextBrightness ( std::string on_off );
};

#endif
