
#include "module.h"

Module::Module(ModuleType t): type(t), level(1), active(true)
{
}

bool Module::IsActive() const {
    return active;
}

void Module::Toggle() {
    active = !active;
}

ModuleType Module::GetType() const {
    return type;
}

int Module::GetEnergyOutput() const {
    if (!active) return 0;
    switch (type){
        case ModuleType::GENERATOR: return 20 * level;
        case ModuleType::ARCHIVE: return 0;
        default: return 0;
    }
}

int Module::GetEnergyInput() const {
    if (!active) return 0;
    switch (type) {
        case ModuleType::CONTROL_CENTER: return 30;
        case ModuleType::HABITATION: return 10;
        default: return 0;
    }
}

int Module::GetDataOutput() const {
    if (!active) return 0;
    if (type == ModuleType::ARCHIVE) return 5 * level;
    return 0;
}

int Module::GetHabitationSlots() const {
    if (type == ModuleType::HABITATION) return 8 * level;
    return 0;
}