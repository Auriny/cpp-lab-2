#pragma once
#include <string>
#include <iostream>
#include <random>
#include <algorithm>

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
    Robot(const std::string &n) : name(n) {}

    Robot& operator--() {
        Age();
        return *this;
    }

    Robot& operator++() {
        chassisIntegrity = 100;
        firmwareIntegrity = 100;
        return *this;
    }

    bool operator==(const Robot& other) const {
        return GetType() == other.GetType() &&
               chassisIntegrity == other.chassisIntegrity &&
               firmwareIntegrity == other.firmwareIntegrity;
    }

    virtual Robot* CloneWithStats(int chassis, int firmware, std::string name) const = 0;

    Robot* operator+(const Robot& other) const {
        if (ageDays < 5 || other.ageDays < 5) return nullptr;

        static std::default_random_engine rng(std::random_device{}());
        std::uniform_real_distribution mutation(0.8, 1.2);

        double factor = mutation(rng);

        int avgChassis = (chassisIntegrity + other.chassisIntegrity) / 2;
        int avgFirmware = (firmwareIntegrity + other.firmwareIntegrity) / 2;

        int newChassis = static_cast<int>(avgChassis * factor);
        int newFirmware = static_cast<int>(avgFirmware * factor);

        newChassis = std::clamp(newChassis, 1, 100);
        newFirmware = std::clamp(newFirmware, 1, 100);

        const Robot* stronger =(chassisIntegrity + firmwareIntegrity >other.chassisIntegrity + other.firmwareIntegrity)? this : &other;

        return stronger->CloneWithStats(newChassis, newFirmware, "малышка");
    }

    virtual std::string GetType() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Robot& r) {
        os << r.name << " | " << r.GetType()
           << " | Шасси: " << r.chassisIntegrity
           << " | Прошивка: " << r.firmwareIntegrity
           << " | Возраст: " << r.ageDays;

        if (!r.alive) os << " | МЁРТВ";
        return os;
    }

    // friend std::istream& operator>>(std::istream& is, Robot& r) {
        // is >> r.name;
        // return is;
    // }

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