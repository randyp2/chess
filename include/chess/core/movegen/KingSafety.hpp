#pragma once

#include "chess/core/BitBoard.hpp"

namespace chess::core {

/// @brief Contains information about king safety
struct KingSafety {
    int king_square;

    /*
    0 - Generate moves normally
    1 - King can move out of check or pieces can block or capture attacker
    2+ - Only king can move (double check)
    */
    int checker_count;

    Bitboard checkers = 0ULL; // Bitboard every checker

    // Squares non-king pieces can move to resolve check
    Bitboard resolve_masks = ~Bitboard{0};
    Bitboard pinned_pieces = 0ULL; // Pieces that are pinned and can't move
    Bitboard enemy_attacks = 0ULL; // Squares enemy pieces are attacking
};

// Forward declaration whole class defintion not need in header file
class Position;

/// @brief Retrieve updated KingSafety info to evaluate legal moves
[[nodiscard]]
KingSafety getKingSafteyInfo(const Position &pos);
} // namespace chess::core
