#include "../src/Headers/System/Time.hpp"
#include <cmath>

namespace System
{
    int Time::currentTime{0};
    uint32_t Time::lastEventID{0};

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
        sprintf(dateText, "%02d/%02d/%04d", day, month, year);

        formattedString = dateText;
        return formattedString;
    }

} // namespace System
