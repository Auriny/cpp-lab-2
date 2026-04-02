#pragma once
#include "module.h"
#include <iostream>

class GeneratorModule : public Module {
private:
    int level = 1;
    bool active = true;

public:
    int GetEnergyOutput() const override {
        return active ? 20 * level : 0;
    }

    int GetEnergyInput() const override { return 0; }
    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    void Upgrade() override { level++; }

    bool IsActive() const override { return active; }
    void Toggle() override { active = !active; }
};