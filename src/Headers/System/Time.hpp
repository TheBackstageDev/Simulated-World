#pragma once

#include <string>
#include <vector>

namespace System
{
    struct Event
    {
        Event(int time, std::string log)
        : timeOfEvent(time), eventLog(log)
        {};

        int timeOfEvent{0.0f};
        std::string eventLog{"Log is Empty."};
    };
    
    class Time
    {
    private:
        int currentTime; // in days
        std::vector<Event> history; // log of all events

        void addEvent(const std::string& event); 
    public:
        Time();
        Time(int startTime);

        float getCurrentTime() const { return currentTime; }
        void logEvent(const std::string &event);

        void advanceTime() { ++currentTime; }
    };
    
} // namespace System
