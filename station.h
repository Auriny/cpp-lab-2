#pragma once
#include <vector>
#include <string>
#include <random>

#include "Robot.h"
#include "Module.h"

class Station {
private:
    std::string name;

    std::vector<Robot> robots;
    std::vector<Module> modules;

    int energy;
    int maxEnergy;
    int bits;

    int day;

    std::default_random_engine rng{std::random_device{}()};

public:
    Station();

    void Init();

    int CalculateSignalChance();

    int CalculateHabitation();

    void ProductionPhase();

    void HousingCheck();

    void SignalAttempt();

    void ModuleConsumption();

    void RepairPhase();

    void AgingPhase();

    void RemoveDead();

    void CorporationTax();

    void ProcessDay();

    void PrintStatus();

    bool IsGameOver();

    void StartGame();
};