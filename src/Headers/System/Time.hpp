#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace System
{
    struct Event
    {
        Event(int time, std::string log, uint32_t id)
        : timeOfEvent(time), eventLog(log), id(id)
        {};

        uint32_t id{0};
        int timeOfEvent{0};
        std::string eventLog{"Log is Empty."};
    };
    
    class Time
    {
    private:
        static uint32_t lastEventID;
        static int currentTime; // in days
        static std::vector<Event> history; // log of all events
        static std::unordered_map<uint32_t, std::vector<Event>> populationHistory; // The log of all events for each pop

    public:
        Time();
        Time(int startTime);

        static int getCurrentTime() { return currentTime; }
        static std::string getCurrentTimeFormatted();
        static void logEvent(const std::string event);
        static void logPopEvent(const std::vector<Event>& popHistory, uint32_t id);
        Event getEvent(uint32_t id) { return history[id]; } 

        void advanceTime() { ++currentTime; }
    };
    
} // namespace System
