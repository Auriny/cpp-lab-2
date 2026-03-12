#include "Robot.h"
#include <iostream>
#include <algorithm>
#include <utility>
#include <random>

Robot::Robot(std::string n, RobotType t)
          : name(std::move(n)),
          type(t),
          chassisIntegrity(100),
          firmwareIntegrity(100),
          alive(true),
          ageDays(0)
{
}

bool Robot::IsAlive() const {
    return alive;
}

RobotType Robot::GetType() const {
    return type;
}

std::string Robot::GetName() const {
    return name;
}

int Robot::GetSlotsUsed() const {
    if (type == RobotType::INTEGRATOR) {
        return 1;
    } else return 2;
}

Resources Robot::ProduceResources() const {
    if (!alive) return {0,0};
    double multiplier = ageDays > 10 ? 1.5 : 1.0;

    if (type == RobotType::INTEGRATOR) {
        return {int(10 * multiplier),int(3 * multiplier)};
    } else return {int(3 * multiplier), int(10 * multiplier)};
}

// чем старше робот тем быстрее он сломается :3
void Robot::Age() {
    static std::default_random_engine rng(std::random_device{}());

    int wearBonus = ageDays / 20;

    std::uniform_int_distribution<int> chassisWear(1 + wearBonus, 4 + wearBonus);
    std::uniform_int_distribution<int> firmwareWear(1, 2 + wearBonus);

    ageDays++;

    chassisIntegrity -= chassisWear(rng);
    firmwareIntegrity -= firmwareWear(rng);

    if (chassisIntegrity <= 0 || firmwareIntegrity <= 0) alive = false;
}

void Robot::DamageChassis(int dmg) {
    chassisIntegrity -= dmg;
    if (chassisIntegrity <= 0) alive = false;
}

void Robot::RepairChassis(int amount) {
    chassisIntegrity = std::min(100, chassisIntegrity + amount);
}

void Robot::RepairFirmware(int amount) {
    firmwareIntegrity = std::min(100, firmwareIntegrity + amount);
}

void Robot::PrintStatus() const {
    std::cout << name << " | ";

    if (type == RobotType::INTEGRATOR) {
        std::cout << "ИНТЕГРАТОР | ";
    } else std::cout << "ХРАНИТЕЛЬ | ";

    std::cout << "Шасси:" << chassisIntegrity
              << " Прошивка:" << firmwareIntegrity;

    if (!alive) std::cout << " МЁРТВ";

    std::cout << std::endl;
}