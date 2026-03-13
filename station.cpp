#include "station.h"
#include <iostream>
#include <algorithm>

#include "module.h"
#include "robot.h"

Station::Station()
        : energy(500),
          maxEnergy(1000),
          bits(0),
          day(1)
{
}

void Station::Init() {
    modules.emplace_back(ModuleType::ARCHIVE);
    modules.emplace_back(ModuleType::CONTROL_CENTER);
    modules.emplace_back(ModuleType::HABITATION);
    modules.emplace_back(ModuleType::GENERATOR);

    robots.emplace_back("A-001", RobotType::INTEGRATOR);
    robots.emplace_back("A-002", RobotType::INTEGRATOR);
    robots.emplace_back("Омч", RobotType::KEEPER);
    robots.emplace_back("Чмо", RobotType::KEEPER);
}

int Station::CalculateSignalChance() {
    int totalPower = 0;

    for (const auto& m : modules) totalPower += m.GetEnergyOutput();
    for (const auto& r : robots) if (r.IsAlive()) totalPower += 10;

//    totalPower += energy / 20;

    int aliveRobots = 0;
    for (const auto& r : robots)
        if (r.IsAlive())aliveRobots++;

    int interference = aliveRobots * 10;

    double chance = 100* totalPower / (totalPower + interference);

    if (chance < 5) chance = 5;
    if (chance > 90) chance = 90;

    return (int)chance;
}

int Station::CalculateHabitation() {
    int slots = 0;
    for (const auto& m : modules) slots += m.GetHabitationSlots();

    return slots;
}

void Station::ProductionPhase() {
    for (const auto& m : modules) {
        energy += m.GetEnergyOutput();
        bits += m.GetDataOutput();
    }

    for (auto& r : robots) {
        if (!r.IsAlive()) continue;

        auto res = r.ProduceResources();

        energy += res.energy;
        bits += res.data;
    }

    if (energy > maxEnergy) energy = maxEnergy;
}

void Station::HousingCheck() {
    int capacity = CalculateHabitation();
    int used = 0;

    for (const auto& r : robots) if (r.IsAlive())used += r.GetSlotsUsed();

    if (used > capacity) {
        int overflow = used - capacity;

        std::cout << "Переполнение жилого отсека! Роботы получают урон.\n";

        for (auto& r : robots) {
            if (!r.IsAlive()) continue;
            r.DamageChassis(10 * 5);
            overflow -= r.GetSlotsUsed();
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
    for (const auto& m : modules) energy -= m.GetEnergyInput();
}

void Station::RepairPhase() {
    for (auto& r : robots) {
        if (!r.IsAlive())continue;

        if (energy >= 5) {
            r.RepairChassis(2);
            energy -= 5;
        }

        if (bits >= 5) {
            r.RepairFirmware(1);
            bits -= 5;
        }
    }
}

void Station::AgingPhase() {
    for (auto& r : robots) r.Age();
}

void Station::RemoveDead() {
    robots.erase(remove_if(
                    robots.begin(),
                    robots.end(),
                    [](const auto& r){
                        return !r.IsAlive();
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
    std::cout << "\n===== СТАНЦИЯ: " << name << " =====\n";
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

    for (auto& r : robots) r.PrintStatus();
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
        ProcessDay();

        std::cout << "\nНажмите ENTER чтобы перейти к следующему дню...";

        std::string tmp;
        std::getline(std::cin, tmp);
    }
    std::cout << "\nСтанция больше не функционирует.\n";
}