#pragma once

#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log_sink_registry.h"

namespace mage
{
class StdoutLogSink final : public absl::LogSink
{
public:
    void Send(const absl::LogEntry& anEntry) override;
};
} // namespace mage
