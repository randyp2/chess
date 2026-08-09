#include "chess/core/magic/Magic.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/magic/MagicHelpers.hpp"
#include "chess/core/magic/PrecomputedMagics.hpp"

#include <array>
#include <bit>
#include <vector>

namespace chess::core {

namespace {

/// @brief Stores the data needed for one magic-bitboard lookup.
struct Entry {
    Bitboard mask{};  // Possible attack squares
    Bitboard magic{}; // Magic number for hashing
    int shift{};      // Shift value to retrieve upper relevant bits
    std::vector<Bitboard> attacks; // List of attack tables
};

/// @brief Stores one magic-bitboard entry for every rook and bishop square.
struct Tables {
    std::array<Entry, 64> rooks;
    std::array<Entry, 64> bishops;
};

void buildEntry(Entry &entry, int square, bool rook, Bitboard magic) {
    entry.mask = createSliderMovementMask(square, rook);
    entry.magic = magic;

    const int relevant_bits = std::popcount(entry.mask);
    entry.shift = 64 - relevant_bits;

    // 2^n gives number of unique blocker positions
    const std::size_t permutations = 1ULL << relevant_bits;
    entry.attacks.assign(permutations, 0ULL);

    Bitboard blockers = 0ULL;
    do {

        const std::size_t key = (blockers * entry.magic) >> entry.shift;
        const Bitboard attacks =
            createSliderAttacksSlow(square, blockers, rook);

        entry.attacks[key] = attacks;
        blockers =
            (blockers - entry.mask) & entry.mask; // Generate next permutation

    } while (blockers != 0ULL);
}

/// @brief Construct the rooks and bishops table once and reuse other times
Tables &tables() {
    // Construct once and reuse value
    static Tables result = [] {
        Tables value;
        for (int square = 0; square < 64; ++square) {
            buildEntry(value.rooks[square], square, true, ROOK_MAGICS[square]);
            buildEntry(value.bishops[square], square, false,
                       BISHOP_MAGICS[square]);
        }

        return value;
    }();

    return result;
}

Bitboard lookup(const Entry &entry, Bitboard occupied) {
    const Bitboard blockers = occupied & entry.mask;
    const std::size_t key = (blockers * entry.magic) >> entry.shift;
    return entry.attacks[key];
}

} // namespace

void initializeMagicBitboards() { (void)tables(); }

Bitboard rookMagicAttacks(int square, Bitboard occupied) {
    assert(square >= 0 && square < 64);
    return lookup(tables().rooks[square], occupied);
}

Bitboard bishopMagicAttacks(int square, Bitboard occupied) {
    assert(square >= 0 && square < 64);
    return lookup(tables().bishops[square], occupied);
}
} // namespace chess::core
