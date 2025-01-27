#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
using namespace std;
namespace fs = filesystem; 

enum validCommands{
    echo,
    exit0,
    invalid,
    type,
    clear,
    cls,
    ls,
    cd,
    touch,
    del,
    rm,
    find0,
    getDate,
    mkdir,
    md,
    rmdir,
    rd,
    rename0
};

validCommands isValid(string command){
    command = command.substr(0,command.find(' '));
    if(command == "echo"){return validCommands::echo;}
    if(command == "exit"){return validCommands::exit0;}
    if(command == "type"){return validCommands::type;}
    if(command == "cls"){return validCommands::cls;}
    if(command == "clear"){return validCommands::clear;}
    if(command == "ls"){return validCommands::ls;}
    if(command == "cd"){return validCommands::cd;}
    if(command == "touch"){return validCommands::touch;}
    if(command == "del"){return validCommands::del;}
    if(command == "rm"){return validCommands::rm;}
    if(command == "find"){return validCommands::find0;}
    if(command == "get-date"){return validCommands::getDate;}
    if(command == "mkdir"){return validCommands::mkdir;}
    if(command == "md"){return validCommands::md;}
    if(command == "rmdir"){return validCommands::rmdir;}
    if(command == "rd"){return validCommands::rd;}
    if(command == "rename"){return validCommands::rename0;}
    return validCommands::invalid;
}

bool isValidType(string command){
    if(isValid(command) == echo){return true;} 
    if(isValid(command) == exit0){return true;}
    if(isValid(command) == type){return true;}
    if(isValid(command) == cls){return true;}
    if(isValid(command) == clear){return true;}
    if(isValid(command) == cd){return true;}
    if(isValid(command) == touch){return true;}
    if(isValid(command) == del){return true;}
    if(isValid(command) == rm){return true;}
    if(isValid(command) == find0){return true;}
    if(isValid(command) == getDate){return true;}
    if(isValid(command) == mkdir){return true;}
    if(isValid(command) == md){return true;}
    if(isValid(command) == rmdir){return true;}
    if(isValid(command) == rd){return true;}
    if(isValid(command) == rename0){return true;}
    return false;
} 

void listDirectoryContent(){
    const string dirPath = ".";

    try{
        // checking if the path is valid
        if(!fs::is_directory(dirPath)){
            cerr << "Error" << ": directory not found" << endl;
            return;
        }

        // iterating through the current directory

        cout << left << setw(30) << "Name"
             << setw(15) << "Type"
             << "Size (Byte)" << endl;
        cout << string(60, '-') << endl;
        for(const auto& entry : fs::directory_iterator(dirPath)){
            string name = entry.path().filename().string();
            string type = entry.is_directory() ? "Directory":"File";
            uintmax_t size = entry.is_regular_file() ? fs::file_size(entry):0;
            
            cout << left << setw(30) << name
                 << setw(15) << type
                 << size << endl;
        }
    }
    catch(const fs::filesystem_error& e){
        cerr << "Error accessing directory: " << e.what() << endl;
    }
}

void getCurrentPath(){
    string currentPath = fs::current_path().string();
    try{    
        cout << currentPath;  
    }
    catch (const fs::filesystem_error& e){
        cerr << "Error accessing path: " << e.what();
    }
}

void changeDirectory(string newPath){
    try{
        fs::current_path(newPath);
    }
    catch (const fs::filesystem_error& e){
        cerr << "Error: " << e.what() << endl;
    }
}

void createFile(string fileName){
    try{
        if(fs::exists(fileName)){
            cerr << "Error: " << fileName << " already exists" << endl;
        }
        else{
            ofstream file(fileName);
            cout << fileName << ": created successfully" << endl;
        }
    }
    catch (const exception& e){
        cerr << "Error: " << e.what() << endl;
    }
}

