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
            for (int j = 0; j < grid.size(); ++j) {
                for (int k = 0; k < grid[j].size(); ++k) {
                    if (getUnit(j, k)->getUnitType() == UnitType::VIABLE_UNIT) {
                        std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(getUnit(j, k));
                        if (viableUnit->plant != nullptr) {
                            viableUnit->iteratePlant();
                        }
                    }
                }
            }
            iterateAnimals(AnimalType::HERBIVORE);
            iterateAnimals(AnimalType::OMNIVORE);
            // Mark animals as ready to be iterated (movable) during the next iteration
            for (int j = 0; j < grid.size(); ++j) {
                for (int k = 0; k < grid[j].size(); ++k) {
                    if (getUnit(j, k)->getUnitType() == UnitType::VIABLE_UNIT) {
                        std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(getUnit(j, k));
                        if (viableUnit->animal != nullptr) {
                            viableUnit->animal->setMoved(false);
                            viableUnit->animal->setReproduced(false);
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
                    if (viableUnit->animal != nullptr &&
                        viableUnit->animal->getAnimalType() == animalType &&
                        !viableUnit->animal->hasMoved()) {

                        auto& animal = viableUnit->animal;

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
                                if (topMates == maxMates) {
                                    deltaY = -1;
                                } else if (rightMates == maxMates) {
                                    deltaX = 1;
                                } else if (bottomMates == maxMates) {
                                    deltaY = 1;
                                } else if (leftMates == maxMates) {
                                    deltaX = -1;
                                }
                            } else if (animal->getEnergy() < animal->getMaxEnergy()) {
                                unsigned int topEnergy = 0;
                                unsigned int rightEnergy = 0;
                                unsigned int bottomEnergy = 0;
                                unsigned int leftEnergy = 0;
                                if (animal->getEnergy() >= 2) {
                                    topEnergy = availableEnergy(*animal, top) + availableEnergy(*animal, topLeft) + availableEnergy(*animal, topCenter);
                                    rightEnergy = availableEnergy(*animal, topRight) + availableEnergy(*animal, rightCenter) + availableEnergy(*animal, right);
                                    bottomEnergy = availableEnergy(*animal, bottomCenter) + availableEnergy(*animal, bottomRight) + availableEnergy(*animal, bottom);
                                    leftEnergy = availableEnergy(*animal, left) + availableEnergy(*animal, leftCenter) + availableEnergy(*animal, bottomLeft);
                                } else if (animal->getEnergy() < 3) {
                                    topEnergy = availableEnergy(*animal, topCenter);
                                    rightEnergy = availableEnergy(*animal, rightCenter);
                                    bottomEnergy = availableEnergy(*animal, bottomCenter);
                                    leftEnergy = availableEnergy(*animal, leftCenter);
                                }

                                unsigned int maxEnergy = std::max(std::max(topEnergy, bottomEnergy), std::max(rightEnergy, leftEnergy));
                                if (maxEnergy > 0) {
                                    if (topEnergy == maxEnergy) {
                                        deltaY = -1;
                                    } else if (rightEnergy == maxEnergy) {
                                        deltaX = 1;
                                    } else if (bottomEnergy == maxEnergy) {
                                        deltaY = 1;
                                    } else if (leftEnergy == maxEnergy) {
                                        deltaX = -1;
                                    }
                                } else {
                                    int random = rand() % 100;
                                    if (random < 25) {
                                        deltaY = -1;
                                    } else if (random < 50) {
                                        deltaX = 1;
                                    } else if (random < 75) {
                                        deltaY = 1;
                                    } else {
                                        deltaX = -1;
                                    }
                                }
                            } else {
                                int random = rand() % 100;
                                if (random < 25) {
                                    deltaY = -1;
                                } else if (random < 50) {
                                    deltaX = 1;
                                } else if (random < 75) {
                                    deltaY = 1;
                                } else {
                                    deltaX = -1;
                                }
                            }
                        }

                        // Try to move to the desired ViableUnit
                        // Otherwise stay put
                        // (this leads to some irrational behavior
                        // on the part of the animals, but this
                        // quirkiness is good because it better
                        // simulates life in that it isn't perfect)

                        // Overall, the movement algorithm isn't
                        // very smart, but that is a relatively
                        // accurate simulation of real life. If
                        // the algorithm were smart 100% of the
                        // time, animals could run away forever.

                        if (canMoveTo(*animal, getUnit(j + deltaY, k + deltaX))) {
                            std::shared_ptr<ViableUnit> destination = std::dynamic_pointer_cast<ViableUnit>(getUnit(j + deltaY, k + deltaX));
                            unsigned int plantEnergy = destination->plant != nullptr ? destination->plant->consume() : 0;
                            unsigned int animalEnergy = destination->animal != nullptr ? destination->animal->getEnergy() : 0;
                            animal->setEnergy(animal->getEnergy() + plantEnergy + animalEnergy - 1);

                            // Move there
                            destination->animal = std::move(viableUnit->animal);
                            destination->animal->setMoved();

                            if (destination->animal->getEnergy() <= 0) {
                                destination->animal.reset(nullptr);
                            }
                        }
                    }
                }
            }
        }
        for (int j = 0; j < grid.size(); ++j) {
            for (int k = 0; k < grid[j].size(); ++k) {
                if (getUnit(j, k)->getUnitType() == UnitType::VIABLE_UNIT) {
                    std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(getUnit(j, k));
                    if (viableUnit->animal != nullptr &&
                        viableUnit->animal->getAnimalType() == animalType &&
                        !viableUnit->animal->hasReproduced()) {

                        auto& animal = viableUnit->animal;

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

                        std::vector<std::shared_ptr<Unit>> spawnLocations;
                        if (canMateWith(*animal, topCenter)) {
                            spawnLocations = {topLeft, top, topRight, rightCenter, bottomCenter, leftCenter};
                            if (spawnChild(*animal, spawnLocations)) {
                                std::shared_ptr<ViableUnit> mateUnit = std::dynamic_pointer_cast<ViableUnit>(topCenter);
                                animal->setReproduced();
                                animal->setEnergy((unsigned int) (0.4 * animal->getEnergy()));
                                mateUnit->animal->setReproduced();
                                mateUnit->animal->setEnergy((unsigned int) (0.4 * mateUnit->animal->getEnergy()));
                            }
                        } else if (canMateWith(*animal, rightCenter)) {
                            spawnLocations = {topRight, right, bottomRight, bottomCenter, leftCenter, topCenter};
                            if (spawnChild(*animal, spawnLocations)) {
                                std::shared_ptr<ViableUnit> mateUnit = std::dynamic_pointer_cast<ViableUnit>(rightCenter);
                                animal->setReproduced();
                                animal->setEnergy((unsigned int) (0.4 * animal->getEnergy()));
                                mateUnit->animal->setReproduced();
                                mateUnit->animal->setEnergy((unsigned int) (0.4 * mateUnit->animal->getEnergy()));
                            }
                        } else if (canMateWith(*animal, bottomCenter)) {
                            spawnLocations = {bottomRight, bottom, bottomLeft, leftCenter, topCenter, rightCenter};
                            if (spawnChild(*animal, spawnLocations)) {
                                std::shared_ptr<ViableUnit> mateUnit = std::dynamic_pointer_cast<ViableUnit>(bottomCenter);
                                animal->setReproduced();
                                animal->setEnergy((unsigned int) (0.4 * animal->getEnergy()));
                                mateUnit->animal->setReproduced();
                                mateUnit->animal->setEnergy((unsigned int) (0.4 * mateUnit->animal->getEnergy()));
                            }
                        } else if (canMateWith(*animal, leftCenter)) {
                            spawnLocations = {bottomLeft, left, topLeft, topCenter, rightCenter, bottomCenter};
                            if (spawnChild(*animal, spawnLocations)) {
                                std::shared_ptr<ViableUnit> mateUnit = std::dynamic_pointer_cast<ViableUnit>(leftCenter);
                                animal->setReproduced();
                                animal->setEnergy((unsigned int) (0.4 * animal->getEnergy()));
                                mateUnit->animal->setReproduced();
                                mateUnit->animal->setEnergy((unsigned int) (0.4 * mateUnit->animal->getEnergy()));
                            }
                        }
                    }
                }
            }
        }
    }

    bool Environment::spawnChild(const Animal& animal, const std::vector<std::shared_ptr<Unit>>& spawnLocations) {
        for (auto& spawnLocation : spawnLocations) {
            if (spawnLocation != nullptr && spawnLocation->getUnitType() == UnitType::VIABLE_UNIT) {
                std::shared_ptr<ViableUnit> viableLocation = std::dynamic_pointer_cast<ViableUnit>(spawnLocation);
                if (viableLocation->plant == nullptr && viableLocation->animal == nullptr) {
                    // Spawn offspring here
                    int random = rand() % 100;
                    unsigned int energy = (unsigned int) ((random >= 35 ? (double) random / 100 : 0.35) * animal.getMaxEnergy());
                    viableLocation->animal = std::make_unique<Animal>(animal.toString(), animal.getMaxEnergy(),
                            energy, animal.getAnimalType(), animal.getFoodChain());
                    // Prevent an animal from moving or reproducing on the same turn it is born
                    viableLocation->animal->setMoved();
                    viableLocation->animal->setReproduced();

                    return true;
                }
            }
        }
        return false;
    }

    bool Environment::canMoveTo(const Animal& animal, const std::shared_ptr<Unit>& unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);

            if (viableUnit->plant == nullptr && viableUnit->animal == nullptr) {
                return true;
            } else if (viableUnit->animal == nullptr) {
                return canConsume(animal, *viableUnit->plant);
            } else if (viableUnit->plant == nullptr) {
                return canConsume(animal, *viableUnit->animal);
            } else {
                return canConsume(animal, *viableUnit->plant) && canConsume(animal, *viableUnit->animal) && animal.getEnergy() < animal.getMaxEnergy() - 1;
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

    bool Environment::isPredatorOf(const Animal& animal, const std::shared_ptr<Unit>& unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            if (viableUnit->animal == nullptr) {
                return false;
            } else {
                std::vector<std::string> foodChain = viableUnit->animal->getFoodChain();
                return std::find(foodChain.begin(), foodChain.end(), animal.toString()) != foodChain.end();
            }
        }
    }

    bool Environment::canMateWith(const Animal& animal, const std::shared_ptr<Unit>& unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            if (viableUnit->animal == nullptr || viableUnit->animal->toString() != animal.toString()) {
                return false;
            } else {
                return animal.getEnergy() > 0.5 * animal.getMaxEnergy() &&
                    viableUnit->animal->getEnergy() > 0.5 * viableUnit->animal->getMaxEnergy() &&
                    !animal.hasReproduced() && !viableUnit->animal->hasReproduced();
            }
        }
    }

    unsigned int Environment::availableEnergy(const Animal& animal, const std::shared_ptr<Unit>& unit) {
        if (unit == nullptr || unit->getUnitType() == UnitType::OBSTACLE) {
            return false;
        } else {
            std::shared_ptr<ViableUnit> viableUnit = std::dynamic_pointer_cast<ViableUnit>(unit);
            unsigned int plantEnergy = 0;
            unsigned int animalEnergy = 0;
            if (viableUnit->plant != nullptr && canConsume(animal, *viableUnit->plant)) {
                plantEnergy = viableUnit->plant->getEnergy();
            }
            if (viableUnit->animal != nullptr && canConsume(animal, *viableUnit->animal)) {
                animalEnergy = viableUnit->animal->getEnergy();
            }
            return plantEnergy + animalEnergy;
        }
    }

    std::string Environment::toString() const {
        std::string result;
        for (int i = 0; i < grid.size(); ++i) {
            for (const auto& j : grid[i]) {
                result += j->toString();
            }
            if (i < grid.size() - 1) {
                result += "\n";
            }
        }
        return result;
    }

    std::shared_ptr<Unit> Environment::parseUnit(char ch, const std::vector<std::string>& speciesLines) const {
        try {
            for (auto &speciesDefinition : speciesLines) {
                unsigned int typeCharIndex = speciesDefinition.find(' ') + 1;
                char typeChar = speciesDefinition.at(typeCharIndex);
                if (ch == typeChar) { // Occupied space
                    std::unique_ptr<ViableUnit> unit(new ViableUnit());

                    std::string organismClassification = speciesDefinition.substr(0, typeCharIndex - 1);
                    if (organismClassification == "plant") {
                        int div = speciesDefinition.find(' ', 8);
                        unsigned int regrowthCoefficient = std::stoi(speciesDefinition.substr(8, div - 8));
                        unsigned int energy = std::stoi(speciesDefinition.substr(div + 1));

                        unit->plant = std::make_unique<Plant>(std::string(1, ch), energy, regrowthCoefficient);
                    } else {
                        unsigned int energy = std::stoi(speciesDefinition.substr(speciesDefinition.find(']') + 2));

                        std::string foodChainStr = speciesDefinition.substr(speciesDefinition.find('[') + 1,
                                                                            speciesDefinition.find(']') -
                                                                            speciesDefinition.find('[') - 1);
                        std::vector<std::string> foodChain;
                        for (int i = 0; i < foodChainStr.length(); ++i) {
                            if (i % 3 == 0) {
                                foodChain.emplace_back(1, foodChainStr[i]);
                            }
                        }

                        AnimalType animalType =
                                organismClassification == "herbivore" ? AnimalType::HERBIVORE : AnimalType::OMNIVORE;
                        unit->animal = std::make_unique<Animal>(std::string(1, ch), energy, energy, animalType,
                                                                foodChain);
                    }
                    return unit;
                }
            }
            if (ch == ' ') { // Empty space
                return std::shared_ptr<ViableUnit>(new ViableUnit());
            } else { // Obstacle
                return std::shared_ptr<Obstacle>(new Obstacle(std::string(1, ch)));
            }
        } catch (const std::exception& exception) {
            std::cerr
                << "An error occurred when parsing the input files:\n"
                << exception.what()
                << std::endl;
            exit(-4);
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