#pragma once

namespace chess::config {
class DebugConfig {
  public:
    /* Noexcept - Ensures that function deals with an exception and wont let it
     * reach back to caller*/
    static DebugConfig disabled() noexcept;

    bool print_bitboards() const noexcept;

    void enable_bitboards() noexcept;

  private:
    bool print_bitboards_ = false;
};
} // namespace chess::config
