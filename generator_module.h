#pragma once
#include "module.h"

class GeneratorModule : public Module {
public:
    int GetEnergyOutput() const override { return IsActive() ? 20 * level : 0; }
    int GetEnergyInput() const override { return 0; }
    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<GeneratorModule>(*this);
    }
    std::string GetType() const override { return "Генератор"; }
};