#pragma once
#include <string>

struct Resources {
    int energy;
    int data;
};

class Robot {
protected:
    std::string name;
    int chassisIntegrity = 100;
    int firmwareIntegrity = 100;
    bool alive = true;
    int ageDays = 0;

public:
    Robot(std::string n) : name(n) {}

    virtual Resources ProduceResources() const = 0;
    virtual int GetSlotsUsed() const = 0;

    virtual void Age();
    virtual void PrintStatus() const = 0;

    bool IsAlive() const { return alive; }

    void DamageChassis(int dmg);
    void RepairChassis(int amount);
    void RepairFirmware(int amount);

    virtual ~Robot() = default;
};