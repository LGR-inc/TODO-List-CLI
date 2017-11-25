#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

/*
    TODO: Have external store loader search for external stores on all drive letters or otherwise generalise the stores loading to not be drive specific.
    TODO: standardise variable naming system, make up your mind. [IN PROGRESS]
*/
/*
    PS: Variables to be officially changed to camelCase.
*/

// Linux support is here! Yay!
#ifdef __linux__
  #include <unistd.h>
#elif defined(_WIN32)
  #include <windows.h>
#endif

#include "textcolour.h"
TextColourManager cmdTextColour;

void terminateProgram()
{
    // Called to terminate and to pass comment on this event
    
    std::cout 
        << std::endl 
        << "So you have decided to terminate the program." 
        << std::endl
        << "Fair enough; go procrastinate, or find something to do and come back." 
        << std::endl 
        << std::endl;
    std::system("timeout /t 5");
    std::system("exit");
}

void printList(std::vector<std::string> listToPrint)
{
    // Takes a vector and prints its elements in the desired format (Fairly self-explanatory)
    
    for(unsigned int k = 1; k < listToPrint.size(); k++) // Iterates k from 1 to the size of the list
    {
        std::cout << "[" << k << "]\t" << listToPrint[k-1] << std::endl; // Numbers the element (k) then prints the appropriate item in the list.
    }
}

void editAddItemToList(std::vector<std::string>& vectorToBeEdited)
{
    // Adds an item to the vector (TODO list) passed to it
    
    // Inquery for the item to be added
    std::cout
            << "So you want to add an item to do; ok, what is it?" << std::endl
            << "State the item you wish to add:" << std::endl;
    std::string itemToBeAdded; // Place holder variable for item to be added
    std::getline(std::cin, itemToBeAdded); // Assigns the all text on the present line to the place holder variable, as TODO list items may include spaces which would not be appropriately recorded by std::cin alone
    vectorToBeEdited.push_back(itemToBeAdded); // Add the item to the end of the list
}

void editRemoveItemFromList(std::vector<std::string>& vectorToBeEdited)
{
    // Removes a item from the vector (TODO list) passed to it
    
    // Inquery for the item to be removed
    std::cout
            << "So you wish to remove an item from the list?" << std::endl
            << "State the number associated with the item you wish to remove:" << std::endl;
    std::string itemToBeRemoved; // Holder for string result from inquery
    std::cin >> itemToBeRemoved; // Assigns the user input to holder
    int itemToBeRemovedInt; // To be used to access the vector later
    std::stringstream convert(itemToBeRemoved); // Medium of exchange; used to convert the string to int
    if(!(convert >> itemToBeRemovedInt)) // Check if conversion can be made to int
    {
        // If not:
        cmdTextColour.cleanScreen(); // Clear console; for clarity, avoids the console from getting cluttered
        printList(vectorToBeEdited); // Re-print the list
        std::cout << "Please enter a valid response" << std::endl; // Ask for a appropriate response
        editRemoveItemFromList(vectorToBeEdited); // Call parent function such that the check can be commited recursively untill this statement is not triggered
    }
    vectorToBeEdited.erase(vectorToBeEdited.begin() + (itemToBeRemovedInt - 1)); // If conversion is done successfully, erase the item of the appropriate position; using the appropriate shifts
}

void readLoadVectorFromExternalStoreOnDemand(std::vector<std::string>& vectorToBeLoaded, std::string pathToStore)
{ // [IN PROGRESS: PRESENTLY A MESS; MUCH WORK TO DO]
    // Assign values to vectorToBeLoaded based on a text document at the pathToStore
    
    std::string path;
    if(pathToStore[pathToStore.size()-1] != '\\')
    {
        path = pathToStore + "\thingsToDo_ExternalStore.txt";
    }
    // Last point; Thinking about sorting inputs from pathToStore, to see what the addition made below should be.
    // std::string path = pathToStore + "\\thingsToDo_ExternalStore.txt";
    std::ifstream storageFile(path);
    std::string line;
    while(std::getline(storageFile, line))
    {
        vectorToBeLoaded.push_back(line);
    }
}

void readLoadVectorFromStore(std::vector<std::string>& vectorToBeLoaded)
{
    // Assign values to vectorToBeLoaded from a store [PLACEHOLDER FUNCTION; 
    // not intend for use in final distribution, works for now, 
    // no where near what I would like but it lets the program run]
    
    std::ifstream storageFile("G:\\thingsToDo_ExternalStore.txt"); // Open an input stream to the file at this location
    if(!storageFile) // If that can't be done (i.e. it isn't there)
    {
        storageFile.close(); // Close that variable to be reassigned
        std::ifstream storageFile("thingsToDo.txt"); // Open an input stream to the file at this location
         if(!storageFile) // If that can't be done (i.e. it isn't there)
        {
            // Therefore no appropriate file found (very limited I know, I'm working on it)
            std::cout
                << "Error: File does not exist in appropriate location." <<std::endl
                << "Generating blank storage file." << std::endl;
            storageFile.close(); // Close that variable to be reassigned
            std::ofstream storageFile("thingsToDo.txt"); // Opens an output stream to the file at this location, but seen as though no such file exists it generates a blank one instead
            storageFile.close(); // Clost that variable to be reassigned and for neatness
            return; // Job done; no file to load from so a blank one made and therefore nothing to assign to the vectorToBeLoaded
        }
    }
    std::string line; // Holder for present line of the successfully opened file
    while(std::getline(storageFile, line)) // While there are more lines
    {
        vectorToBeLoaded.push_back(line); // Assign each of them, sequentially, to the vectorToBeLoaded
    }
    storageFile.close(); // Close the variable for neatness
}

