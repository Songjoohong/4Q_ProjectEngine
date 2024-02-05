#include "pch.h"
#include "Test.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>

using json = nlohmann::json;

// Define your component classes (example)
struct ComponentA {
    int valueA;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ComponentA, valueA)
};

struct ComponentB {
    float valueB;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ComponentB, valueB)
};

// Define your GameObject structure
struct GameObject {
    std::string name;
    ComponentA componentA;
    ComponentB componentB;
    // Add other components as needed

    // Serialize GameObject and its components
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameObject, name, componentA, componentB)
};

int main() {
    std::vector<GameObject> gameObjects = {/* Initialize your game objects here */ };

    // Serialize game objects to JSON string
    json jsonObject = gameObjects;

    // Write JSON string to file
    std::ofstream outputFile("game_objects.json");
    outputFile << std::setw(4) << jsonObject << std::endl;
    outputFile.close();

    return 0;
}