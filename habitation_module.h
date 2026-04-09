#pragma once
#include "module.h"
#include <iostream>

class HabitationModule : public Module {
private:
    int level = 1;
    bool active = true;

public:
    int GetEnergyOutput() const override { return 0; }

    int GetEnergyInput() const override {
        return active ? 10 : 0;
    }

    Module* Clone() const override {
        return new HabitationModule(*this);
    }

    std::string GetType() const override { return "Жилой модуль"; }
    int GetLevel() const override { return level; }

    int GetDataOutput() const override { return 0; }

    int GetHabitationSlots() const override {
        return 8 * level;
    }

    void Upgrade() override { level++; }

    bool IsActive() const override { return active; }
    void Toggle() override { active = !active; }
};