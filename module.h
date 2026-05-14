#pragma once
#include <string>
#include <iostream>
#include <memory>

class Module {
protected:
    int level = 1;
    bool active = true;
    int disabledTimer = 0;

public:
    virtual std::string GetType() const = 0;
    int GetLevel() const { return level; }

    bool IsActive() const { return active && disabledTimer == 0; }
    void Toggle() { active = !active; }
    void SetActive(bool act) { active = act; }

    void SetDisabledTimer(int days) { disabledTimer = days; }
    int GetDisabledTimer() const { return disabledTimer; }

    friend std::ostream& operator<<(std::ostream& os, const Module& m) {
        os << m.GetType()
           << " | Уровень: " << m.GetLevel()
           << " | Активен: " << (m.IsActive() ? "Да" : "Нет");
        return os;
    }

    virtual std::shared_ptr<Module> operator+(const Module& other) const {
        if (GetType() != other.GetType() || GetLevel() != other.GetLevel())
            return nullptr;

        auto upgraded = Clone();
        upgraded->Upgrade();
        return upgraded;
    }

    virtual std::shared_ptr<Module> Clone() const = 0;

    virtual int GetEnergyOutput() const = 0;
    virtual int GetEnergyInput() const = 0;
    virtual int GetDataOutput() const = 0;
    virtual int GetHabitationSlots() const = 0;

    virtual void Upgrade() { level++; }

    virtual ~Module() = default;
};