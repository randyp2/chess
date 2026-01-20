#pragma once

#include <cassert>
#include <cstdint>
namespace chess::core::BitBoard {

// Type alias for better readabilty
using BitBoard = std::uint64_t;

/* ================== FILE MASKS ================== */
constexpr BitBoard FILE_A = 0x0101010101010101ULL;
constexpr BitBoard FILE_B = FILE_A << 1;
constexpr BitBoard FILE_C = FILE_B << 2;
constexpr BitBoard FILE_D = FILE_C << 3;
constexpr BitBoard FILE_E = FILE_D << 4;
constexpr BitBoard FILE_F = FILE_E << 5;
constexpr BitBoard FILE_G = FILE_F << 6;
constexpr BitBoard FILE_H = FILE_G << 7;

// To prevent wrapping
constexpr BitBoard NOT_FILE_A = ~FILE_A;
constexpr BitBoard NOT_FILE_H = ~FILE_H;

/* ================== RANK MASKS ================== */
constexpr BitBoard RANK_1 = 0x0000000000000011ULL;
constexpr BitBoard RANK_2 = RANK_1 << 8;
constexpr BitBoard RANK_3 = RANK_2 << 16;
constexpr BitBoard RANK_4 = RANK_3 << 24;
constexpr BitBoard RANK_5 = RANK_4 << 32;
constexpr BitBoard RANK_6 = RANK_5 << 40;
constexpr BitBoard RANK_7 = RANK_6 << 48;
constexpr BitBoard RANK_8 = RANK_7 << 56;

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
inline BitBoard shift_up(BitBoard bb) noexcept { return bb << 8; }
inline BitBoard shift_down(BitBoard bb) noexcept { return bb >> 8; }
inline BitBoard shift_left(BitBoard bb) noexcept {
    return (bb & NOT_FILE_A) << 1;
}
inline BitBoard shift_right(BitBoard bb) noexcept {
    return (bb & NOT_FILE_H) >> 1;
}

} // namespace chess::core::BitBoard
