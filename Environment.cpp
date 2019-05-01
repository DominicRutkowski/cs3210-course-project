//
// Created by Dominic Rutkowski on 2019-03-30.
//

#include "Environment.hpp"

namespace cs3210 {

    Environment::Environment(const std::vector<std::string>& mapLines, const std::vector<std::string>& speciesLines):
    grid{std::vector<std::vector<std::shared_ptr<Unit>>>(mapLines.size())} {
        for (int i = 0; i < mapLines.size(); ++i) {
            grid[i] = std::vector<std::shared_ptr<Unit>>(mapLines[i].size());
            for (int j = 0; j < mapLines[i].size(); ++j) {
                grid[i][j] = std::move(parseUnit(mapLines[i][j], speciesLines));
            }
        }
    }

    void Environment::iterate(unsigned int iterations) {
        for (int i = 0; i < iterations; ++i) {
            // Iterate plants
            for (auto& row : grid) {
                for (auto& unit : row) {
                    if (unit->getUnitType() == UnitType::VIABLE_UNIT) {
                        auto& viableUnit = dynamic_cast<ViableUnit&>(*unit);
                        if (viableUnit.getPlant() != nullptr) {
                            viableUnit.iteratePlant();
                        }
                    }
                }
            }
            iterateAnimals(AnimalType::HERBIVORE);
            iterateAnimals(AnimalType::OMNIVORE);
            // Mark animals as ready to be iterated (movable) during the next iteration
            for (auto& row : grid) {
                for (auto& unit : row) {
                    if (unit->getUnitType() == UnitType::VIABLE_UNIT) {
                        auto& viableUnit = dynamic_cast<ViableUnit&>(*unit);
                        auto& animal = viableUnit.getAnimal();
                        if (animal != nullptr) {
                            animal->setIterated(false);
                        }
                    }
                }
            }
        }
    }

    void Environment::iterateAnimals(AnimalType animalType) {
        for (int j = 0; j < grid.size(); ++j) {
            for (int k = 0; k < grid[j].size(); ++k) {
                if (getUnit(j, k)->getUnitType() == UnitType::VIABLE_UNIT) {
                    std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(getUnit(j, k));
                    auto& animal = viableUnit->getAnimal();
                    if (animal != nullptr &&
                        animal->getAnimalType() == animalType &&
                        !animal->hasIterated()) {

                        // Initialize surrounding units
                        std::shared_ptr<Unit> top = getUnit(j - 2, k);
                        std::shared_ptr<Unit> topLeft = getUnit(j - 1, k - 1);
                        std::shared_ptr<Unit> topCenter = getUnit(j - 1, k);
                        std::shared_ptr<Unit> topRight = getUnit(j - 1, k + 1);
                        std::shared_ptr<Unit> left = getUnit(j, k - 2);
                        std::shared_ptr<Unit> leftCenter = getUnit(j, k - 1);
                        std::shared_ptr<Unit> rightCenter = getUnit(j, k + 1);
                        std::shared_ptr<Unit> right = getUnit(j, k + 2);
                        std::shared_ptr<Unit> bottomLeft = getUnit(j + 1, k - 1);
                        std::shared_ptr<Unit> bottomCenter = getUnit(j + 1, k);
                        std::shared_ptr<Unit> bottomRight = getUnit(j + 1, k + 1);
                        std::shared_ptr<Unit> bottom = getUnit(j + 2, k);

                        animal->setIterated();
                    }
                }
            }
        }
    }

    std::string Environment::toString() const {
        std::string result;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                result += grid[i][j]->toString();
            }
            if (i < grid.size() - 1) {
                result += "\n";
            }
        }
        return result;
    }

    std::shared_ptr<Unit> Environment::parseUnit(char ch, const std::vector<std::string>& speciesLines) const {
        for (auto& speciesDefinition : speciesLines) {
            unsigned int typeCharIndex = speciesDefinition.find(' ') + 1;
            char typeChar = speciesDefinition.at(typeCharIndex);
            if (ch == typeChar) { // Occupied space
                std::unique_ptr<ViableUnit> unit(new ViableUnit());

                std::string organismClassification = speciesDefinition.substr(0, typeCharIndex - 1);
                if (organismClassification == "plant") {
                    int div = speciesDefinition.find(' ', 8);
                    unsigned int regrowthCoefficient = std::stoi(speciesDefinition.substr(8, div - 8));
                    unsigned int energy = std::stoi(speciesDefinition.substr(div + 1));

                    unit->setPlant(std::unique_ptr<Plant>(new Plant(std::string(1, ch), energy, regrowthCoefficient)));
                } else {
                    unsigned int energy = std::stoi(speciesDefinition.substr(speciesDefinition.find(']') + 2));

                    std::string foodChainStr = speciesDefinition.substr(speciesDefinition.find('[') + 1, speciesDefinition.find(']') - speciesDefinition.find('[') - 1);
                    std::vector<std::string> foodChain;
                    for (int i = 0; i < foodChainStr.length(); ++i) {
                        if (i % 3 == 0) {
                            foodChain.push_back(std::string(1, foodChainStr[i]));
                        }
                    }

                    AnimalType animalType = organismClassification == "herbivore" ? AnimalType::HERBIVORE : AnimalType::OMNIVORE;
                    unit->setAnimal(std::unique_ptr<Animal>(new Animal(std::string(1, ch), energy, energy, animalType, foodChain)));
                }
                return unit;
            }
        }
        if (ch == ' ') { // Empty space
            return std::shared_ptr<ViableUnit>(new ViableUnit());
        } else { // Obstacle
            return std::shared_ptr<Obstacle>(new Obstacle(std::string(1, ch)));
        }
    }

    std::shared_ptr<Unit> Environment::getUnit(int x, int y) {
        try {
            return grid.at(x).at(y);
        } catch (const std::out_of_range& exception) {
            return nullptr;
        }
    }

}