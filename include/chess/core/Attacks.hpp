#pragma once

#include "chess/core/BitBoard.hpp"

namespace chess::core::attacks {

/*
 * @brief Precompute the attack squares of leaper pieces and create magic
 * bitboards for sliding pieces
 */
void initialize();

/// @brief Retrieve all attack squares of a knight
Bitboard knight(int square) noexcept;

/// @brief Retrieve all attack squares of a king
Bitboard king(int square) noexcept;

/// @brief Retrieve attack squares before and including first blocker for a
/// bishop
Bitboard bishop(int square, Bitboard occupied);

/// @brief Retrieve attack squares before and including first blocker for a rook
Bitboard rook(int square, Bitboard occupied);

/// @brief Retrieve attack squares before and including first blocker for a
/// queen
Bitboard queen(int square, Bitboard occupied);

} // namespace chess::core::attacks
