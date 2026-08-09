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
#include "chess/core/BitBoard.hpp"
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

/**
 * @brief Given the occupied squares on a bitboard find the given attacks a
 * rook can do
 *
 * @params square Represents the curent square the rook is on
 * @params occupied Bitboard containing the occupied squares on the board
 *
 * @returns Bitboard containing the possible attack squares
 */
Bitboard rookMagicAttacks(int square, Bitboard occupied);

/**
 * @brief Given the occupied squares on a bitboard find the given attacks a
 * bishop can do
 *
 * @params square Represents the curent square the bishop is on
 * @params occupied Bitboard containing the occupied squares on the board
 *
 * @returns Bitboard containing the possible attack squares
 */
Bitboard bishopMagicAttacks(int square, Bitboard occupied);
} // namespace chess::core
