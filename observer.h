#pragma once
#include <vector>
#include <string>
#include <algorithm>

// интерфейс наблюдателя подписчика
class IObserver {
public:
    virtual void OnAlarm(const std::string& threat) = 0;
    virtual ~IObserver() = default;
};

// шина издатель
class AlarmSystem {
    std::vector<IObserver*> observers;
public:
    void Subscribe(IObserver* obs) {
        observers.push_back(obs);
    }

    void Unsubscribe(IObserver* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void Notify(const std::string& threat) {
        for (auto obs : observers) obs->OnAlarm(threat);
    }
};