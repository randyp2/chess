#include "chess/core/magic/Magic.hpp"
#include "chess/core/BitBoard.hpp"
#include <array>
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

void buildEntry(Entry &entry, int square, bool rook, Bitboard magic) {}

/// @brief Construct the rooks and bishops table once and reuse other times
Tables &tables() {
    // Construct once and reuse value
    static Tables result = [] {
        Tables value;
        for (int square = 0; square < 64; ++square) {
        }

        return Tables{};
    }();
}

} // namespace

void initializeMagicBitboards() {}
} // namespace chess::core
