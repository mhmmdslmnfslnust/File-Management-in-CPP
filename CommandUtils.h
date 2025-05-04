#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


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