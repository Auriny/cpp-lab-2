#pragma once
#include "robot.h"

class LocalComputer : public Robot {
public:
    LocalComputer(std::string n) : Robot(n) {}

    std::string GetType() const override { return "ВЫЧИСЛИТЕЛЬ"; }

    Robot* CloneWithStats(int ch, int fw, std::string n) const override {
        LocalComputer * r = new LocalComputer(n);
        r->RepairChassis(100);
        r->RepairFirmware(100);
        r->DamageChassis(100 - ch);
        r->RepairFirmware(fw - 100);
        return r;
    }

    Resources ProduceResources() const override {
        return {5, 5};
    }

    int GetSlotsUsed() const override { return 1; }

    void PrintStatus() const override;
};

void LocalComputer::PrintStatus() const {
    std::cout << *this << std::endl;
}