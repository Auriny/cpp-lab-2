#include "station.h"
#include "exceptions.h"
#include <iostream>
#include <algorithm>

#include "local_computer.h"
#include "keeper.h"
#include "integrator.h"
#include "generator_module.h"
#include "habitation_module.h"
#include "control_center_module.h"
#include "archive_module.h"

Station::Station() : energy(500), maxEnergy(1000), bits(0), day(1) {}

void Station::Init() {
    modules.push_back(std::make_shared<ArchiveModule>());
    modules.push_back(std::make_shared<ControlCenterModule>());
    modules.push_back(std::make_shared<HabitationModule>());
    modules.push_back(std::make_shared<GeneratorModule>());

    robots.push_back(std::make_unique<Integrator>("A-001"));
    robots.push_back(std::make_unique<Integrator>("A-002"));
    robots.push_back(std::make_unique<Keeper>("Чмо"));
    robots.push_back(std::make_unique<LocalComputer>("LC-01"));
}

int Station::CalculateSignalChance() {
    if (inStorm || commFailureDaysLeft > 0) return 0;

    int totalPower = 0;
    for (const auto& m : modules) totalPower += m->GetEnergyOutput();

    int aliveRobots = 0;
    for (const auto& r : robots) {
        if (!r->IsAlive()) continue;
        aliveRobots++;
        if (dynamic_cast<LocalComputer*>(r.get()) == nullptr) totalPower += 10;
    }

    int interference = aliveRobots * 10;
    double chance = 100 * totalPower / (totalPower + interference);

    if (chance < 5) chance = 5;
    if (chance > 90) chance = 90;

    return static_cast<int>(chance);
}

int Station::CalculateHabitation() const {
    int slots = 0;
    for (const auto& m : modules) slots += m->GetHabitationSlots();
    return slots;
}

void Station::ProductionPhase() {
    for (auto& m : modules) {
        energy += m->GetEnergyOutput();
        bits += m->GetDataOutput();
    }
    for (auto& r : robots) {
        if (!r->IsAlive()) continue;
        auto res = r->ProduceResources();
        energy += res.energy;
        bits += res.data;
    }
    if (energy > maxEnergy) energy = maxEnergy;
}

void Station::HousingCheck() const {
    int capacity = CalculateHabitation();
    int used = 0;
    for (const auto& r : robots) if (r->IsAlive()) used += r->GetSlotsUsed();

    if (used > capacity) {
        int overflow = used - capacity;
        std::cout << "Переполнение жилого отсека! Роботы получают урон.\n";
        for (auto& r : robots) {
            if (!r->IsAlive()) continue;
            r->DamageChassis(10 * 5);
            overflow -= r->GetSlotsUsed();
            if (overflow <= 0) break;
        }
    }
}

void Station::SignalAttempt() {
    int chance = CalculateSignalChance();
    if (chance <= 0) return;

    std::uniform_int_distribution<int> dist(1, 100);
    if (dist(rng) <= chance) {
        bits += 200;
        std::cout << "Пойман сигнал! Получено 200 бит данных.\n";
    }
}

void Station::ModuleConsumption() {
    for (const auto& m : modules) energy -= m->GetEnergyInput();
}

void Station::RepairPhase() {
    for (auto& r : robots) {
        if (!r->IsAlive()) continue;
        if (energy >= 5) { r->RepairChassis(2); energy -= 5; }
        if (bits >= 5) { r->RepairFirmware(1); bits -= 5; }
    }
}

void Station::AgingPhase() const {
    for (auto& r : robots) r->Age();
}

void Station::RemoveDead() {
    robots.erase(std::remove_if(
            robots.begin(), robots.end(),
            [](const std::unique_ptr<Robot>& r) { return !r->IsAlive(); }
    ), robots.end());
}

