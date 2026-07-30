#pragma once

#include "chess/core/BitBoard.hpp"

namespace chess::core::attacks {

// Precompute attacks that do not depend on the position's occupancy.
void initialize() noexcept;

Bitboard knight(int square) noexcept;
Bitboard king(int square) noexcept;

Bitboard bishop(int square, Bitboard occupied);
Bitboard rook(int square, Bitboard occupied);
Bitboard queen(int square, Bitboard occupied);

} // namespace chess::core::attacks
