#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "observer.h"

class Module : public IObserver {
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

    // Реакция на тревогу (Observer)
    void OnAlarm(const std::string& threat) override {
        std::cout << "[" << GetType() << "] активирует щиты и герметизирует отсек. Причина: " << threat << "\n";
    }

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