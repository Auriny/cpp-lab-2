#pragma once

class Module {
public:
    virtual std::string GetType() const = 0;
    virtual int GetLevel() const { return 1; }

    friend std::ostream& operator<<(std::ostream& os, const Module& m) {
        os << m.GetType()
           << " | Уровень: " << m.GetLevel()
           << " | Активен: " << (m.IsActive() ? "Да" : "Нет");
        return os;
    }

    virtual Module* operator+(const Module& other) const {
        if (GetType() != other.GetType()) return nullptr;
        if (GetLevel() != other.GetLevel()) return nullptr;

        Module* upgraded = Clone();
        upgraded->Upgrade();
        return upgraded;
    }

    virtual Module* Clone() const = 0;

    virtual int GetEnergyOutput() const = 0;
    virtual int GetEnergyInput() const = 0;
    virtual int GetDataOutput() const = 0;
    virtual int GetHabitationSlots() const = 0;

    virtual void Upgrade() = 0;

    virtual bool IsActive() const = 0;
    virtual void Toggle() = 0;

    virtual ~Module() = default;
};