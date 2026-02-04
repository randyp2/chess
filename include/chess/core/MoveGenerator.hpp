#pragma once

#include "../core/BitBoard.hpp"
#include "../core/Move.hpp"
#include "../core/Position.hpp"
#include "chess/config/DebugConfig.hpp"
#include <cstdint>

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
    // Generate all pseudo-legal moves given a position
    static void generatePseudoLegal(const Position &pos, MoveList &moves,
                                    const chess::config::DebugConfig &debug);

    // Generate all legal moves given a position
    static void generateLegal(const Position &pos, MoveList &moves);

    static void generatePawnMoves(const Position &pos, MoveList &moves,
                                  const chess::config::DebugConfig &debug);

    // Initialize attack tables
    static void initAttackTables(const Position &pos,
                                 const chess::config::DebugConfig &debug);

  private:
    // --- Helpers for move generation
    static void generateKnightMoves(const Position &pos, MoveList &moves);
    static void generateBishopMoves(const Position &pos, MoveList &moves);
    static void generateRookMoves(const Position &pos, MoveList &moves);
    static void generateQueenMoves(const Position &pos, MoveList &moves);
    static void generateKingMoves(const Position &pos, MoveList &moves);

    // --- Sliding attacks
    static std::uint64_t bishopAttacks(std::uint64_t bishops,
                                       std::uint64_t occupied);
    static std::uint64_t rookAttacks(std::uint64_t rooks,
                                     std::uint64_t occupied);

    static bool isInCheck(const Position &pos);

    // --- Helpers to parse and add moves
    static void parse_move(std::uint64_t bit_board, MoveList &moves, int offset,
                           MoveFlag flag);

    static void parse_pawn_capture(std::uint64_t bit_board, MoveList &moves,
                                   int offset, std::uint64_t en_passant_bb);

    // --- Attacking moves
    static std::uint64_t
        knightAttacks[64]; // Possible knight attacks from each position
    static std::uint64_t
        kingAttacks[64]; // Possible king attacks from each position
};
} // namespace chess::core
