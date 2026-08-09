#include "chess/core/BitBoard.hpp"
#include "chess/core/magic/MagicHelpers.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using chess::core::Bitboard;

Bitboard findMagic(int square, bool rook, std::mt19937_64 &random) {
    const Bitboard base_mask =
        chess::core::createSliderMovementMask(square, rook);

    const int relevant_bits = std::popcount(base_mask);
    const int shift = 64 - relevant_bits;
    const std::size_t permutations = 1ULL << relevant_bits;

    std::vector<Bitboard> expected_attacks(permutations);

    // Construct possible blocker permutations
    Bitboard blockers = 0ULL;
    for (std::size_t i = 0; i < permutations; ++i) {
        const Bitboard attacks =
            chess::core::createSliderAttacksSlow(square, blockers, rook);
        expected_attacks[i] = attacks;

        blockers = (blockers - base_mask) & base_mask;
    }

    std::vector<Bitboard> attack_table(permutations);
    std::vector<bool> used(permutations);
    while (true) {
        const Bitboard candidate = random() & random() & random();

        // Sparse candidates are substantially more likely to produce a valid
        // magic hash.
        if (std::popcount((base_mask * candidate) &
                          0xFF00000000000000ULL) < 6)
            continue;

        std::fill(used.begin(), used.end(), false);
        blockers = 0ULL;
        bool valid = true;

        for (std::size_t i = 0; i < permutations; ++i) {
            const std::size_t key = (blockers * candidate) >> shift;

            if (!used[key]) {
                used[key] = true;
                attack_table[key] = expected_attacks[i];
            } else if (attack_table[key] != expected_attacks[i]) {
                valid = false;
                break;
            }

            blockers = (blockers - base_mask) & base_mask;
        }

        if (valid)
            return candidate;
    }
}

int main() {

    std::mt19937_64 random{0x9E3779B97F4A7C15ULL};

    std::array<Bitboard, 64> rook_magics{};
    std::array<Bitboard, 64> bishop_magics{};

    for (int square = 0; square < 64; ++square) {
        std::cout << "[SEARCHING SQUARE]" << square << "\n";
        rook_magics[square] = findMagic(square, true, random);
        bishop_magics[square] = findMagic(square, false, random);
    }

    std::ofstream output{
        "include/chess/core/magic/PrecomputedMagics.hpp"};
    if (!output) {
        std::cerr << "[OUTPUT FILE ERROR] Unable to open output file\n";
        return 1;
    }

    output << "#pragma once\n\n"
              "#include \"chess/core/BitBoard.hpp\"\n"
              "#include <array>\n\n"
              "namespace chess::core {\n\n";

    output << "inline constexpr std::array<Bitboard, 64> "
              "ROOK_MAGICS{\n";

    for (const Bitboard magic : rook_magics)
        output << "    0x" << std::hex << magic << "ULL,\n";

    output << "};\n\n";

    output << "inline constexpr std::array<Bitboard, 64> "
              "BISHOP_MAGICS{\n";

    for (const Bitboard magic : bishop_magics)
        output << "    0x" << std::hex << magic << "ULL,\n";

    output << "};\n\n"
              "} // namespace chess::core\n";

    std::cout << "Generated PrecomputedMagics.hpp\n";

    return 0;
}
