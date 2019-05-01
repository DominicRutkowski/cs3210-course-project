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
                            animal->setMoved(false);
                            animal->setReproduced(false);
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
                        !animal->hasMoved()) {

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

                        bool topPredator = isPredatorOf(*animal, top) || isPredatorOf(*animal, topLeft) || isPredatorOf(*animal, topCenter);
                        bool rightPredator = isPredatorOf(*animal, topRight) || isPredatorOf(*animal, rightCenter) || isPredatorOf(*animal, right);
                        bool bottomPredator = isPredatorOf(*animal, bottomCenter) || isPredatorOf(*animal, bottomRight) || isPredatorOf(*animal, bottom);
                        bool leftPredator = isPredatorOf(*animal, left) || isPredatorOf(*animal, leftCenter) || isPredatorOf(*animal, bottomLeft);

                        int deltaX = 0;
                        int deltaY = 0;
                        if (topPredator || rightPredator || bottomPredator || leftPredator) {
                            if (rightPredator == leftPredator) {
                                deltaX = 0;
                            } else if (rightPredator) {
                                deltaX = -1;
                            } else if (leftPredator) {
                                deltaX = 1;
                            }

                            if (topPredator == bottomPredator) {
                                deltaY = 0;
                            } else if (topPredator) {
                                deltaY = 1;
                            } else if (bottomPredator) {
                                deltaY = -1;
                            }

                            if (deltaX != 0 && deltaY != 0) {
                                deltaY = 0;
                            }
                        } else {
                            unsigned int topMates = canMateWith(*animal, top) + canMateWith(*animal, topLeft) + canMateWith(*animal, topCenter);
                            unsigned int rightMates = canMateWith(*animal, topRight) + canMateWith(*animal, rightCenter) + canMateWith(*animal, right);
                            unsigned int bottomMates = canMateWith(*animal, bottomCenter) + canMateWith(*animal, bottomRight) + canMateWith(*animal, bottom);
                            unsigned int leftMates = canMateWith(*animal, left) + canMateWith(*animal, leftCenter) + canMateWith(*animal, bottomLeft);

                            unsigned int maxMates = std::max(std::max(topMates, bottomMates), std::max(rightMates, leftMates));
                            if (maxMates > 0) {
                                std::cout << "Mate move " << j << ", " << k << ": ";
                                if (topMates == maxMates) {
                                    deltaY = -1;
                                } else if (rightMates == maxMates) {
                                    deltaX = 1;
                                } else if (bottomMates == maxMates) {
                                    deltaY = 1;
                                } else if (leftMates == maxMates) {
                                    deltaX = -1;
                                }
                                std::cout << j + deltaX << ", " << k + deltaY << std::endl;
                            } else {

                            }
                        }

                        // Try to move to the desired ViableUnit
                        // Otherwise stay put
                        // (this leads to some irrational behavior
                        // on the part of the animals, but this
                        // quirkiness is good because it better
                        // simulates life in that it isn't perfect)
                        if (canMoveTo(*animal, getUnit(j + deltaY, k + deltaX))) {
                            std::shared_ptr<ViableUnit> destination = std::dynamic_pointer_cast<ViableUnit>(getUnit(j + deltaX, k + deltaY));
                            unsigned int plantEnergy = destination->getPlant() != nullptr ? destination->getPlant()->consume() : 0;
                            unsigned int animalEnergy = destination->getAnimal() != nullptr ? destination->getAnimal()->getEnergy() : 0;
                            animal->setEnergy(animal->getEnergy() + plantEnergy + animalEnergy);

                            // Move there
                            destination->animal = std::move(viableUnit->animal);

                            destination->animal->setMoved();

                            destination->animal->setEnergy(destination->animal->getEnergy() - 1);
                            if (destination->animal->getEnergy() <= 0) {
                                destination->animal.reset(); // Animals with no energy die
                            }
                        }
                    }
                }
            }
        }
    }

    bool Environment::canMoveTo(const Animal& animal, const std::shared_ptr<Unit> unit) {\
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            if (viableUnit->getPlant() == nullptr && viableUnit->getAnimal() == nullptr) {
                return true;
            } else if (viableUnit->getAnimal() == nullptr) {
                return canConsume(animal, *viableUnit->getPlant());
            } else if (viableUnit->getPlant() == nullptr) {
                return canConsume(animal, *viableUnit->getAnimal());
            } else {
                return canConsume(animal, *viableUnit->getPlant()) && canConsume(animal, *viableUnit->getAnimal()) && animal.getEnergy() < animal.getMaxEnergy() - 1;
            }
        }
    }

    bool Environment::canConsume(const Animal& animal, const Plant& plant) {
        if (plant.getIterationsUntilGrowth() != 0) {
            return true;
        }
        return std::find(animal.getFoodChain().begin(), animal.getFoodChain().end(), plant.toString()) != animal.getFoodChain().end() &&
               animal.getEnergy() < animal.getMaxEnergy();
    }

    bool Environment::canConsume(const Animal& animal, const Animal& prey) {
        return std::find(animal.getFoodChain().begin(), animal.getFoodChain().end(), prey.toString()) != animal.getFoodChain().end() &&
               animal.getEnergy() < animal.getMaxEnergy();
    }

    bool Environment::isPredatorOf(const Animal& animal, const std::shared_ptr<Unit> unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            if (viableUnit->getAnimal() == nullptr) {
                return false;
            } else {
                std::vector<std::string> foodChain = viableUnit->getAnimal()->getFoodChain();
                return std::find(foodChain.begin(), foodChain.end(), animal.toString()) != foodChain.end();
            }
        }
    }

    bool Environment::canMateWith(const Animal& animal, const std::shared_ptr<Unit> unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            if (viableUnit->getAnimal() == nullptr || viableUnit->getAnimal()->toString() != animal.toString()) {
                return false;
            } else {
                return animal.getEnergy() > 0.5 * animal.getMaxEnergy() && viableUnit->getAnimal()->getEnergy() > 0.5 * viableUnit->getAnimal()->getMaxEnergy();
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