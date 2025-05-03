#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class File {
public:
    string name; // Name of the file
    string content; // Content of the file
    bool is_open; // Flag to check if the file is open

    File(string name = "") : name(name), is_open(false) {} // Constructor to initialize file

    void write_to_file(const string& text) {
        content += text; // Append text to the file content
    }

    void write_at(int pos, const string& text) {
        if (pos >= 0 && pos <= content.size()) {
            content.replace(pos, text.size(), text); // Replace as normal
        } else if (pos > content.size()) {
            content += string(pos - content.size(), ' '); // Pad with spaces
            content += text; // Append text after padding
        }
    }
    

    string read_from_file() {
        return content; // Return the entire file content
    }

    string read_from(int start, int size) {
        // Check for invalid start position
        if (start < 0 || start >= content.size()) {
            cerr << "Error: Start position out of bounds." << endl;
            return "";
        }
    
        // If requested size goes beyond content, adjust it
        if (start + size > content.size()) {
            cerr << "Warning: Requested size exceeds file content. Truncating read." << endl;
            size = content.size() - start;
        }
    
        // Return the valid substring
        return content.substr(start, size);
    }
    

    void move_within_file(int start, int size, int target) {
        if (start >= 0 && start + size <= content.size() && target >= 0 && target <= content.size()) {
            string movingText = content.substr(start, size); // Extract the text to move
            content.erase(start, size); // Remove the text from the original position
            content.insert(target, movingText); // Insert the text at the target position
        } else if (start < 0 || start + size > content.size()) {
            cerr << "Error: Start position or size out of bounds." << endl; // Handle out of bounds error
        } else if (target < 0 || target > content.size()) {
            cerr << "Error: Target position out of bounds." << endl; // Handle out of bounds error
        }
    }

    void truncate_file(int maxSize) {
        if (maxSize >= 0 && maxSize < content.size()) {
            content = content.substr(0, maxSize); // Truncate the file content to the specified size
        } else if (maxSize < 0) {
            cerr << "Error: Size cannot be negative." << endl; // Handle negative size error
        } else {
            cerr << "Warning: Size exceeds current content. No truncation performed." << endl; // Handle size exceeding content
        }
    }
};

class Directory {
public:
    string name; // Name of the directory
    map<string, File> files; // Map of files in the directory
    map<string, Directory> subdirectories; // Map of subdirectories
    Directory* parent; // Pointer to the parent directory

    Directory(string name = "", Directory* parent = nullptr) : name(name), parent(parent) {} // Constructor with default name as "" and parent as nullptr
};

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

// Function to calculate Levenshtein distance
int levenshtein(const string &s1, const string &s2) {
    int len1 = s1.length(), len2 = s2.length();
    vector<vector<int>> dist(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++) dist[i][0] = i;
    for (int j = 0; j <= len2; j++) dist[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            dist[i][j] = min({dist[i - 1][j] + 1, dist[i][j - 1] + 1, dist[i - 1][j - 1] + cost});
        }
    }

    return dist[len1][len2];
}

vector<string> commands = {
    "create <filename>",
    "delete <filename>",
    "mkdir <dirname>",
    "chdir <dirname>",
    "ls",
    "move <source> <target>",
    "open <filename>",
    "close <filename>",
    "write <filename> <text>",
    "write_at <filename> <pos> <text>",
    "read <filename>",
    "read_from <filename> <start> <size>",
    "move_within <filename> <start> <size> <target>",
    "truncate <filename> <size>",
    "memory_map",
    "help",
    "exit"
};

void suggestCommand(const string& userCommand) {
    int minDist = INT_MAX;
    string closestCommand;

    // Check each command and calculate Levenshtein distance
    for (const string& command : commands) {
        int dist = levenshtein(userCommand, command);
        if (dist < minDist) {
            minDist = dist;
            closestCommand = command;
        }
    }

    // Suggest the closest command
    if (minDist > 3) {  // Only suggest if the command is reasonably close (optional threshold)
        cout << "Unknown command. No similar command found.\n";
    } else {
        cout << "Did you mean: '" << closestCommand << "'?\n";
    }
}

