#include "robot.h"
#include <algorithm>
#include <random>

void Robot::Age() {
    static std::default_random_engine rng(std::random_device{}());

    int wearBonus = ageDays / 20;

    std::uniform_int_distribution chassisWear(1 + wearBonus, 4 + wearBonus);
    std::uniform_int_distribution firmwareWear(1, 2 + wearBonus);

    ageDays++;

    chassisIntegrity -= chassisWear(rng);
    firmwareIntegrity -= firmwareWear(rng);

    if (chassisIntegrity <= 0 || firmwareIntegrity <= 0)
        alive = false;
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