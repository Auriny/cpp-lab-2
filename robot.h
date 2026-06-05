#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "observer.h"

struct Resources {
    int energy;
    int data;
};

class Robot : public IObserver {
protected:
    std::string name;
    int chassisIntegrity = 100;
    int firmwareIntegrity = 100;
    int ageDays = 0;
    bool alive = true;

public:
    Robot(std::string n) : name(n) {}
    virtual ~Robot() = default;

    std::string GetName() const { return name; }
    bool IsAlive() const { return alive; }
    int GetChassis() const { return chassisIntegrity; }
    int GetFirmware() const { return firmwareIntegrity; }

    virtual std::string GetType() const = 0;
    virtual std::unique_ptr<Robot> CloneWithStats(int ch, int fw, std::string n) const = 0;
    virtual Resources ProduceResources() const = 0;
    virtual int GetSlotsUsed() const = 0;
    virtual void PrintStatus() const = 0;

    void Age();
    void DamageChassis(int dmg);
    void DamageFirmware(int dmg);
    void RepairChassis(int amount);
    void RepairFirmware(int amount);

    // Реакция на тревогу (Observer)
    void OnAlarm(const std::string& threat) override {
        std::cout << "[" << GetType() << " " << name << "] переходит в режим боевой готовности. Причина: " << threat << "\n";
    }

    virtual std::unique_ptr<Robot> operator+(const Robot& other) const {
        if (GetType() == other.GetType()) {
            return CloneWithStats(100, 100, name + "-2");
        }
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Robot& r) {
        os << r.GetType() << " " << r.name
           << " | Шасси: " << r.chassisIntegrity
           << " | Прошивка: " << r.firmwareIntegrity;
        return os;
    }
};