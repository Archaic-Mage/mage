#pragma once

#include "absl/log/log_entry.h"
#include "absl/log/log_sink.h"

namespace mage
{
class StdoutLogSink final : public absl::LogSink
{
public:
    void Send(const absl::LogEntry& anEntry) override;
};
} // namespace mage
