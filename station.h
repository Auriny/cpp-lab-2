#pragma once
#include <vector>
#include <string>
#include <random>
#include <memory>

#include "robot.h"
#include "module.h"
#include "observer.h"

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
    bool isGameRunning = true;

    std::default_random_engine rng{std::random_device{}()};

    // издатель
    AlarmSystem alarm;

    // синглтонность
    Station();
    ~Station() = default;
    Station(const Station&) = delete;
    Station& operator=(const Station&) = delete;

public:
    static Station& GetInstance();

    void Init();
    void AddRobot(std::unique_ptr<Robot> r);
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
    void EndGame();
    void StartGame();
};