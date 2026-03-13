#pragma once

enum class ModuleType {
    ARCHIVE,
    CONTROL_CENTER,
    HABITATION,
    GENERATOR
};

class Module {
private:
    ModuleType type;
    int level;
    bool active;

public:
    Module(ModuleType t);

    bool IsActive() const;
    void Toggle();

    ModuleType GetType() const;

    int GetEnergyOutput() const;
    int GetEnergyInput() const;
    int GetDataOutput() const;

    int GetHabitationSlots() const;
};