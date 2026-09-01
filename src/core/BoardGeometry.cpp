#include "chess/core/BoardGeometry.hpp"

#include <array>
#include <cassert>
#include <cstdlib>

namespace chess::core::geometry {

namespace {
constexpr std::size_t TOTAL_SQUARES = 64;

/// @brief Bitboards for all possible positions between two points
struct GeometryTables {
    std::array<std::array<Bitboard, TOTAL_SQUARES>, TOTAL_SQUARES> between{};
    std::array<std::array<Bitboard, TOTAL_SQUARES>, TOTAL_SQUARES> line{};
};

/// @brief Determines if current rank and file are on the board
constexpr bool onBoard(int rank, int file) noexcept {
    return rank >= 0 && rank < 8 && file >= 0 && file < 8;
}
/*
Determine sign of a number:
1: Positive
0: Zero
-1: Negative

Used for navigation direction
*/
constexpr int sign(const int num) noexcept { return (num > 0) - (num < 0); }

GeometryTables buildTables() {
    GeometryTables result{};

    for (std::size_t sq1 = 0; sq1 < TOTAL_SQUARES; ++sq1) {
        const int first_rank = sq1 / 8;
        const int first_file = sq1 % 8;

        for (std::size_t sq2 = sq1 + 1; sq2 < TOTAL_SQUARES; ++sq2) {
            const int second_rank = sq2 / 8;
            const int second_file = sq2 % 8;

            // --- Calculate distances
            const int rank_delta = second_rank - first_rank;
            const int file_delta = second_file - first_file;

            // --- Determine if in line
            bool same_rank = rank_delta == 0;
            bool same_file = file_delta == 0;
            bool same_diag = std::abs(rank_delta) == std::abs(file_delta);

            if (!same_rank && !same_file && !same_diag)
                continue;

            // --- Create between masks and line masks
            int rank_step = sign(rank_delta);
            int file_step = sign(file_delta);

            int r = first_rank + rank_step;
            int f = first_file + file_step;

            Bitboard between_mask = 0ULL;
            while (r != second_rank || f != second_file) {
                const int square = r * 8 + f;
                between_mask |= 1ULL << square;

                r += rank_step;
                f += file_step;
            }

            // Go to beginning of line
            r = first_rank;
            f = first_file;
            while (onBoard(r - rank_step, f - file_step)) {
                r -= rank_step;
                f -= file_step;
            }

            Bitboard line_mask = 0ULL;
            while (onBoard(r, f)) {
                const int square = r * 8 + f;
                line_mask |= 1ULL << square;

                r += rank_step;
                f += file_step;
            }

            result.between[sq1][sq2] = between_mask;
            result.between[sq2][sq1] = between_mask;

            result.line[sq1][sq2] = line_mask;
            result.line[sq2][sq1] = line_mask;
        }
    }

    return result;
}

const GeometryTables &tables() noexcept {
    static GeometryTables result = buildTables();
    return result;
}
} // namespace

Bitboard between(int from, int to) noexcept {
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);

    return tables().between[from][to];
}

Bitboard line(int from, int to) noexcept {
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);

    return tables().line[from][to];
}
} // namespace chess::core::geometry
