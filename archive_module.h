#pragma once
#include "module.h"
#include <iostream>

class ArchiveModule : public Module {
private:
    int level = 1;
    bool active = true;

public:
    int GetEnergyOutput() const override { return 0; }
    int GetEnergyInput() const override { return 0; }

    int GetDataOutput() const override {
        return active ? 5 * level : 0;
    }

    Module* Clone() const override {
        return new ArchiveModule(*this);
    }

    std::string GetType() const override { return "Архив"; }
    int GetLevel() const override { return level; }

    int GetHabitationSlots() const override { return 0; }

    void Upgrade() override { level++; }

    bool IsActive() const override { return active; }
    void Toggle() override { active = !active; }
};