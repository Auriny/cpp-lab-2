#pragma once
#include "robot.h"
#include <iostream>


class Integrator : public Robot {
public:
    Integrator(std::string n) : Robot(n) {}

    Resources ProduceResources() const override {
        double mult = ageDays > 10 ? 1.5 : 1.0;
        return {int(10 * mult), int(3 * mult)};
    }

    int GetSlotsUsed() const override { return 1; }

    void PrintStatus() const override;
};

void Integrator::PrintStatus() const {
    std::cout << name << " | ИНТЕГРАТОР | "
              << "Шасси:" << chassisIntegrity
              << " Прошивка:" << firmwareIntegrity;

    if (!alive) std::cout << " МЁРТВ";
    std::cout << std::endl;
}