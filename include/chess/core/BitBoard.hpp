#pragma once

#include <cassert>
#include <cstdint>

namespace chess::core {

// Type alias for better readabilty
using Bitboard = std::uint64_t;

namespace bb {

/* ================== FILE MASKS ================== */

/**
 * 10000000
 */
constexpr Bitboard FILE_A = 0x8080808080808080ULL;
constexpr Bitboard FILE_B = FILE_A >> 1;
constexpr Bitboard FILE_C = FILE_B >> 1;
constexpr Bitboard FILE_D = FILE_C >> 1;
constexpr Bitboard FILE_E = FILE_D >> 1;
constexpr Bitboard FILE_F = FILE_E >> 1;
constexpr Bitboard FILE_G = FILE_F >> 1;
constexpr Bitboard FILE_H = FILE_G >> 1;

// To prevent wrapping
constexpr Bitboard NOT_FILE_A = ~FILE_A;
constexpr Bitboard NOT_FILE_B = ~FILE_B;
constexpr Bitboard NOT_FILE_G = ~FILE_G;
constexpr Bitboard NOT_FILE_H = ~FILE_H;

/* ================== RANK MASKS ================== */
constexpr Bitboard RANK_1 = 0x00000000000000FFULL;
constexpr Bitboard RANK_2 = RANK_1 << 8;
constexpr Bitboard RANK_3 = RANK_2 << 8;
constexpr Bitboard RANK_4 = RANK_3 << 8;
constexpr Bitboard RANK_5 = RANK_4 << 8;
constexpr Bitboard RANK_6 = RANK_5 << 8;
constexpr Bitboard RANK_7 = RANK_6 << 8;
constexpr Bitboard RANK_8 = RANK_7 << 8;

// Return the index of the LSB and pop it
inline int pop_lsb(Bitboard &bb) noexcept {
    assert(bb != 0);
    int idx = __builtin_ctzll(bb);
    bb &= bb - 1;
    return idx;
}

// Return index of lsb w/o popping
inline int lsb(Bitboard bb) noexcept {
    assert(bb != 0);
    return __builtin_ctzll(bb);
}

/* ================== SHIFT HELPERS ================== */
// These are in perspective of white pieces
//  - White's south is black's north
//  - White's east is black's west
inline Bitboard shift_north(Bitboard bb) noexcept { return bb << 8; }
inline Bitboard shift_south(Bitboard bb) noexcept { return bb >> 8; }
inline Bitboard shift_east(Bitboard bb) noexcept {
    return (bb & NOT_FILE_H) >> 1;
}
inline Bitboard shift_west(Bitboard bb) noexcept {
    return (bb & NOT_FILE_A) << 1;
}
inline Bitboard shift_north_east(Bitboard bb) noexcept {
    return shift_north(shift_east(bb));
}
inline Bitboard shift_north_west(Bitboard bb) noexcept {
    return shift_north(shift_west(bb));
}
inline Bitboard shift_south_east(Bitboard bb) noexcept {
    return shift_south(shift_east(bb));
}
inline Bitboard shift_south_west(Bitboard bb) noexcept {
    return shift_south(shift_west(bb));
}

inline Bitboard shift_left(Bitboard bb, int val) noexcept { return bb << val; }

inline Bitboard shift_right(Bitboard bb, int val) noexcept { return bb >> val; }

inline Bitboard shift_up(Bitboard bb, int val) noexcept {
    return bb << 8 * val;
}
inline Bitboard shift_down(Bitboard bb, int val) noexcept {
    return bb >> 8 * val;
}

} // namespace bb
} // namespace chess::core
