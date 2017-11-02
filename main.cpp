#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

std::vector<std::string> thingsToDo;

bool loadList()
{
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
        system("cls");
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
                << "Please enter a valid response" << std::endl
                << "[1]" << "Add a thing to do" << std::endl
                << "[2]" << "Remove an item" << std::endl
                << "[3]" << "Export to USB" << std::endl
                << "[4]" << "Terminate program; get on with life" << std::endl
                << "Type the appropriate number for the option you wish to pick" << std::endl;
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
    loadingQuery();
    writeListWhole(thingsToDo);
}
