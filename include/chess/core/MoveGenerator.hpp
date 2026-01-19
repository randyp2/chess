#pragma once

#include "../core/Move.hpp"
#include "../core/Position.hpp"
#include <cstdint>

namespace chess::core {

// Helpful rank masks
inline constexpr std::uint64_t RANK_1 = 0x00000000000000FFULL;
inline constexpr std::uint64_t RANK_2 = 0x000000000000FF00ULL;
inline constexpr std::uint64_t RANK_7 = 0x00FF000000000000ULL;
inline constexpr std::uint64_t RANK_8 = 0xFF00000000000000ULL;

// Helpful file masks
inline constexpr std::uint64_t FILE_A = 0x8080808080808080ULL;
inline constexpr std::uint64_t FILE_H = 0x0101010101010101ULL;

inline constexpr std::uint64_t NOT_FILE_A = ~FILE_A;
inline constexpr std::uint64_t NOT_FILE_H = ~FILE_H;

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
    static void generatePseudoLegal(const Position &pos, MoveList &moves);

    // Generate all legal moves given a position
    static void generateLegal(const Position &pos, MoveList &moves);

    static void generatePawnMoves(const Position &pos, MoveList &moves);

  private:
    // --- Helpers for move generation
    static void generateKnightMoves(const Position &pos, MoveList &moves);
    static void generateBishopMoves(const Position &pos, MoveList &moves);
    static void generateRookMoves(const Position &pos, MoveList &moves);
    static void generateQueenMoves(const Position &pos, MoveList &moves);
    static void generateKingMoves(const Position &pos, MoveList &moves);

    // --- Attacking moves
    static std::uint64_t knightAttacks(std::uint64_t knights);
    static std::uint64_t kingAttacks(std::uint64_t kings);

    // --- Sliding attacks
    static std::uint64_t bishopAttacks(std::uint64_t bishops,
                                       std::uint64_t occupied);
    static std::uint64_t rookAttacks(std::uint64_t rooks,
                                     std::uint64_t occupied);

    static bool isInCheck(const Position &pos);
};
} // namespace chess::core
