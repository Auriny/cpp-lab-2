#pragma once
#include "module.h"
#include "robot.h"
#include <stdexcept>
#include <iostream>

// в9
class MainReactor : public Module {
public:
    std::string GetType() const override { return "Главный Реактор (Ядро)"; }
    int GetEnergyOutput() const override { return IsActive() ? 50 * level : 0; }
    int GetEnergyInput() const override { return 0; }
    int GetDataOutput() const override { return 0; }
    int GetHabitationSlots() const override { return 0; }

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<MainReactor>(*this);
    }

    void CoreOverdrive() {
        std::cout << "[РЕАКТОР] Включен критический разгон! Выброс энергии!\n";
    }
};

// прокся ограничивающая доступ
class ReactorProxy : public Module {
    std::shared_ptr<MainReactor> realReactor;
public:
    ReactorProxy() : realReactor(std::make_shared<MainReactor>()) {}

    std::string GetType() const override { return "Прокси-интерфейс Реактора"; }

    int GetEnergyOutput() const override { return realReactor->GetEnergyOutput(); }
    int GetEnergyInput() const override { return realReactor->GetEnergyInput(); }
    int GetDataOutput() const override { return realReactor->GetDataOutput(); }
    int GetHabitationSlots() const override { return realReactor->GetHabitationSlots(); }

    std::shared_ptr<Module> Clone() const override {
        return std::make_shared<ReactorProxy>(*this);
    }

    // метод с проверкой прав доступа
    void Overdrive(Robot* accessor) {
        if (!accessor) throw std::runtime_error("Сбой авторизации: робот не идентифицирован.");

        if (accessor->GetType() == "ИНТЕГРАТОР") {
            std::cout << "АВТОРИЗАЦИЯ УСПЕШНА. Допуск Интегратора '" << accessor->GetName() << "' подтвержден.\n";
            realReactor->CoreOverdrive();
        } else throw std::runtime_error("ACCESS DENIED");
    }
};