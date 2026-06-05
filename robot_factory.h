#pragma once
#include <memory>
#include <string>
#include "robot.h"
#include "integrator.h"
#include "keeper.h"
#include "local_computer.h"

//абстрактная фабрика
class IRobotFactory {
public:
    virtual std::unique_ptr<Robot> CreateRobot(const std::string& name) = 0;
    virtual ~IRobotFactory() = default;
};

// конкретные фабрики
class IntegratorFactory : public IRobotFactory {
public:
    std::unique_ptr<Robot> CreateRobot(const std::string& name) override {
        return std::make_unique<Integrator>(name);
    }
};

class KeeperFactory : public IRobotFactory {
public:
    std::unique_ptr<Robot> CreateRobot(const std::string& name) override {
        return std::make_unique<Keeper>(name);
    }
};

class LocalComputerFactory : public IRobotFactory {
public:
    std::unique_ptr<Robot> CreateRobot(const std::string& name) override {
        return std::make_unique<LocalComputer>(name);
    }
};