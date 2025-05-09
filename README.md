# File-Management-in-C++
This project is a **C++-based command-line file system simulator**. It mimics a basic file system with commands like creating files and directories, reading/writing files, navigating between directories, and saving/loading persistent structure.

---

## 📦 Getting Started

### ✅ Requirements
- C++17 or later
- Standard C++ libraries

---

## 🚀 Running the Project

1. Clone or download this repository.
2. Compile and run the `main.cpp` file.

```bash
g++ main.cpp -o modular_file_system
./modular_file_system
```
Ensure `sample.dat`, `File.h`,`Directory.h`,`FileSystem.h`and `CommandUtils.h` are present in the same directory. `sample.dat` file loads the initial directory structure of the simulated file system. While other four files are modular necessary for this system to work.

---

## 📂 About `sample.dat`

The `sample.dat` file defines the initial file system layout using a structured format.

- Directories begin with `DIR <dirname>` and end with `ENDDIR`.
- Files are listed as `FILE <filename> <content>` within their parent directory block.
- This format supports **nesting**, so directories can contain subdirectories and files.

### 🔍 Example Format

```plaintext
DIR root
FILE file1.txt HelloWorld
DIR documents
FILE notes.txt Meeting notes
ENDDIR
ENDDIR
```
You can edit `sample.dat` to define your own initial layout.

To load a different `.dat` file, simply modify the filename in the source code where the file system loads its data.

---

## 💻 CLI Commands

This system supports a comprehensive set of command-line instructions:

| Command                                     | Description                                      |
|---------------------------------------------|--------------------------------------------------|
| `create <filename>`                         | Create a new file in the current directory       |
| `delete <filename>`                         | Delete a file from the current directory         |
| `mkdir <dirname>`                           | Create a new subdirectory                        |
| `chdir <dirname>`                           | Change to a subdirectory (`..` to go up)         |
| `ls`                                        | List all files and subdirectories                |
| `move <source> <target>`                    | Rename or move a file                            |
| `open <filename>`                           | Open a file for writing                          |
| `close <filename>`                          | Close an opened file                             |
| `write <filename> <text>`                   | Append text to a file                            |
| `write_at <filename> <pos> <text>`          | Insert text at a specific position               |
| `read <filename>`                           | Read the full content of a file                  |
| `read_from <filename> <start> <size>`       | Read a portion of a file                         |
| `move_within <filename> <start> <size> <target>` | Move content inside a file                    |
| `truncate <filename> <size>`                | Cut the file down to a given size                |
| `memory_map`                                | Show the full tree of current directory structure|
| `help`                                      | Show all available commands                      |
| `exit`                                      | Save the file system and exit the program        |

---

## 📄 File Saving & Loading

- On exit, the system automatically saves the current directory structure and file contents to `sample.dat`, preserving all changes for future sessions.

- You can use a different file by modifying the filename in the source code.

---

## 📚 Documentation

- **`UserGuide.pdf`** – Provides a detailed explanation and usage examples for all commands.

---

## 📦 Code Structure
### `File.h`
```cpp
#pragma once
class File { ... };
```
### `Directory.h`
```cpp
#pragma once
#include "File.h"
class Directory { ... };
```
### `FileSystem`
```cpp
#pragma once
#include "Directory.h"
class FileSystem { ... };
```

### `CommandUtil.h`
```cpp
#pragma once
int levenshtein(...) {...};
void suggestCommand(...) {...};
vector<string> commands = {...};
```

### `main.cpp`
```cpp
#include "FileSystem.h"
#include "CommandUtils.h"

int main() {...}
```

