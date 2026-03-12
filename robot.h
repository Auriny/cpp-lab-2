#pragma once
#include <string>

enum class RobotType {
    INTEGRATOR,
    KEEPER
};

struct Resources {
    int energy;
    int data;
};

class Robot {
private:
    std::string name;
    RobotType type;

    int chassisIntegrity;
    int firmwareIntegrity;

    bool alive;
    int ageDays;

public:
    Robot(std::string n, RobotType t);

    bool IsAlive() const;
    RobotType GetType() const;
    std::string GetName() const;

    int GetSlotsUsed() const;

    Resources ProduceResources() const;

    void Age();

    void DamageChassis(int dmg);

    void RepairChassis(int amount);
    void RepairFirmware(int amount);

    void PrintStatus() const;
};