void writeAddToLocalListStore(std::string vectorItemToBeStored)
{
    // Adds an item to the local storage file [Note: Is this obsolete now?]
    
    std::ofstream storageFile("thingsToDo.txt"); // Open an output stream to the file at this location (program directory)
    storageFile << vectorItemToBeStored << "\n"; // Add the item followed by a carriage return
    storageFile.close(); // Close the file for neatness
}

void writeOverwriteLocalListStore(std::vector<std::string> vectorToBeStored)
{
    // Overwrite the local store with the vectorToBeStored
    
    std::ofstream storageFile("thingsToDo.txt"); // Open an output stream to the file at this location
    storageFile.clear(); // Clear it (we are overwriting)
    for(unsigned int limit = 0 ; limit < vectorToBeStored.size() ; limit++) // Iterate up to the size of the vectorToBeStored
    {
        storageFile << vectorToBeStored[limit] << "\n"; // Add the element to the file and the return, then loop to add next item...
    }
    storageFile.close(); // Close file for neatness
}

void writeSpecificLocationLocalListStore(std::string vectorItemToBeStored, int lineNumber)
{
    // Write a new item(vectorItemToBeStored) 
    // to a specific location in the file(lineNumber)
    // [WORK IN PROGRESS; unsure if it works,
    // been a while since I thought about this bit]
    
    std::ofstream storageFile("thingsToDo.txt"); // Open output stream to the file at this location
    storageFile.seekp(lineNumber); // Seek the line given by the lineNumber; set 'cursor' location to the begining of that line
    storageFile << vectorItemToBeStored << "\n"; // Add new item, followed by a return
    storageFile.close(); // Close file for neatness
}

void queryAdd(std::vector<std::string>& thingsToDo, bool print = false)
{
    // Query loop offering options for adding items
    
    if (print) // If print == true, print list
    {
        printList(thingsToDo);
    }
    std::string inputResponse; // Variable for storing the response of the user
    // Print options
    std::cout 
            << "So you wish to add an item to the list, please choose an option:" << std::endl
            << "[1]\t" << "Add item to the end of list" << std::endl
            << "[2]\t" << "Add item to specific position" << std::endl
            << "[X]\t" << "Back" << std::endl
            << std::endl;
    std::cin >> inputResponse; // Store user resposne
    if (inputResponse == "1") // Add item to the end of the list
    {
        editAddItemToList(thingsToDo);
    }
    else if (inputResponse == "2") // Add item to specific line of the list
    {
        // editAddItemToPosition(thingsToDo); // Ansent Function; current closest is writeSpecificLocationLocalListStore
    }
    else if (inputResponse == "X" || inputResponse == "x") // Go back to previous menu
    {
        return;
    }
    else // Invalid response given
    {
        cmdTextColour.cleanScreen();
        std::cout << "Please enter a valid response" << std::endl;
        queryAdd(thingsToDo, true);
    }
        
}

void queryMaster(std::vector<std::string> thingsToDo)
{
    // The main query loop
    
    printList(thingsToDo); // Prints a given vector in the form: elementOne \n elementTwo \n ...
    std::string inputResponse; // String used to hold result from user query for comparison and handeling
    // Printing options
    std::cout
            << std::endl
            << "[1]\t" << "Add a new item" << std::endl
            << "[2]\t" << "Remove an item" << std::endl
            << "[3]\t" << "Import from USB" << std::endl
            << "[4]\t" << "Export to USB" << std::endl
            << "[X]\t" << "Terminate Program" << std::endl
            << std::endl;
    std::cin >> inputResponse; // Take user input 
    // Compare input to options; execute as appropriate then (in most cases) call queryMaster() again [NOTE: Consider switch statement]
    if(inputResponse == "1") // Add an item
    {
        editAddItemToList(thingsToDo);
        queryMaster(thingsToDo);
    }
    else if(inputResponse == "2") // Remove an item
    {
        editRemoveItemFromList(thingsToDo);
        queryMaster(thingsToDo);
    }
    else if (inputResponse == "3") // Import from USB
    {
        //Import; show imported list and ask if local copy should be overwritten of to edit the external file
    }
    else if (inputResponse == "4") // Export to USB
    {
        //Export and check for overwriting
    }
    else if (inputResponse == "X" || inputResponse == "x") // Termination requested
    {
        terminateProgram();
    }
    else // Invalid response given
    {
        cmdTextColour.cleanScreen();
        std::cout << "Please enter a valid response" << std::endl << std::endl;
        queryMaster(thingsToDo);
    }
}

std::vector<std::string> initialLoad()
{
    // Initial Loading Procedure
    
    std::vector<std::string> thingsToDo; // Place holder vector storing the TODO list presently being used
    readLoadVectorFromStore(thingsToDo); // Assigning available store to vector (store is either thingsToDo.txt or thingsToDO_ExternalStore.txt if the stor is ona  aremovable disk)
    return thingsToDo;
}

int main()
{
    queryMaster(initialLoad()); // Initialising the query response using initialLoad as the storage vector
    return 0;
}
