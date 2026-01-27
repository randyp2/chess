#pragma once

#include <cstdint> // For fix integer bits

/**
 * std::array has no overhead
 *  - has .size() field -> returns constant O(1)
 *  - doesnt decay to pointer
 *  - has copy and assignment operator
 */
#include <array>
#include <string>
#include <vector>

#include "../config/DebugConfig.hpp"
#include "../core/Move.hpp"
#include "../core/Piece.hpp"

/**
 * Holds a snapshot of current Position
 * Used to keep track of positions and board logic
 */
namespace chess::core {

class Position {
  public:
    /**
     * Default constructor
     * - Initialize to starting chess position
     */
    Position();

    /**
     * Paramterized constructor
     * - Initialize to fen position passed to constructor
     */
    Position(const std::string &);

    /* =============== BASIC HELPERS =============== */
    Color getSideToMove() const { return this->side_to_move; }

    /* =============== BITBOARD GETTERS =============== */

    // Return bitboard with specific color and piece
    std::uint64_t getPieces(Color color, PieceType piece) const;
    std::uint64_t getPieces(PieceType color) const;

    // Return occupied bitboard with specific color
    std::uint64_t getOccupied(Color color) const;

    // Return occupied bitboard squares
    std::uint64_t getOccupied() const;

    /* =============== UI GETTERS =============== */
    /**
     * Return info about all possible pieces - 32 pieces
     *
     * @returns - vector of PieceOnSquare struct for each piece
     */
    std::vector<PieceOnSquare> getAllPieces() const;

    // --- Debugging
    void print_bitboard(std::uint64_t bb) const;

    /* =============== LOGICAL GAME MOVES =============== */
    // Move one piece square to square
    void makeMove(const Move &move, const chess::config::DebugConfig &debugger);
    bool findPieceAt(int squareIdx, Color &outColor, PieceType &outPiece) const;

  private:
    // Bitboard for every piece -> 12 total
    // 2 x 6
    // White: King, Queen, Bishop, Knight, Rook, Pawn
    // Black: King, Queen, Bishop, Knight, Rook, Pawn
    std::array<
        std::array<std::uint64_t, static_cast<std::size_t>(PieceType::Count)>,
        static_cast<std::size_t>(Color::Count)>
        bit_boards{};

    Color side_to_move = Color::White;

    // There can only be one en passant square in the entire game of chess
    // If a pawn moves up twice then the current square it sits on is the ep
    // square
    //  This square is removed every other turn
    int en_passant_square = -1;

    // --- Helpers
    void clear();
    void parse_fen(const std::string &fen);
};

} // namespace chess::core
