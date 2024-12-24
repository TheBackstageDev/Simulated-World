#pragma once

#include <string>
#include <functional>

namespace Simulation_AI
{
    class Law
    {
    private:
        std::string name;
        std::string description;
        std::function<void()> effect;  
    public:
        Law(const std::string &name, const std::string &description, std::function<void()> effect)
            : name(name), description(description), effect(effect) {}

        void applyEffect() const { effect(); }

        std::string getName() const { return name; }
        std::string getDescription() const { return description; }
    };
    
} // namespace Simulation_AI
