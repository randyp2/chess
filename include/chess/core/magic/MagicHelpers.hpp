#pragma once

#include "chess/core/BitBoard.hpp"
#include <array>

namespace chess::core {

struct SliderDirection {
    int rank;
    int file;
};

inline constexpr std::array<SliderDirection, 4> ROOK_DIRECTIONS{{
    {1, 0},  // North
    {-1, 0}, // South
    {0, 1},  // East
    {0, -1}  // West
}};

inline constexpr std::array<SliderDirection, 4> BISHOP_DIRECTIONS{{
    {1, -1},  // Northeast
    {1, 1},   // Northwest
    {-1, -1}, // Southeast
    {-1, 1},  // Southwest
}};

inline constexpr bool onBoard(int rank, int file) noexcept {
    return rank >= 0 && rank < 8 && file >= 0 && file < 8;
}

/// @brief Conver the rank and file to a square index (0-64)
inline constexpr int squareIdx(int rank, int file) noexcept {
    return rank * 8 + (7 - file);
}

/**
 * Creates a mask based for ROOK | BISHOP and relevant to the blockers passed
 * int
 *
 * @param square Initial starting square of the piece
 * @param blockers Bitboard containing all blockers relevant to attack mas
 * @param rook 1 -> Rook Piece | 0 -> Bishop piece
 * @param excludeEdge Whether or not to include edge in mask
 *
 * @return uint64 attack mask for specified piece
 */
inline Bitboard traceSliderRays(int square, Bitboard blockers, bool rook,
                                bool excludeEdge) noexcept {
    const auto &directions = rook ? ROOK_DIRECTIONS : BISHOP_DIRECTIONS;
    const int start_rank = square / 8;
    const int start_file = 7 - square % 8;

    Bitboard attacks = 0ULL;

    for (const auto dir : directions) {
        int rank = start_rank + dir.rank;
        int file = start_file + dir.file;

        // Loop in current direction
        while (onBoard(rank, file)) {

            if (excludeEdge && !onBoard(rank + dir.rank, file + dir.file))
                break;

            const Bitboard target = 1ULL << squareIdx(rank, file);
            attacks |= target;

            // Stop on first instance of blocker
            if (!excludeEdge && (blockers & target))
                break;

            rank += dir.rank;
            file += dir.file;
        }
    }

    return attacks;
}

inline Bitboard createSliderMovementMask(int square, bool rook) noexcept {
    return traceSliderRays(square, 0ULL, rook, true);
}

inline Bitboard createSliderAttacksSlow(int square, Bitboard blockers,
                                        bool rook) noexcept {
    return traceSliderRays(square, blockers, rook, false);
}

} // namespace chess::core
