#pragma once

#include "absl/log/check.h"

#define MAGE_CHECK_IMPL(Condition_, Format_, ...) CHECK(Condition_) << std::format(Format_, ##__VA_ARGS__);

#define MAGE_ASSERT(Condition_, Format_, ...) MAGE_CHECK_IMPL(Condition_, Format_, ##__VA_ARGS__)

#define MAGE_THROW_EXCEPTION(Format_, ...) MAGE_CHECK_IMPL(false, Format_, ##__VA_ARGS__)
