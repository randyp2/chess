#pragma once

/*
    Magic bitboards are a way to allow for fast lookups for sliding pieces.

    Traditional way is to scan each cell that a sliding piece can attack and
    determine in blockers. Doing so creates overhead during runtime as these
    directions have to be scanned each time a move is made.

    Magic bitboards eliminates this runtime overhead and trades it for
    initialization overhead. This is much preferred as chess engines have to
    perform fast lookups regardings moves.

*/
namespace chess::core {

/*
 * @brief Initialize precomputed magic-bitboard attack tables for rooks and
 * bishops.
 *
 * Generates every possible blocker configuration for sliding pieces
 * (rooks and bishops) and stores them in an array using a hashing technique for
 * fast lookups.
 */
void initializeMagicBitboards();
} // namespace chess::core
