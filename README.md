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
2. Compile and run the `tester.cpp` file.

```bash
g++ tester.cpp -o file_system
./file_system
```
Ensure `sample.dat` is present in the same directory. This file loads the initial directory structure of the simulated file system.

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
