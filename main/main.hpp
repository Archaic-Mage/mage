#pragma once

#include <string>
#include <iostream>

struct Logger 
{
    inline static void info(std::string aMessage)
    {
        std::cout << "[INFO] " << aMessage << std::endl;
    }
};