#pragma once
#include "robot.h"

class Keeper : public Robot {
public:
    Keeper(std::string n) : Robot(n) {}

    Resources ProduceResources() const override {
        double mult = ageDays > 10 ? 1.5 : 1.0;
        return {int(3 * mult), int(10 * mult)};
    }

    int GetSlotsUsed() const override { return 2; }

    void PrintStatus() const override;
};

void Keeper::PrintStatus() const {
    std::cout << name << " | ХРАНИТЕЛЬ | "
              << "Шасси:" << chassisIntegrity
              << " Прошивка:" << firmwareIntegrity;

    if (!alive) std::cout << " МЁРТВ";
    std::cout << std::endl;
}