void Station::CorporationTax() {
    if (day % 5 != 0) return;
    std::cout << "Орбитальная корпорация взимает налог.\n";
    if (energy > 0) {
        int tax = energy * 0.10;
        energy -= tax;
        std::cout << "Списано энергии: " << tax << std::endl;
    } else {
        int tax = bits * 0.20;
        bits -= tax;
        std::cout << "Списано данных: " << tax << std::endl;
    }
}

void Station::ProcessDay() {
    std::cout << "\n===== СТАНЦИЯ: " << name << " =====";
    std::cout << "\n=== ДЕНЬ " << day << " ===\n";
  
    for (auto& m : modules) {
        if (m->GetDisabledTimer() > 0) {
            m->SetDisabledTimer(m->GetDisabledTimer() - 1);
            if (m->GetDisabledTimer() == 0) m->SetActive(true);
        }
    }

    if (commFailureDaysLeft > 0) commFailureDaysLeft--;

    ProductionPhase();
    HousingCheck();
    SignalAttempt();
    ModuleConsumption();
    RepairPhase();
    AgingPhase();
    CorporationTax();
    RemoveDead();

    if (day > 10) {
        if (stormDaysLeft > 0) {
            stormDaysLeft--;
            if (stormDaysLeft == 0) {
                inStorm = false;
                std::cout << "\n[Квантовый шторм закончился]\n";
            }
        } else if (!inStorm) {
            std::uniform_int_distribution<int> stormChance(1, 100);
            if (stormChance(rng) <= 20) {
                inStorm = true;
                std::uniform_int_distribution<int> dur(2, 5);
                stormDaysLeft = dur(rng);
                std::cout << "\n!!! ВНИМАНИЕ: СТАНЦИЯ ВОШЛА В КВАНТОВЫЙ ШТОРМ (Дней: " << stormDaysLeft << ") !!!\n";
            }
        }
    }

    if (inStorm) {
        std::uniform_int_distribution<int> evDist(1, 100);
        if (evDist(rng) <= 50) {
            std::uniform_int_distribution<int> typeDist(1, 4);
            int type = typeDist(rng);
            if (type == 1) throw PowerSurgeException();
            else if (type == 2) throw FirmwareGlitchException();
            else if (type == 3) throw MeteorStrikeException();
            else if (type == 4) throw CommunicationFailureException();
        }
    }

    PrintStatus();
    day++;
}

void Station::PrintStatus() const {
    std::cout << "Энергия: " << energy << "/" << maxEnergy << std::endl;
    std::cout << "Данные: " << bits << std::endl;
    std::cout << "Шанс сигнала: " << CalculateSignalChance() << "%\n";

    std::cout << "\nРоботы:\n";
    for (const auto& r : robots) r->PrintStatus();

    std::cout << "\nМодули:\n";
    for (size_t i = 0; i < modules.size(); i++) {
        std::cout << i + 1 << ": " << *modules[i] << "\n";
    }
}

bool Station::IsGameOver() {
    return robots.empty() || (energy <= 0 && bits <= 0);
}