void removeFileDir(string fileDirName){
    if(fs::exists(fileDirName)){
        fs::remove(fileDirName);
        cout << fileDirName << " removed successfully" << endl;
    }
    else{
        cerr << "Error: " << fileDirName << " does not exists" << endl;
    }
}

void checkFileOrDirExists(string fileOrDir){
    if(fs::exists(fileOrDir)){
        cout << fileOrDir << ": found" << endl;
    }
    else{
        cerr << "Error: " << fileOrDir << " not found" << endl;
    }
}

void getDateTime(){
    time_t timestamp;
    time(&timestamp);
    cout << endl;
    cout << ctime(&timestamp) << endl;
}

void createDir(string dirName){
    try{
        if(fs::exists(dirName)){
            cerr << "Error: " << dirName << " already exists" << endl;
        }
        else{
            fs::create_directory(dirName);
            cout << dirName << ": created successfully" << endl;
        }
    }
    catch (const fs::filesystem_error& e){
        cerr << "Error: " << e.what() << endl;
    }
}

void renameFileDir(string& input) {
    input = input.erase(0, input.find(' ')+1);
    string oldName = input.substr(0, input.find('^'));
    string newName = input.substr(input.find('^')+1);
    try {
        if (fs::exists(oldName)) {
            if (fs::exists(newName)) {
                cerr << "Error: '" << newName << "' already exists." << endl;
                return;
            } else {
                fs::rename(oldName, newName);
                cout << "'" << oldName << "' renamed to '" << newName << "' successfully." << endl;
            }
        } else {
            cerr << "Error: '" << oldName << "' does not exist." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main(){
    cout << "LUCE SHELL" << endl;
    cout << "Copyright (C) Vedant Visoliya. All rights reserved." << endl;
    // varibles
    bool done = false;
    string input;
    string newPath;
    // looping

    do{
        getCurrentPath();
        cout << "$ ";
        getline(cin, input);

        switch (isValid(input)){
            case exit0:
                done = true;
                break;
            case echo:
                input = input.substr(input.find(" ")+1, input.length());
                cout << input << endl;
                break;
            case type:
                input = input.substr(input.find(" ")+1, input.length());
                if(isValidType(input))
                {
                    cout << input << " is a builtin command" << endl;
                }
                else{
                    cout << input << ": command not found" << endl;
                }
                break;
            case clear:
                cout << "\033[2J\033[H" << endl;
                break;
            case cls:
                cout << "\033[2J\033[H" << endl;
                break;
            case ls:
                listDirectoryContent();
                break;
            case cd:
                newPath = fs::current_path().string();
                input = input.substr(input.find(" ")+1, input.length());
                if(input == ".."){
                    changeDirectory(input);
                }
                else{
                    newPath = newPath + "\\" + input;
                    changeDirectory(newPath);
                } 
                break;
            case touch:
                input = input.substr(input.find(" ")+1, input.length());
                createFile(input);
                break;
            case del:
                input = input.substr(input.find(" ")+1, input.length());
                removeFileDir(input);
                break;
            case rm:
                input = input.substr(input.find(" ")+1, input.length());
                removeFileDir(input);
                break;
            case find0:
                input = input.substr(input.find(" ")+1, input.length());
                checkFileOrDirExists(input);
                break;
            case getDate:
                getDateTime();
                break;
            case mkdir:
                input = input.substr(input.find(" ")+1, input.length());
                createDir(input);
                break;
            case md:
                input = input.substr(input.find(" ")+1, input.length());
                createDir(input);
                break;
            case rmdir:              
                input = input.substr(input.find(" ")+1, input.length());
                removeFileDir(input);
                break;
            case rd:
                input = input.substr(input.find(" ")+1, input.length());
                removeFileDir(input);
                break;
            case rename0:
                renameFileDir(input);
                break;
            default:
                input = input.substr(0, input.find(" "));
                cout << input << ": command not found" << endl;
                break;
        }

    }while(!done);
    return 0;
}