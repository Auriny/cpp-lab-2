#include "station.h"
#include <iostream>
#include <algorithm>

#include "module.h"
#include "robot.h"
#include "local_computer.h"
#include "keeper.h"
#include "integrator.h"
#include "generator_module.h"
#include "habitation_module.h"
#include "control_center_module.h"
#include "archive_module.h"

Station::Station()
        : energy(500),
          maxEnergy(1000),
          bits(0),
          day(1)
{
}

Station::~Station() {
    for (auto r : robots) delete r;
    for (auto m : modules) delete m;
}

void Station::Init() {
    modules.push_back(new ArchiveModule());
    modules.push_back(new ControlCenterModule());
    modules.push_back(new HabitationModule());
    modules.push_back(new GeneratorModule());

    robots.push_back(new Integrator("A-001"));
    robots.push_back(new Integrator("A-002"));
    robots.push_back(new Keeper("Чмо"));
    robots.push_back(new LocalComputer("LC-01"));
}

int Station::CalculateSignalChance() {
    int totalPower = 0;

    for (const auto* m : modules) totalPower += m->GetEnergyOutput();
//    for (const auto& r : robots) if (r->IsAlive()) totalPower += 10;

//    totalPower += energy / 20;

    int aliveRobots = 0;
    for (const auto* r : robots) {
        if (!r->IsAlive()) continue;
        aliveRobots++;

        if (dynamic_cast<const LocalComputer*>(r) == nullptr)
            totalPower += 10;
    }

    int interference = aliveRobots * 10;

    double chance = 100* totalPower / (totalPower + interference);

    if (chance < 5) chance = 5;
    if (chance > 90) chance = 90;

    return (int)chance;
}

int Station::CalculateHabitation() {
    int slots = 0;
    for (const auto& m : modules) slots += m->GetHabitationSlots();

    return slots;
}

void Station::ProductionPhase() {
    for (auto* m : modules) {
        energy += m->GetEnergyOutput();
        bits += m->GetDataOutput();
    }

    for (auto* r : robots) {
        if (!r->IsAlive()) continue;

        auto res = r->ProduceResources();
        energy += res.energy;
        bits += res.data;
    }

    if (energy > maxEnergy) energy = maxEnergy;
}

void Station::HousingCheck() {
    int capacity = CalculateHabitation();
    int used = 0;

    for (const auto* r : robots) if (r->IsAlive()) used += r->GetSlotsUsed();

    if (used > capacity) {
        int overflow = used - capacity;

        std::cout << "Переполнение жилого отсека! Роботы получают урон.\n";

        for (auto* r : robots) {
            if (!r->IsAlive()) continue;
            r->DamageChassis(10* 5);
            overflow -= r->GetSlotsUsed();
            if (overflow <= 0) break;
        }
    }
}

void Station::SignalAttempt() {
    int chance = CalculateSignalChance();

    std::uniform_int_distribution<int> dist(1,100);

    int roll = dist(rng);

    if (roll <= chance) {
        int reward = 200;
        bits += reward;

        std::cout << "Пойман сигнал! Получено " << reward << " бит данных.\n";
    }
}

void Station::ModuleConsumption() {
    for (const auto& m : modules) energy -= m->GetEnergyInput();
}

void Station::RepairPhase() {
    for (auto& r : robots) {
        if (!r->IsAlive())continue;

        if (energy >= 5) {
            r->RepairChassis(2);
            energy -= 5;
        }

        if (bits >= 5) {
            r->RepairFirmware(1);
            bits -= 5;
        }
    }
}

void Station::AgingPhase() {
    for (auto& r : robots) r->Age();
}

void Station::RemoveDead() {
    robots.erase(remove_if(
                         robots.begin(),
                         robots.end(),
                         [](Robot* r) {
                             if (!r->IsAlive()) {
                                 delete r;
                                 return true;
                             }
                             return false;
                         }),
                 robots.end());
}

void Station::CorporationTax() {
    if (day % 5 != 0)return;
    std::cout << "Орбитальная корпорация взимает налог.\n";

    if (energy > 0) {
        int tax = energy * 0.10;
        energy -= tax;
        std::cout << "Списано энергии: " << tax << std::endl;
    }
    else {
        int tax = bits * 0.20;
        bits -= tax;
        std::cout << "Списано данных: " << tax << std::endl;
    }
}

