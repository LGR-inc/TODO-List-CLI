#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

void terminateProgram()
{
    std::cout << std::endl << "So you have decided to terminate the program." << std::endl
        << "Fair enough; go procrastinate, or fine something to do and come back." << std::endl << std::endl;
    std::system("timeout /t 5");
    std::system("exit");
}

void printList(std::vector<std::string> listToPrint)
{
    for(unsigned int k = 1; k < listToPrint.size(); k++)
    {
        std::cout << "[" << k << "]\t" << listToPrint[k-1] << std::endl;
    }
}

void edit_addItemToList(std::vector<std::string>& vectorToBeEdited)
{
    std::cout
            << "So you want to add an item to do; ok, what is it?" << std::endl
            << "State the item you wish to add:" << std::endl;
    std::string itemToBeAdded;
    std::getline(std::cin, itemToBeAdded);
    vectorToBeEdited.push_back(itemToBeAdded);
}

void edit_removeItemFromList(std::vector<std::string>& vectorToBeEdited)
{
    std::cout
            << "So you wish to remove an item from the list?" << std::endl
            << "State the number associated with the item you wish to remove:" << std::endl;
    std::string itemToBeRemoved;
    std::cin >> itemToBeRemoved;
    int itemToBeRemovedInt;
    std::stringstream convert(itemToBeRemoved);
    if(!(convert >> itemToBeRemovedInt))
    {
        system("cls");
        printList(vectorToBeEdited);
        std::cout << "Please enter a valid response" << std::endl;
        edit_removeItemFromList(vectorToBeEdited);
    }
    vectorToBeEdited.erase(vectorToBeEdited.begin() + (itemToBeRemovedInt - 1));
}

void read_loadVectorFromExternalStoreOnDemand(std::vector<std::string>& vectorToBeLoaded, std::string pathToStore)
{
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

void read_loadVectorFromStore(std::vector<std::string>& vectorToBeLoaded)
{
    std::ifstream storageFile("G:\\thingsToDo_ExternalStore.txt");
    if(!storageFile)
    {
        storageFile.close();
        std::ifstream storageFile("thingsToDo.txt");
         if(!storageFile) //Always test the file open.
        {
            std::cout
                << "Error: File does not exist in appropriate location." <<std::endl
                << "Generating blank storage file." << std::endl;
            storageFile.close();
            std::ofstream storageFile("thingsToDo.txt");
            storageFile.close();
            return;
        }
    }
    std::string line;
    while(std::getline(storageFile, line))
    {
        vectorToBeLoaded.push_back(line);
    }
    storageFile.close();
}

void write_addToLocalListStore(std::string vectorItemToBeStored)
{
    std::ofstream storageFile("thingsToDo.txt");
    storageFile << vectorItemToBeStored << "\n";
    storageFile.close();
}

void write_overwriteLocalListStore(std::vector<std::string> vectorToBeStored) // Writes the passed vector
{
    std::ofstream storageFile("thingsToDo.txt");
    storageFile.clear();
    for(unsigned int limit = 0 ; limit < vectorToBeStored.size() ; limit++)
    {
        write_addToLocalListStore(vectorToBeStored[limit]);
    }
    storageFile.close();
}

void write_specificLocationLocalListStore(std::string vectorItemToBeStored, int lineNumber)
{
    std::ofstream storageFile("thingsToDo.txt");
    storageFile.seekp(lineNumber);
    write_addToLocalListStore(vectorItemToBeStored);
    storageFile.close();
}

void query(std::vector<std::string> thingsToDo) // The main query loop
{
    printList(thingsToDo);
    std::string input_Response;
    std::string terminationPosition = "Null";
    std::cout
            << std::endl
            << "[1]\t" << "Add a new item" << std::endl
            << "[2]\t" << "Remove an item" << std::endl
            << "[3]\t" << "Import from USB" << std::endl
            << "[4]\t" << "Export to USB" << std::endl
            << "[5]\t" << "Terminate Program" << std::endl
            << std::endl;
    terminationPosition = "5";
    std::cin >> input_Response;
    if(input_Response == "1") // Add an item
    {
        edit_addItemToList(thingsToDo);
        query(thingsToDo);
    }
    else if(input_Response == "2") // Remove an item
    {
        edit_removeItemFromList(thingsToDo);
        query(thingsToDo);
    }
    else if (input_Response == "3") // Import from USB
    {
        //Import; show imported list and ask if local copy should be overwritten of to edit the external file
    }
    else if (input_Response == "4") // Export to USB
    {
        //Export and check for overwriting
    }
    else if (input_Response == terminationPosition) // Termination requested
    {
        terminateProgram();
    }
    else // Invalid response given
    {
        system("cls");
        std::cout << "Please enter a valid response" << std::endl << std::endl;
        query(thingsToDo);
    }
}

std::vector<std::string> initialLoad() // Initial Loading Procedure
{
    std::vector<std::string> thingsToDo;
    read_loadVectorFromStore(thingsToDo);
    return thingsToDo;
}

int main()
{
    query(initialLoad()); // Initialising the query response using initialLoad as the storage vector
    return 0;
}
