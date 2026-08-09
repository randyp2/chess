#pragma once

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
#include "../core/BitBoard.hpp"
#include "../core/Move.hpp"
#include "../core/Piece.hpp"

/**
 * Holds a snapshot of current Position
 * Used to keep track of positions and board logic
 */
namespace chess::core {

class Position {
  public:
    /// @brief Initialize to starting chess position
    Position();

    /// @brief Initialize to fen position passed to constructor
    Position(const std::string &);

    Color getSideToMove() const { return this->side_to_move; }

    Bitboard getPieces(Color color, PieceType piece) const;
    Bitboard getPieces(PieceType color) const;

    Bitboard getOccupied(Color color) const;
    Bitboard getOccupied() const;

    Bitboard getEnPassantSquareBB() const { return this->en_passant_square_bb; }

    /**
     * Return info about all possible pieces - 32 pieces
     *
     * @returns - vector of PieceOnSquare struct for each piece
     */
    std::vector<PieceOnSquare> getAllPieces() const;

    void print_bitboard(Bitboard bb) const;

    // Move one piece square to square
    void makeMove(const Move &move, const chess::config::DebugConfig &debugger);
    bool findPieceAt(int squareIdx, Color &outColor, PieceType &outPiece) const;

  private:
    // Bitboard for every piece -> 12 total
    // 2 x 6
    // White: King, Queen, Bishop, Knight, Rook, Pawn
    // Black: King, Queen, Bishop, Knight, Rook, Pawn
    std::array<std::array<Bitboard, static_cast<std::size_t>(PieceType::Count)>,
               static_cast<std::size_t>(Color::Count)>
        bit_boards{};

    Color side_to_move = Color::White;

    // There can only be one en passant square in the entire game of chess
    // If a pawn moves up twice then the current square it sits on is the ep
    // square
    //  This square is removed every other turn
    Bitboard en_passant_square_bb = 0ULL;

    void clear();
    void parse_fen(const std::string &fen);
};

} // namespace chess::core
