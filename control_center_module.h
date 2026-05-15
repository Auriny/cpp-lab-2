#pragma once
#include "module.h"

class ControlCenterModule : public Module {
public:
    int GetEnergyOutput() const override { return 0; }
    int GetEnergyInput() const override { return IsActive() ? 30 : 0; }
    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    void Upgrade() override {}

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<ControlCenterModule>(*this);
    }
    std::string GetType() const override { return "Центр управления"; }
};