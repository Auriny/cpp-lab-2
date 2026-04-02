#pragma once
#include "robot.h"

class LocalComputer : public Robot {
public:
    LocalComputer(std::string n) : Robot(n) {}

    Resources ProduceResources() const override {
        return {5, 5};
    }

    int GetSlotsUsed() const override { return 1; }

    void PrintStatus() const override;
};

void LocalComputer::PrintStatus() const {
    std::cout << name << " | ЛОКАЛЬНЫЙ ВЫЧИСЛИТЕЛЬ | "
              << "Шасси:" << chassisIntegrity
              << " Прошивка:" << firmwareIntegrity;

    if (!alive) std::cout << " МЁРТВ";
    std::cout << std::endl;
}