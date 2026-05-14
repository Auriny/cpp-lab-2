#pragma once
#include "robot.h"

class LocalComputer : public Robot {
public:
    LocalComputer(std::string n) : Robot(n) {}

    std::string GetType() const override {
        return "ВЫЧИСЛИТЕЛЬ";
    }

    std::unique_ptr<Robot> CloneWithStats(int ch, int fw, std::string n) const override {
        auto r = std::make_unique<LocalComputer>(n);
        r->RepairChassis(100); r->RepairFirmware(100);
        r->DamageChassis(100 - ch); r->DamageFirmware(100 - fw);
        return r;
    }

    Resources ProduceResources() const override {
        return {5, 5};
    }

    int GetSlotsUsed() const override { return 1; }

    void PrintStatus() const override {
        std::cout << *this << std::endl;
    }
};