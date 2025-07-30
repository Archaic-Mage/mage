#pragma once

#include "absl/log/log.h"

#define MAGE_LOG_IMPL(Level_, Format_, ...) LOG(Level_) << std::format(Format_, ##__VA_ARGS__);

#define MAGE_LOG_INFO(Format_, ...) MAGE_LOG_IMPL(INFO, Format_, ##__VA_ARGS__)

#define MAGE_LOG_WARN(Format_, ...) MAGE_LOG_IMPL(WARNING, Format_, ##__VA_ARGS__)

#define MAGE_LOG_ERROR(Format_, ...) MAGE_LOG_IMPL(ERROR, Format_, ##__VA_ARGS__)

#define MAGE_LOG_FATAL(Format_, ...) MAGE_LOG_IMPL(FATAL, Format_, ##__VA_ARGS__)
