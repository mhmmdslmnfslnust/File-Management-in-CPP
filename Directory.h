#pragma once

#include <iostream>
#include <map>
#include "File.h"
using namespace std;


class Directory {
    public:
        string name; // Name of the directory
        map<string, File> files; // Map of files in the directory
        map<string, Directory> subdirectories; // Map of subdirectories
        Directory* parent; // Pointer to the parent directory
    
        Directory(string name = "", Directory* parent = nullptr) : name(name), parent(parent) {} // Constructor with default name as "" and parent as nullptr
};