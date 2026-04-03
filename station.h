#pragma once
#include <vector>
#include <string>
#include <random>

#include "robot.h"
#include "module.h"

class Station {
    std::string name;

    std::vector<Robot*> robots;
    std::vector<Module*> modules;

    int energy;
    int maxEnergy;
    int bits;

    int day;

    std::default_random_engine rng{std::random_device{}()};

public:
    Station();

    void Init();

    int CalculateSignalChance() const;

    int CalculateHabitation() const;

    void ProductionPhase();

    void HousingCheck() const;

    void SignalAttempt();

    void ModuleConsumption();

    void RepairPhase();

    void AgingPhase() const;

    void RemoveDead();

    void CorporationTax();

    void ProcessDay();

    void PrintStatus() const;

    bool IsGameOver() const;

    void StartGame();

    ~Station();
};
