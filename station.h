#pragma once
#include <vector>
#include <string>
#include <random>
#include <memory>

#include "robot.h"
#include "module.h"

class Station {
    std::string name;

    std::vector<std::unique_ptr<Robot>> robots;
    std::vector<std::shared_ptr<Module>> modules;

    int energy;
    int maxEnergy;
    int bits;
    int day;

    bool inStorm = false;
    int stormDaysLeft = 0;
    int commFailureDaysLeft = 0;

    std::default_random_engine rng{std::random_device{}()};

public:
    Station(); // V деструктор пустой теперь, должно само без него почитситься
    ~Station() = default;

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
    bool IsGameOver();
    void StartGame();
};