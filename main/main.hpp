#pragma once

#include "protocols/test-proto/test.pb.h"

#include <iostream>
#include <string>

namespace mage
{
struct Logger
{
    inline static void info(std::string aMessage)
    {
        std::cout << "[INFO] " << aMessage << std::endl;
    }

    inline static void testInfo(test::AddressBook const& aAddressBook)
    {
        Logger::info(aAddressBook.SerializeAsString());
    }
};
} // namespace mage