void Station::StartGame() {
    std::cout << "Введите название станции:\n";
    getline(std::cin, name);
    Init();

    while (!IsGameOver()) {
        std::cout << "\n===== РЕСУРСЫ =====\n";
        std::cout << "Энергия: " << energy << "/" << maxEnergy << "\n";
        std::cout << "Биты: " << bits << "\n";
        std::cout << "\n1 - следующий день\n2 - построить генератор (100 энергии)\n3 - улучшить модуль (50 бит)\n4 - синтез роботов\n5 - объединить модули\n6 - аналитика \n";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            default: {
                try {
                    ProcessDay();
                } catch (const PowerSurgeException& e) {
                    std::cout << "\nАВАРИЯ: " << e.what() << "\n";
                    energy -= 50; if (energy < 0) energy = 0;
                } catch (const FirmwareGlitchException& e) {
                    std::cout << "\nАВАРИЯ: " << e.what() << "\n";
                    for(auto& r : robots) if(r->IsAlive()) r->DamageFirmware(10);
                } catch (const MeteorStrikeException& e) {
                    std::cout << "\nАВАРИЯ: " << e.what() << "\n";
                    if (!modules.empty()) {
                        std::uniform_int_distribution<int> mDist(0, modules.size() - 1);
                        int target = mDist(rng);
                        modules[target]->SetActive(false);
                        modules[target]->SetDisabledTimer(1);
                    }
                } catch (const CommunicationFailureException& e) {
                    std::cout << "\nАВАРИЯ: " << e.what() << "\n";
                    commFailureDaysLeft = 3;
                } catch (const std::exception& e) {
                    std::cout << "\nНЕИЗВЕСТНЫЙ СБОЙ: " << e.what() << "\n";
                }
            } break;

            case 2: {
                if (energy >= 100) {
                    modules.push_back(std::make_shared<GeneratorModule>());
                    energy -= 100;
                    std::cout << "Генератор построен!\n";
                } else std::cout << "Недостаточно энергии\n";
            } break;

            case 3: {
                if (modules.empty()) continue;
                std::cout << "Выбери модуль:\n";
                for (size_t i = 0; i < modules.size(); i++) std::cout << i << " - " << modules[i]->GetType() << "\n";

                int index; std::cin >> index; std::cin.ignore();
                if (index >= 0 && index < modules.size() && bits >= 50) {
                    modules[index]->Upgrade();
                    bits -= 50;
                    std::cout << "Модуль улучшен\n";
                } else std::cout << "Ошибка или недостаточно бит\n";
            } break;

            case 4: {
                if (robots.size() < 2) break;
                int a, b;
                std::cout << "Выбери двух роботов:\n"; std::cin >> a >> b;
                std::unique_ptr<Robot> child = *robots[a] + *robots[b];
                if (child) {
                    std::cout << "Создан новый робот:\n" << *child << "\n";
                    robots.push_back(std::move(child));
                } else std::cout << "Синтез невозможен\n";
            } break;

            case 5: {
                int a, b; std::cin >> a >> b;
                std::shared_ptr<Module> result = *modules[a] + *modules[b];
                if (result) {
                    int max_idx = std::max(a, b);
                    int min_idx = std::min(a, b);
                    modules.erase(modules.begin() + max_idx);
                    modules.erase(modules.begin() + min_idx);
                    modules.push_back(result);
                    std::cout << "Модули объединены\n";
                } else std::cout << "Нельзя объединить\n";
            } break;

            case 6: {
                std::cout << "\n--- ТЕРМИНАЛ АНАЛИТИКИ ---\n";

                std::cout << "\n[Перекличка]\n";
                std::sort(robots.begin(), robots.end(), [](const std::unique_ptr<Robot>& a, const std::unique_ptr<Robot>& b) {
                    return a->GetName() < b->GetName();
                });
                std::for_each(robots.begin(), robots.end(), [](const std::unique_ptr<Robot>& r) { std::cout << *r << "\n"; });

                std::cout << "\n[Инвентаризация]\n";
                std::sort(modules.begin(), modules.end(), [](const std::shared_ptr<Module>& a, const std::shared_ptr<Module>& b) {
                    return a->GetType() < b->GetType();
                });
                std::for_each(modules.begin(), modules.end(), [](const std::shared_ptr<Module>& m) { std::cout << *m << "\n"; });

                std::cout << "\n[Оценка потерь]\n";
                int criticalCount = std::count_if(robots.begin(), robots.end(), [](const std::unique_ptr<Robot>& r) {
                    return r->GetChassis() < 50;
                });
                std::cout << "Роботов с критическим шасси (< 50%): " << criticalCount << "\n";

                std::cout << "\n[Статус связи]\n";
                bool allActive = std::all_of(modules.begin(), modules.end(), [](const std::shared_ptr<Module>& m) {
                    return m->IsActive();
                });
                if (allActive) {
                    std::cout << "Все модули активны. Системы связи стабильны.\n";
                }
                //else std::cout << "ахтунг!!на станции присутствуют неактивные модули!\n";
            } break;
        }
    }
    std::cout << "\nСтанция - В С Ё\n";
}