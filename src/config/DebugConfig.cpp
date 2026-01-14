#include "../../include/chess/config/DebugConfig.hpp"

namespace chess::config {

// Return DebugConfig object with default values
DebugConfig DebugConfig::disabled() noexcept { return DebugConfig{}; }

bool DebugConfig::print_bitboards() const noexcept { return print_bitboards_; }

void DebugConfig::enable_bitboards() noexcept { print_bitboards_ = true; }
} // namespace chess::config
