#pragma once

#include <string>
#include <vector>

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
        std::vector<Event> history; // log of all events

    public:
        Time();
        Time(int startTime);

        static int getCurrentTime() { return currentTime; }
        static std::string getCurrentTimeFormatted();
        void logEvent(const std::string event);
        Event getEvent(uint32_t id) { return history[id]; } 

        void advanceTime() { ++currentTime; }
    };
    
} // namespace System
