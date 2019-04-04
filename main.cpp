#include "Environment.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // Open ./input/map.txt and write its contents to mapStr
    std::vector<std::string> mapLines;
    std::string mapIn;
    std::fstream mapFile("../input/map.txt");
    if (mapFile.is_open()) {
        while (getline(mapFile, mapIn)) {
            mapLines.push_back(mapIn);
        }
        mapFile.close();
    } else {
        std::cerr << "Error opening ./input/map.txt" << std::endl;
        return -1;
    }

    // Open ./input/species.txt and write its contents to speciesStr
    std::vector<std::string> speciesLines;
    std::string speciesIn;
    std::fstream speciesFile("../input/species.txt");
    if (speciesFile.is_open()) {
        while (getline(speciesFile, speciesIn)) {
            speciesLines.push_back(speciesIn);
        }
        speciesFile.close();
    } else {
        std::cerr << "Error opening ./input/species.txt" << std::endl;
        return -2;
    }

    cs3210::Environment environment(mapLines, speciesLines);

    return 0;
}
