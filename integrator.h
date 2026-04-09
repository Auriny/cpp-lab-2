#pragma once
#include "robot.h"
#include <iostream>


class Integrator : public Robot {
public:
    Integrator(std::string n) : Robot(n) {}

    std::string GetType() const override { return "ИНТЕГРАТОР"; }

    Robot* CloneWithStats(int ch, int fw, std::string n) const override {
        Integrator* r = new Integrator(n);
        r->RepairChassis(100);
        r->RepairFirmware(100);
        r->DamageChassis(100 - ch);
        r->RepairFirmware(fw - 100);
        return r;
    }

    Resources ProduceResources() const override {
        double mult = ageDays > 10 ? 1.5 : 1.0;
        return {int(10 * mult), int(3 * mult)};
    }

    int GetSlotsUsed() const override { return 1; }

    void PrintStatus() const override;
};

void Integrator::PrintStatus() const {
    std::cout << *this << std::endl;
}