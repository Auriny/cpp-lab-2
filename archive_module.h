#pragma once
#include "module.h"

class ArchiveModule : public Module {
public:
    int GetEnergyOutput() const override { return 0; }
    int GetEnergyInput() const override { return 0; }
    int GetDataOutput() const override { return IsActive() ? 5 * level : 0; }
    int GetHabitationSlots() const override { return 0; }

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<ArchiveModule>(*this);
    }
    std::string GetType() const override { return "Архив"; }
};