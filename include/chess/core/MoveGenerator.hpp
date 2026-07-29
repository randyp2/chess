#pragma once

#include "../core/BitBoard.hpp"
#include "../core/Move.hpp"
#include "../core/Position.hpp"
#include "chess/config/DebugConfig.hpp"
#include <cstddef>

namespace chess::core {

inline constexpr std::size_t MAX_MOVES = 256;
// Keep track of pseudo-legal moves and move count
// 256 known to be a safe upper bound number to store these moves
struct MoveList {

    Move moves[MAX_MOVES];
    std::size_t size = 0; // Current moves stored

    void clear() { size = 0; }
    void add(const Move &m) { moves[size++] = m; }
};

class MoveGenerator {
  public:
    /**
     * Generate all pseudo-legal moves given a positoin
     *
     * A pseudolegal move in chess is a move that follows the basic movement and
     * capture rules for a specific piece, but may still be illegal because
     * it leaves or puts the player's own king in check
     *
     * @param pos - Position class that owns bitboard game state
     * @param moves - Stores all legal moves
     * @param debug - Class for logging and debugging
     */
    static void generatePseudoLegal(const Position &pos, MoveList &moves,
                                    const chess::config::DebugConfig &debug);

    // Generate all legal moves given a position
    static void generateLegal(const Position &pos, MoveList &moves);

    static void generatePawnMoves(const Position &pos, MoveList &moves,
                                  const chess::config::DebugConfig &debug);

    // Initialize attack tables
    static void initAttackTables();

  private:
    // --- Helpers for move generation
    static void generateKnightMoves(const Position &pos, MoveList &moves);
    static void generateBishopMoves(const Position &pos, MoveList &moves);
    static void generateRookMoves(const Position &pos, MoveList &moves);
    static void generateQueenMoves(const Position &pos, MoveList &moves);
    static void generateKingMoves(const Position &pos, MoveList &moves);

    // --- Sliding attacks
    static Bitboard bishopAttacks(Bitboard bishops, Bitboard occupied);
    static Bitboard rookAttacks(Bitboard rooks, Bitboard occupied);

    static bool isInCheck(const Position &pos);

    // --- Attacking moves
    static Bitboard
        knightAttacks[64]; // Possible knight attacks from each position
    static Bitboard
        kingAttacks[64]; // Possible king attacks from each position
};
} // namespace chess::core
