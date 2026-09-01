#pragma once

#include "chess/core/BitBoard.hpp"

namespace chess::core::geometry {

/// @brief Generate Bitboard mask between from and to square exclusive
Bitboard between(int from, int to) noexcept;

/// @brief Generate Bitboard mask for the whole file, rank, diagonal containing
/// from and to square
Bitboard line(int from, int to) noexcept;
} // namespace chess::core::geometry
