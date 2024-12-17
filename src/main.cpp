#include "Headers/System/Simulation.hpp"

#include <stdexcept>
#include <iostream>

int main()
{    
    System::Simulation app;

    try
    {
        app.init();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}