#include "Environment.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// https://stackoverflow.com/a/313990/7300063
std::string toLowerCase(const std::string& string) {
    std::string str = string;
    std::transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}

// https://stackoverflow.com/a/17976083/7300063
bool isUnsignedInt(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), isdigit);
}

int main(int argc, char** argv) {

    std::string mapFilePath(argv[1]);
    std::string speciesFilePath(argv[2]);

    // Open ./input/map.txt and write its contents to mapStr
    std::vector<std::string> mapLines;
    std::string mapIn;
    std::ifstream mapFile(mapFilePath);
    if (mapFile.is_open()) {
        while (getline(mapFile, mapIn)) {
            mapLines.push_back(mapIn);
        }
        mapFile.close();
    } else {
        std::cerr << "Error opening " << mapFilePath << std::endl;
        return -1;
    }

    // Open ./input/species.txt and write its contents to speciesStr
    std::vector<std::string> speciesLines;
    std::string speciesIn;
    std::ifstream speciesFile(speciesFilePath);
    if (speciesFile.is_open()) {
        while (getline(speciesFile, speciesIn)) {
            speciesLines.push_back(speciesIn);
        }
        speciesFile.close();
    } else {
        std::cerr << "Error opening " << speciesFilePath << std::endl;
        return -2;
    }

    cs3210::Environment environment(mapLines, speciesLines);
    std::cout << "Initial environment:\n" << environment.toString() << "\n" << std::endl;

    bool saved = true;
    std::string rawInput;
    while (true) {
        std::cout << "Enter a command: ";
        getline(std::cin, rawInput);

        std::string input = toLowerCase(rawInput);
        std::string command = input.find(' ') != std::string::npos ? input.substr(0, input.find(' ')) : input;
        std::string args = rawInput.find(' ') != std::string::npos ? rawInput.substr(rawInput.find(' ') + 1) : "";

        // Quit
        if (command == "quit" || command == "q" ||  command == "-1") {
            if (!saved) {
                std::cerr << "Would you like to save before quiting?" << std::endl;
                std::string saveResponse;
                getline(std::cin, saveResponse);
                std::string saveCommand = toLowerCase(saveResponse.find(' ') != std::string::npos ? saveResponse.substr(0, saveResponse.find(' ')) : saveResponse);
                std::string saveArgs = saveResponse.find(' ') != std::string::npos ? saveResponse.substr(saveResponse.find(' ') + 1) : "";

                std::cout << "Save command: " << saveCommand << std::endl;
                std::cout << "Save args: " << saveArgs << std::endl;

                if (saveCommand == "yes" || saveCommand == "y") {
                    if (!saveArgs.empty()) {
                        mapFilePath = saveArgs;
                    }
                    std::ofstream saveMap(mapFilePath);
                    saveMap << environment.toString();
                    saveMap.close();
                    saved = true;
                }
            }
            break;

        // Save
        } else if (command == "write" || command == "w" || command == "save" || command == "s") {
            if (!args.empty()) {
                mapFilePath = args;
            }
            std::ofstream saveMap(mapFilePath);
            saveMap << environment.toString();
            saveMap.close();
            saved = true;

        // Save and quit
        } else if (command == "wq") {
            if (!args.empty()) {
                mapFilePath = args;
            }
            std::ofstream saveMap(mapFilePath);
            saveMap << environment.toString();
            saveMap.close();
            saved = true;
            break;

        // Iterate n times
        } else if (isUnsignedInt(input)) {
            environment.iterate(std::stoi(input));
            saved = false;

        // Iterate once
        } else if (input.empty()) {
            environment.iterate();
            saved = false;

        // Invalid command, quit
        } else {
            std::cerr << "Invalid command" << std::endl;
            continue;
        }

        std::cout << environment.toString() << "\n" << std::endl;
    }

    return 0;
}
