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

/**
 * Holds a snapshot of current Position
 * Used to keep track of positions and board logic
 */
namespace chess::core {

// Use scoped enums to represent color types and piece types
//  - Better readablity: Must use scopes
//  - Type safety

enum class Color : std::uint8_t {
    White = 0,
    Black = 1,
    Count // Keep track of count in enum
};

enum class PieceType : std::uint8_t {
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
    Count // Keep track of count in enum
};

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

    // --- Bitboard getters

    // Return bitboard with specific color and piece
    std::uint64_t pieces(Color color, PieceType piece) const;
    std::uint64_t pieces(PieceType color) const;

    // Return occupied bitboard with specific color
    std::uint64_t occupied(Color color) const;

    // Return occupied bitboard squares
    std::uint64_t occupied() const;

    // --- Debugging
    void print_bitboard(std::uint64_t bb);

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

    // --- Helpers
    void clear();
    void parse_fen(const std::string &fen);
};

} // namespace chess::core
