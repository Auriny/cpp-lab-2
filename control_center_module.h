#pragma once
#include "module.h"
#include <iostream>

class ControlCenterModule : public Module {
private:
    bool active = true;

public:
    int GetEnergyOutput() const override { return 0; }

    int GetEnergyInput() const override {
        return active ? 30 : 0;
    }

    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    void Upgrade() override {}

    bool IsActive() const override { return active; }
    void Toggle() override { active = !active; }
};