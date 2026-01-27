#pragma once

#include <cassert>
#include <cstdint>
namespace chess::core::BitBoard {

// Type alias for better readabilty
using BitBoard = std::uint64_t;

/* ================== FILE MASKS ================== */
constexpr BitBoard FILE_A = 0x1010101010101010ULL;
constexpr BitBoard FILE_B = FILE_A >> 1;
constexpr BitBoard FILE_C = FILE_B >> 1;
constexpr BitBoard FILE_D = FILE_C >> 1;
constexpr BitBoard FILE_E = FILE_D >> 1;
constexpr BitBoard FILE_F = FILE_E >> 1;
constexpr BitBoard FILE_G = FILE_F >> 1;
constexpr BitBoard FILE_H = FILE_G >> 1;

// To prevent wrapping
constexpr BitBoard NOT_FILE_A = ~FILE_A;
constexpr BitBoard NOT_FILE_H = ~FILE_H;

/* ================== RANK MASKS ================== */
constexpr BitBoard RANK_1 = 0x00000000000000FFULL;
constexpr BitBoard RANK_2 = RANK_1 << 8;
constexpr BitBoard RANK_3 = RANK_2 << 8;
constexpr BitBoard RANK_4 = RANK_3 << 8;
constexpr BitBoard RANK_5 = RANK_4 << 8;
constexpr BitBoard RANK_6 = RANK_5 << 8;
constexpr BitBoard RANK_7 = RANK_6 << 8;
constexpr BitBoard RANK_8 = RANK_7 << 8;

// Return the index of the LSB and pop it
inline int pop_lsb(BitBoard &bb) noexcept {
    assert(bb != 0);
    int idx = __builtin_ctzll(bb);
    bb &= bb - 1;
    return idx;
}

// Return index of lsb w/o popping
inline int lsb(BitBoard bb) noexcept {
    assert(bb != 0);
    return __builtin_ctzll(bb);
}

/* ================== SHIFT HELPERS ================== */
// These are in perspective of white pieces
//  - White's south is black's north
//  - White's east is black's west
inline BitBoard shift_north(BitBoard bb) noexcept { return bb << 8; }
inline BitBoard shift_south(BitBoard bb) noexcept { return bb >> 8; }
inline BitBoard shift_east(BitBoard bb) noexcept {
    return (bb & NOT_FILE_H) >> 1;
}
inline BitBoard shift_west(BitBoard bb) noexcept {
    return (bb & NOT_FILE_A) << 1;
}
inline BitBoard shift_north_east(BitBoard bb) noexcept {
    return shift_north(shift_east(bb));
}
inline BitBoard shift_north_west(BitBoard bb) noexcept {
    return shift_north(shift_west(bb));
}
inline BitBoard shift_south_east(BitBoard bb) noexcept {
    return shift_south(shift_east(bb));
}
inline BitBoard shift_south_west(BitBoard bb) noexcept {
    return shift_south(shift_west(bb));
}

} // namespace chess::core::BitBoard
