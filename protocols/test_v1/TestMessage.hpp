#pragma once

#include <array>

struct TestMessage
{
    std::int32_t theMessageNumber = 1;
    std::array<char, 5> theUserName;
    std::uint32_t theUserId;
};

struct TestMessage2
{
    std::int32_t theMessageNumber = 2;
    std::uint32_t theUserId;
    std::array<char, 30> theUserInfo;
};
