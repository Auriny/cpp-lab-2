#pragma once
#include <exception>

class PowerSurgeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Скачок напряжения! Утрачено 50 Энергии.";
    }
};

class FirmwareGlitchException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Сбой матриц! Прошивка всех роботов повреждена на 10%.";
    }
};

class MeteorStrikeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Микро-метеорит! Случайный модуль станции отключен на 1 день.";
    }
};

// в4
class CommunicationFailureException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Навигационный сбой! Связь потеряна, шанс поймать сигнал равен 0 на 3 дня.";
    }
};

class TestException : public std::exception {
public:
    const char* what() const noexcept override {
        return "test";
    }
};