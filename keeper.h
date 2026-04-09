#pragma once
#include "robot.h"

class Keeper : public Robot {
public:
    Keeper(std::string n) : Robot(n) {}

    std::string GetType() const override { return "ХРАНИТЕЛЬ"; }

    Robot* CloneWithStats(int ch, int fw, std::string n) const override {
        Keeper * r = new Keeper(n);
        r->RepairChassis(100);
        r->RepairFirmware(100);
        r->DamageChassis(100 - ch);
        r->RepairFirmware(fw - 100);
        return r;
    }

    Resources ProduceResources() const override {
        double mult = ageDays > 10 ? 1.5 : 1.0;
        return {int(3 * mult), int(10 * mult)};
    }

    int GetSlotsUsed() const override { return 2; }

    void PrintStatus() const override;
};

void Keeper::PrintStatus() const {
    std::cout << *this << std::endl;
}