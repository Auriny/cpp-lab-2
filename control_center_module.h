#pragma once
#include "module.h"
#include <iostream>

class ControlCenterModule : public Module {
private:
    int level = 1;
    bool active = true;

public:
    int GetEnergyOutput() const override { return 0; }

    int GetEnergyInput() const override {
        return active ? 30 : 0;
    }

    Module* Clone() const override {
        return new ControlCenterModule(*this);
    }

    std::string GetType() const override { return "Жилой модуль"; }
    int GetLevel() const override { return level; }

    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    void Upgrade() override {}

    bool IsActive() const override { return active; }
    void Toggle() override { active = !active; }
};