void Station::ProcessDay() {
    std::cout << "\n===== СТАНЦИЯ: " << name << " =====";
    std::cout << "\n=== ДЕНЬ " << day << " ===\n";

    ProductionPhase();
    HousingCheck();
    SignalAttempt();
    ModuleConsumption();
    RepairPhase();
    AgingPhase();
    CorporationTax();
    RemoveDead();
    PrintStatus();

    day++;
}

void Station::PrintStatus() {
    std::cout << "Энергия: " << energy << "/" << maxEnergy << std::endl;
    std::cout << "Данные: " << bits << std::endl;
    std::cout << "Шанс сигнала: " << CalculateSignalChance() << "%\n";

    std::cout << "\nРоботы:\n";
    for (auto* r : robots) r->PrintStatus();

    std::cout << "\nМодули:\n";
    for (int i = 0; i < modules.size(); i++) {
        std::cout << i+1 << ": ";

        std::cout << "\nМодули:\n";
        for (int i = 0; i < modules.size(); i++) {
            std::cout << i + 1 << ": " << *modules[i] << "\n";
        }

//        if (dynamic_cast<GeneratorModule*>(modules[i])) std::cout << "Генератор";
//        else if (dynamic_cast<ArchiveModule*>(modules[i])) std::cout << "Архив";
//        else if (dynamic_cast<HabitationModule*>(modules[i])) std::cout << "Жилой";
//        else if (dynamic_cast<ControlCenterModule*>(modules[i])) std::cout << "Центр";

        std::cout << "\n";
    }
}

bool Station::IsGameOver() {
    if (robots.empty()) return true;
    if (energy <= 0 && bits <= 0) return true;

    return false;
}

void Station::StartGame() {
    std::cout << "Введите название станции:\n";
    getline(std::cin, name);

//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Init();

    while (!IsGameOver()) {
        std::cout << "\n===== РЕСУРСЫ =====\n";
        std::cout << "Энергия: " << energy << "/" << maxEnergy << "\n";
        std::cout << "Биты: " << bits << "\n";

        std::cout << "\n1 - следующий день\n";
        std::cout << "2 - построить генератор (100 энергии)\n";
        std::cout << "3 - улучшить модуль (50 бит)\n";
        std::cout << "4 - синтез роботов\n";
        std::cout << "5 - объединить модули\n";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            default: {
                ProcessDay();
            }
            break;

            case 2: {
                if (energy >= 100) {
                    modules.push_back(new GeneratorModule());
                    energy -= 100;
                    std::cout << "Генератор построен!\n";
                } else std::cout << "Недостаточно энергии\n";
            }
            break;

            case 3: {
                if (modules.empty()) {
                    std::cout << "Нет модулей для улучшения\n";
                    continue;
                }

                std::cout << "Выбери модуль:\n";

                for (int i = 0; i < modules.size(); i++) {
                    std::cout << i << " - модуль\n";
                }

                int index;
                std::cin >> index;
                std::cin.ignore();

                if (index >= 0 && index < modules.size()) {
                    if (bits >= 50) {
                        modules[index]->Upgrade();
                        bits -= 50;
                        std::cout << "Модуль улучшен\n";
                    } else {
                        std::cout << "Недостаточно бит\n";
                    }
                }
            }
            break;

            case 4: {
                if (robots.size() < 2) {
                    std::cout << "Недостаточно роботов\n";
                    break;
                }

                int a, b;
                std::cout << "Выбери двух роботов:\n";
                std::cin >> a >> b;

                Robot* child = *robots[a] + *robots[b];

                if (!child) {
                    std::cout << "Синтез невозможен\n";
                } else {
                    robots.push_back(child);
                    std::cout << "Создан новый робот:\n" << *child << "\n";
                }
            }
            break;

            case 5: {
                int a, b; //todo
                std::cin >> a >> b;

                Module* result = *modules[a] + *modules[b];

                if (!result) {
                    std::cout << "Нельзя объединить\n";
                    break;
                }

                delete modules[a];
                delete modules[b];

                modules.erase(modules.begin() + std::max(a,b));
                modules.erase(modules.begin() + std::min(a,b));

                modules.push_back(result);

                std::cout << "Модули объединены\n";
            }
            break;
        }

    }
    std::cout << "\nСтанция умерла\n";
}