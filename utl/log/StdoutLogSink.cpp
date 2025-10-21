#include "utl/log/StdoutLogSink.hpp"

#include <iostream>

namespace mage
{

void StdoutLogSink::Send(const absl::LogEntry& anEntry)
{
    std::cout << anEntry.text_message_with_prefix_and_newline();
}
} // namespace mage
