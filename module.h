#pragma once

class Module {
public:
    virtual int GetEnergyOutput() const = 0;
    virtual int GetEnergyInput() const = 0;
    virtual int GetDataOutput() const = 0;
    virtual int GetHabitationSlots() const = 0;

    virtual void Upgrade() = 0;

    virtual bool IsActive() const = 0;
    virtual void Toggle() = 0;

    virtual ~Module() = default;
};