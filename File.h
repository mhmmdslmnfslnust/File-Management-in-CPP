#pragma once

using namespace std; // Use standard namespace
#include <iostream> // Include iostream for input/output operations


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