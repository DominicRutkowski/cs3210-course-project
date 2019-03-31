#include <fstream>
#include <iostream>
#include <string>

int main() {
    // Open ./input/map.txt and write its contents to mapStr
    std::string mapStr;
    std::string mapIn;
    std::fstream mapFile("../input/map.txt");
    if (mapFile.is_open()) {
        while (getline(mapFile, mapIn)) {
            mapStr += mapIn + "\n";
        }
        mapFile.close();
    } else {
        std::cerr << "Error opening ./input/map.txt" << std::endl;
        return -1;
    }

    // Open ./input/species.txt and write its contents to speciesStr
    std::string speciesStr;
    std::string speciesIn;
    std::fstream speciesFile("../input/species.txt");
    if (speciesFile.is_open()) {
        while (getline(speciesFile, speciesIn)) {
            speciesStr += speciesIn + "\n";
        }
        speciesFile.close();
    } else {
        std::cerr << "Error opening ./input/species.txt" << std::endl;
        return -2;
    }

    return 0;
}
