#include "chess/core/Attacks.hpp"
#include "chess/core/magic/Magic.hpp"

#include <array>
#include <cassert>

namespace chess::core::attacks {
namespace {

struct LeaperAttackTables {
    std::array<Bitboard, 64> knights{};
    std::array<Bitboard, 64> kings{};
};

Bitboard calculateKnightAttacks(Bitboard square) noexcept {
    Bitboard attacks = 0ULL;

    attacks |= bb::shift_left(bb::shift_up(square, 2), 1) & bb::NOT_FILE_H;
    attacks |= bb::shift_right(bb::shift_up(square, 2), 1) & bb::NOT_FILE_A;
    attacks |= bb::shift_left(bb::shift_up(square, 1), 2) &
               (bb::NOT_FILE_G & bb::NOT_FILE_H);
    attacks |= bb::shift_right(bb::shift_up(square, 1), 2) &
               (bb::NOT_FILE_A & bb::NOT_FILE_B);

    attacks |= bb::shift_left(bb::shift_down(square, 2), 1) & bb::NOT_FILE_H;
    attacks |= bb::shift_right(bb::shift_down(square, 2), 1) & bb::NOT_FILE_A;
    attacks |= bb::shift_left(bb::shift_down(square, 1), 2) &
               (bb::NOT_FILE_G & bb::NOT_FILE_H);
    attacks |= bb::shift_right(bb::shift_down(square, 1), 2) &
               (bb::NOT_FILE_A & bb::NOT_FILE_B);

    return attacks;
}

Bitboard calculateKingAttacks(Bitboard square) noexcept {
    return bb::shift_north(square) | bb::shift_south(square) |
           bb::shift_east(square) | bb::shift_west(square) |
           bb::shift_north_east(square) | bb::shift_north_west(square) |
           bb::shift_south_east(square) | bb::shift_south_west(square);
}

const LeaperAttackTables &leaperAttackTables() noexcept {
    static const LeaperAttackTables tables = [] {
        LeaperAttackTables result;

        for (int square = 0; square < 64; ++square) {
            const Bitboard squareBB = 1ULL << square;
            result.knights[square] = calculateKnightAttacks(squareBB);
            result.kings[square] = calculateKingAttacks(squareBB);
        }

        return result;
    }();

    return tables;
}

} // namespace

void initialize() {
    (void)leaperAttackTables();
    initializeMagicBitboards();
}

Bitboard knight(int square) noexcept {
    assert(square >= 0 && square < 64);
    return leaperAttackTables().knights[square];
}

Bitboard king(int square) noexcept {
    assert(square >= 0 && square < 64);
    return leaperAttackTables().kings[square];
}

Bitboard bishop(int square, Bitboard occupied) {
    return bishopMagicAttacks(square, occupied);
}

Bitboard rook(int square, Bitboard occupied) {
    return rookMagicAttacks(square, occupied);
}

Bitboard queen(int square, Bitboard occupied) {
    return bishopMagicAttacks(square, occupied) |
           rookMagicAttacks(square, occupied);
}

} // namespace chess::core::attacks
