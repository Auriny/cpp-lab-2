#pragma once
#include "module.h"

class HabitationModule : public Module {
public:
    int GetEnergyOutput() const override { return 0; }
    int GetEnergyInput() const override { return IsActive() ? 10 : 0; }
    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 8 * level; }

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<HabitationModule>(*this);
    }
    std::string GetType() const override { return "Жилой модуль"; }
};