void menu() {
    cout << "\nCommands:\n"
         << "01. create <filename>\n"
         << "02. delete <filename>\n"
         << "03. mkdir <dirname>\n"
         << "04. chdir <dirname>\n"
         << "05. move <source> <target>\n"
         << "06. open <filename>\n"
         << "07. close <filename>\n"
         << "08. write <filename> <text>\n"
         << "09. write_at <filename> <pos> <text>\n"
         << "10. read <filename>\n"
         << "11. read_from <filename> <start> <size>\n"
         << "12. move_within <filename> <start> <size> <target>\n"
         << "13. truncate <filename> <size>\n"
         << "14. memory_map\n"
         << "15. help\n"
         << "16. ls\n"
         << "17. exit\n";
}

int main() {
    FileSystem fs;  // Create a FileSystem object
    fs.loadFromFile("dil.dat");  // Load file system data from "dil.dat"
    menu();  // Display menu of available commands

    while (true) {  // Main command loop
        fs.displayPath();            // Shows path like: main>submain>
        string cmd;
        cin >> cmd;  // Read user command

        // Command processing block
        if (cmd == "create") {
            string fname;
            cin >> fname;
            fs.createFile(fname);  // Create new file
        } else if (cmd == "delete") {
            string fname;
            cin >> fname;
            fs.deleteFile(fname);  // Delete specified file
        } else if (cmd == "help") {
            string specificCmd;
            getline(cin, specificCmd);  // Read the rest of the line after "help"
        
            // Trim leading spaces (if any)
            if (!specificCmd.empty() && specificCmd[0] == ' ') {
                specificCmd = specificCmd.substr(1);
            }
        
            if (specificCmd.empty()) {
                fs.showHelp();  // Show general help if no specific command is provided
            } else {
                fs.showSpecificHelp(specificCmd);  // Show help for the specific command
            }
        } else if (cmd == "mkdir") {
            string dname;
            cin >> dname;  
            fs.mkdir(dname);  // Create new directory
        } else if (cmd == "chdir") {
            string dname;
            cin >> dname;  
            fs.chDir(dname);  // Change current directory
        } else if (cmd == "ls") {
            fs.listFiles();  // Lists all files and directories in the current directory
        } else if (cmd == "move") {
            string src, tgt;
            cin >> src >> tgt;  // Read source and target paths
            fs.moveFile(src, tgt);  // Move file from source to target
        } else if (cmd == "open") {
            string fname;
            cin >> fname;           
            fs.openFile(fname);  // Open specified file
        } else if (cmd == "close") {
            string fname;
            cin >> fname;            
            fs.closeFile(fname);  // Close specified file
        } else if (cmd == "write") {
            string fname, text;
            cin >> fname;  
            getline(cin, text);  // Read text to write (allows spaces)
            if (text[0] == ' ') text = text.substr(1);  // Remove leading space if present
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) file->write_to_file(text);  // Write text if file exists
        } else if (cmd == "write_at") {
            string fname;
            int pos;
            string text;
            cin >> fname >> pos;  // Read filename and position
            getline(cin, text);  // Read text to write
            if (text[0] == ' ') text = text.substr(1);
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) file->write_at(pos, text);  // Write at position if file exists
        } else if (cmd == "read") {
            string fname;
            cin >> fname;  // Read filename
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) cout << file->read_from_file() << endl;  // Output file contents
        } else if (cmd == "read_from") {
            string fname;
            int start, size;
            cin >> fname >> start >> size;  // Read filename, start position, and size
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) cout << file->read_from(start, size) << endl;  // Output portion of file
        } else if (cmd == "move_within") {
            string fname;
            int start, size, target;
            cin >> fname >> start >> size >> target;  // Read filename and positions
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) file->move_within_file(start, size, target);  // Move data within file
        } else if (cmd == "truncate") {
            string fname;
            int size;
            cin >> fname >> size;  // Read filename and new size
            File* file = fs.openFile(fname);  // Get file pointer
            if (file) file->truncate_file(size);  // Truncate file to specified size
        } else if (cmd == "memory_map") {
            fs.showMemoryMap();  // Display memory map of file system
        } else if (cmd == "exit") {
            fs.saveToFile("dil.dat");  // Save file system state
            cout << "File system saved. Exiting...\n";
            break;  // Exit command loop
        } else {
            suggestCommand(cmd);  // Handle invalid commands by suggesting similar commands
        }
    }

    return 0;  // End of program
}