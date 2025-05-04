#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "Directory.h"

using namespace std;

class FileSystem {
    private:
        Directory root; // Root directory of the file system
        Directory* currentDir; // Pointer to the current working directory
        vector<string> path;      // Tracks the current directory path (relative to root)
    
        // Help map for command descriptions
        vector<pair<string, string>> helpMap = {
            {"create", "01. create <filename>                    - Create a new file in current directory"},
            {"delete", "02. delete <filename>                    - Delete a file from current directory"},
            {"mkdir", "03. mkdir <dirname>                      - Create a new directory"},
            {"chdir", "04. chdir <dirname>                      - Change to specified directory (use '..' to go up)"},
            {"ls", "05. ls                                   - Lists all files and directories in the current directory"},
            {"move", "06. move <source> <target>               - Rename/move a file"},
            {"open", "07. open <filename>                      - Open a file for writing"},
            {"close", "08. close <filename>                     - Close an opened file"},
            {"write", "09. write <filename> <text>              - Write text at the end of file"},
            {"write_at", "10. write_at <filename> <pos> <text>     - Write text at specific position"},
            {"read", "11. read <filename>                      - Read entire file content"},
            {"read_from", "12. read_from <filename> <start> <size>  - Read part of file"},
            {"move_within", "13. move_within <filename> <start> <size> <target> - Move internal file data"},
            {"truncate", "14. truncate <filename> <size>           - Cut file size to specified length"},
            {"memory_map", "15. memory_map                           - Show current directory and files tree"},
            {"help", "16. help                                 - To show work of available commands"},
            {"exit", "17. exit                                 - Exit the program"}
        };
    public:
    FileSystem() : root("root", nullptr), currentDir(&root) {
        // Initialize the path as empty, meaning we're at the root
        path.push_back(""); // Root is the starting point but not shown in the path
    }
    
        // Function to display the current path (excluding root)
        void displayPath() {
            if (path.size() == 1 && path[0] == "") {
                cout << "> "; // Just root, so no path
                return;
            }
        
            for (size_t i = 1; i < path.size(); ++i) { // Skip the root marker ""
                if (i > 1) cout << ">";
                cout << path[i];
            }
            cout << "> ";
        }
        
    
    
        void showHelp() {
            cout << "Available Commands:\n";
            for (const auto& entry : helpMap) {
                cout << entry.second << endl;
            }
        }
    
        // Function to show specific help
        void showSpecificHelp(const string& command) {
            for (const auto& entry : helpMap) {
                if (entry.first == command) {
                    cout << entry.second << endl;
                    return;
                }
            }
            cout << "Unknown command. Use 'help' to see the list of available commands.\n";
        }
        
    
        void createFile(const string& filename) {
            if (currentDir->files.find(filename) == currentDir->files.end()) {
                currentDir->files[filename] = File(filename); // Create a new file in the current directory
                cout << "File created: " << filename << endl;
            } else {
                cout << "File already exists.\n"; // File with the same name already exists
            }
        }
    
        void deleteFile(const string& filename) {
            if (currentDir->files.erase(filename)) {
                cout << "File deleted: " << filename << endl; // Delete the file if it exists
            } else {
                cout << "File not found.\n"; // File not found in the current directory
            }
        }
    
        void mkdir(const string& dname) {
            if (dname.empty()) {
                cout << "Directory name cannot be empty.\n";
                return;
            }
            if (dname == "root") {
                cout << "Cannot create another 'root' directory.\n";
                return;
            }
            if (currentDir->subdirectories.find(dname) != currentDir->subdirectories.end()) {
                cout << "Directory already exists.\n";
                return;
            }
            Directory newDir;
            newDir.name = dname;
            newDir.parent = currentDir;
            currentDir->subdirectories[dname] = newDir;
            cout << "Directory created: " << dname << endl;
        }
        
    
        void chDir(const string& dirname) {
            if (dirname == "..") {
                if (currentDir->parent != nullptr) {
                    currentDir = currentDir->parent;  // Move to the parent directory
                    path.pop_back();  // Remove the last directory from the path
                } else {
                    cout << "Already at root directory.\n";  // Already at the root directory
                }
            } else if (currentDir->subdirectories.find(dirname) != currentDir->subdirectories.end()) {
                currentDir = &currentDir->subdirectories[dirname];  // Change to the specified subdirectory
                path.push_back(dirname);  // Add the subdirectory name to the path
            } else {
                cout << "Directory not found.\n";  // Subdirectory not found
            }
        }
        
