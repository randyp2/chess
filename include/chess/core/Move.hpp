#pragma once
#include <cstdint>

#include "../core/Piece.hpp"

/**
 * We can use 16 bit word to encapture what a type of move is
 * Storing these into flags and encoding them.
 *
 * Minimum info needed to represent a move
 *  - From Square
 *  - To Square
 *  - Move kind
 *  - Promoted piece code: In case of promotions
 *
 *
 * Why 16 bits?
 *  64 Squares -> 6 bits needed to represent numbers 0-64
 *  - First 6 bits (From Square)
 *  - Next 6 bits (To Square)
 *  - 4 bits for encoding special types of moves = 0 - 15
 *
 * Reference: https://www.chessprogramming.org/Encoding_Moves
 */
namespace chess::core {

// Flags to keep track of normal moves and special moves in chess
enum class MoveFlag : std::uint8_t {

    QUIET_MOVES = 0, // Moves that dont alter material
    DOUBLE_PP = 1,
    KING_CASTLE = 2,
    QUEEN_CASTLE = 3,
    CAPTURES = 4,
    EP_CAPTURE = 5, // Stupid enpassant

    // Types of promotions
    KNIGHT_PROM = 8,
    BISHOP_PROM = 9,
    ROOK_PROM = 10,
    QUEEN_PROM = 11,

    // CAPTURING THEN PROMOTING
    KNIGHT_PROMO_CAPTURE = 12,
    BISHOP_PROMO_CAPTURE = 13,
    ROOK_PROMO_CAPTURE = 14,
    QUEEN_PROMO_CAPTURE = 15,
};

class Move {

  public:
    /**
     * Parameterized constructor
     *
     * Apply masks to from, to, and flags to represent a 16 bit integer
     * MSB -> LSB = flags | to | from
     *
     * @params from - 6 bit integer representing from square idx
     *         to - 6 bit integer representing to square idx
     *         flags - 4 bit integer representing to square idx
     */
  constexpr Move(std::uint8_t from, std::uint8_t to, MoveFlag flags)
      : move_((flags & 0x0f << 12) | (to & 0x3f <<) | (from &))

            private : std::uint16_t move_;
}

} // namespace chess::core
