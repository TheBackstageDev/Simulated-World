#include "../src/Headers/System/Time.hpp"
#include <cmath>

namespace System
{
    int Time::currentTime{0};
    uint32_t Time::lastEventID{0};

    std::vector<Event> Time::history;
    std::unordered_map<uint32_t, std::vector<Event>> Time::populationHistory;

    Time::Time() 
    {
        logEvent("Let there be Light!");
    };
    Time::Time(int startTime)
    {
        currentTime = startTime;
    }

    void Time::logEvent(const std::string event)
    {
        Event newEvent{currentTime, event, ++lastEventID};
        history.push_back(newEvent);
    }

    void Time::logPopEvent(const std::vector<Event>& popHistory, uint32_t id)
    {
        populationHistory.emplace(id, std::move(popHistory));
    }

    std::string System::Time::getCurrentTimeFormatted()
    {
        std::string formattedString;

        int daysPassed = getCurrentTime();

        int year = daysPassed / 365; 
        int remainingDays = daysPassed % 365;

        int month = remainingDays / 30 + 1; 
        int day = remainingDays % 30 + 1;   

        // Format the date as dd/mm/yyyy
        char dateText[20];
        sprintf_s(dateText, "%02d/%02d/%04d", day, month, year);

        formattedString = dateText;
        return formattedString;
    }

} // namespace System
