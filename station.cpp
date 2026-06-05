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
#include "robot_factory.h"
#include "main_reactor.h"

//синглтонность
Station::Station() : energy(500), maxEnergy(1000), bits(0), day(1) {}

Station& Station::GetInstance() {
    static Station instance;
    return instance;
}

void Station::AddRobot(std::unique_ptr<Robot> r) {
    alarm.Subscribe(r.get());
    robots.push_back(std::move(r));
}

void Station::Init() {
    modules.push_back(std::make_shared<ArchiveModule>());
    modules.push_back(std::make_shared<ControlCenterModule>());
    modules.push_back(std::make_shared<HabitationModule>());

    // в9
    modules.push_back(std::make_shared<ReactorProxy>());
    for(auto& m : modules) alarm.Subscribe(m.get());

    // фабричный метод
    IntegratorFactory intFac;
    KeeperFactory keepFac;
    LocalComputerFactory lcFac;

    AddRobot(intFac.CreateRobot("A-001"));
    AddRobot(intFac.CreateRobot("A-002"));
    AddRobot(keepFac.CreateRobot("Чмо"));
    AddRobot(lcFac.CreateRobot("LC-01"));
}

int Station::CalculateSignalChance() const {
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
    for (auto& r : robots) {
        if (!r->IsAlive()) alarm.Unsubscribe(r.get());
    }

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

                alarm.Notify("Квантовый шторм!");
            }
        }
    }

    if (inStorm) {
        std::uniform_int_distribution evDist(1, 100);
        if (evDist(rng) <= 50) {
            std::uniform_int_distribution typeDist(1, 4);
            int type = typeDist(rng);
            switch (type) {
                case 1: throw PowerSurgeException(); break;
                case 2: throw FirmwareGlitchException(); break;
                case 3: throw MeteorStrikeException(); break;
                case 4: throw CommunicationFailureException(); break;
            }
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
    return !isGameRunning || robots.empty() || (energy <= 0 && bits <= 0);
}

// the ending ;3
void Station::EndGame() {
    isGameRunning = false;
    std::cout << ">>> КВАНТОВЫЙ НАВИГАЦИОННЫЙ МАЯК УСПЕШНО АКТИВИРОВАН <<<\n";
    std::cout << "Ковчег успешно покинул Глубокий Сектор и вышел на орбиту Земли-2.\n";
    std::cout << "-----------\n";
    std::cout << "[ФИНАЛЬНАЯ СТАТИСТИКА ЭКСПЕДИЦИИ]\n";
    std::cout << "Прожито дней: " << day << "\n";
    std::cout << "Выживших роботов: " << robots.size() << "\n";
    std::cout << "Построено модулей: " << modules.size() << "\n";

    int score = (robots.size() * 200) + (modules.size() * 100) - (day * 15);
    if(score < 0) score = 0;
    std::cout << "Итоговый рейтинг станции ( очки): " << score << "\n";
    std::cout << "-----------\n";
}

void Station::StartGame() {
    std::cout << "Введите название станции:\n";
    getline(std::cin, name);
    Init();

    while (!IsGameOver()) {
        std::cout << "\n===== РЕСУРСЫ =====\n";
        std::cout << "Энергия: " << energy << "/" << maxEnergy << "\n";
        std::cout << "Биты: " << bits << "\n";
        std::cout << "\n1 - Следующий день\n2 - Построить обычный генератор (100 энергии)\n3 - Улучшить модуль (50 бит)\n4 - Синтез роботов\n5 - Объединить модули\n6 - Аналитика\n7 - Управление Главным Реактором (Proxy)\n8 - Построить Квантовый Маяк [КОНЦОВКА] (500 Эн / 500 Бит)\n";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
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
                    auto newGen = std::make_shared<GeneratorModule>();
                    modules.push_back(newGen);
                    alarm.Subscribe(newGen.get());
                    energy -= 100;
                    std::cout << "Обычный генератор построен!\n";
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
                std::cout << "Выбери двух роботов:\n";

                for (size_t i = 0; i < robots.size(); i++)
                    std::cout << i << " - " << robots[i]->GetName() << "\n";

                std::cin >> a >> b;

                if (a < 0 || b < 0 || a >= robots.size() || b >= robots.size()) {
                    std::cout << "Неверный индекс\n";
                    break;
                }

                auto child = *robots[a] + *robots[b];

                if (child) {
                    std::cout << "Создан новый робот:\n" << *child << "\n";
                    AddRobot(std::move(child));
                } else std::cout << "Синтез невозможен\n";

            } break;

            case 5: {
                int a, b; std::cin >> a >> b;
                std::shared_ptr<Module> result = *modules[a] + *modules[b];
                if (result) {
                    int max_idx = std::max(a, b);
                    int min_idx = std::min(a, b);

                    alarm.Unsubscribe(modules[max_idx].get());
                    alarm.Unsubscribe(modules[min_idx].get());

                    modules.erase(modules.begin() + max_idx);
                    modules.erase(modules.begin() + min_idx);

                    modules.push_back(result);
                    alarm.Subscribe(result.get());

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
                } else std::cout << "ахтунг!!на станции присутствуют неактивные модули!\n";
            } break;

            case 7: {
                std::cout << "Запрос доступа к Главному Реактору. Отправить робота:\n";
                for (size_t i = 0; i < robots.size(); i++) {
                    std::cout << i << " - " << robots[i]->GetName() << " (" << robots[i]->GetType() << ")\n";
                }
                int r_idx; std::cin >> r_idx; std::cin.ignore();

                if (r_idx >= 0 && r_idx < robots.size()) {
                    auto it = std::find_if(modules.begin(), modules.end(), [](const std::shared_ptr<Module>& m) {
                        return m->GetType() == "Прокси-интерфейс Реактора";
                    });

                    if (it != modules.end()) {
                        auto proxy = std::dynamic_pointer_cast<ReactorProxy>(*it);
                        if (proxy) {
                            try {
                                proxy->Overdrive(robots[r_idx].get());
                                energy += 100;
                            } catch (const std::exception& e) {
                                std::cout << "\n[КРИТИЧЕСКАЯ ОШИБКА ДОСТУПА] " << e.what() << "\n";
                            }
                        }
                    } else std::cout << "Интерфейс Главного Реактора не обнаружен.\n";
                }
            } break;

            case 8: {
                if (energy >= 500 && bits >= 500) {
                    energy -= 500;
                    bits -= 500;
                    EndGame();
                } else std::cout << "Недостаточно ресурсов! Требуется 500 Энергии и 500 Бит.\n";
            } break;
        }
    }

    if (robots.empty() || (energy <= 0 && bits <= 0)) {
        std::cout << "\nСТАНЦИЯ УНИЧТОЖЕНА. Миссия провалена.\n";
    }
}