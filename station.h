#pragma once
#include <vector>
#include <string>
#include <random>
#include <memory>

#include "robot.h"
#include "module.h"

class Station {
private:
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