        void listFiles() {
            if (currentDir->files.empty() && currentDir->subdirectories.empty()) {
                cout << "Directory is empty.\n";
            } else {
                cout << "\nContents of directory '" << currentDir->name << "':\n";
        
                // List subdirectories
                for (const auto& dirEntry : currentDir->subdirectories) {
                    cout << "📁  " << dirEntry.second.name << endl;
                }
        
                // List files
                for (const auto& fileEntry : currentDir->files) {
                    cout << "📄  " << fileEntry.second.name << endl;
                }
            }
        }
        
    
        void moveFile(const string& source, const string& target) {
            if (currentDir->files.find(source) != currentDir->files.end()) {
                File f = currentDir->files[source]; // Get the source file
                f.name = target; // Rename the file
                currentDir->files[target] = f; // Add the renamed file to the map
                currentDir->files.erase(source); // Remove the original file
                cout << "Moved file: " << source << " -> " << target << endl;
            } else {
                cout << "Source file not found.\n"; // Source file not found
            }
        }
    
        File* openFile(const string& filename) {
            if (currentDir->files.find(filename) != currentDir->files.end()) {
                File* file = &currentDir->files[filename];
                if (file->is_open) {
                    cout << "Error: File is already open.\n";
                } else {
                    file->is_open = true;
                }
                return file; // Return a pointer to the file
            } else {
                cout << "File not found.\n"; // File not found
                return nullptr;
            }
        }
    
        void closeFile(const string& filename) {
            if (currentDir->files.find(filename) != currentDir->files.end()) {
                currentDir->files[filename].is_open = false; // Mark the file as closed
                cout << "File closed.\n";
            } else {
                cout << "File not found.\n"; // File not found
            }
        }
    
        void showMemoryMap(Directory* dir = nullptr, int depth = 0) {
            if (dir == nullptr) dir = &root; // Start from the root directory if no directory is specified
            for (auto& d : dir->subdirectories) {
                for (int i = 0; i < depth; i++) cout << "  "; // Indent based on depth
                cout << "📁 " << d.first << endl; // Print directory name
                showMemoryMap(&d.second, depth + 1); // Recursively show subdirectories
            }
            for (auto& f : dir->files) {
                for (int i = 0; i < depth; i++) cout << "  "; // Indent based on depth
                cout << "📄 " << f.first << endl; // Print file name
            }
        }
    
        // Save the file system
        void saveToFile(const string& filename) {
            ofstream fout(filename);
            if (!fout) {
                cout << "Failed to save.\n";
                return;
            }
        
            // Save files in root
            for (auto& f : root.files) {
                fout << "FILE " << f.second.name << " " << f.second.content << endl;
            }
        
            // Save subdirectories in root
            for (auto& d : root.subdirectories) {
                saveDir(fout, d.second);
            }
        
            fout.close();
        }    
        
    
        void saveDir(ofstream& fout, Directory& dir) {
            fout << "DIR " << dir.name << endl; // Write directory name
            for (auto& f : dir.files) {
                fout << "FILE " << f.second.name << " " << f.second.content << endl; // Write file name and content
            }
            for (auto& d : dir.subdirectories) {
                saveDir(fout, d.second); // Recursively save subdirectories
            }
            fout << "ENDDIR\n"; // Mark the end of the directory
        }
    
        // Load the file system
        void loadFromFile(const string& filename) {
            ifstream fin(filename);
            if (!fin) {
                cout << "No save file found. Starting new filesystem.\n"; // Handle missing save file
                return;
            }
            root = Directory("root", nullptr); // Reset the root directory
            currentDir = &root; // Reset the current directory
            loadDir(fin, &root); // Load the root directory and its contents
            fin.close();
        }
    
        void loadDir(ifstream& fin, Directory* dir) {
            string type, name, content;
            while (fin >> type) {
                if (type == "DIR") {
                    fin >> name;
                    dir->subdirectories[name] = Directory(name, dir); // Create a new subdirectory
                    loadDir(fin, &dir->subdirectories[name]); // Recursively load subdirectories
                } else if (type == "FILE") {
                    fin >> name;
                    getline(fin, content);
                    if (!content.empty() && content[0] == ' ') content = content.substr(1); // Remove leading space
                    dir->files[name] = File(name); // Create a new file
                    dir->files[name].content = content; // Set file content
                } else if (type == "ENDDIR") {
                    break; // End of the current directory
                }
            }
        }
    };