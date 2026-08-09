#pragma once
#include <cstdint>

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
    King = 0,
    Queen = 1,
    Bishop = 2,
    Knight = 3,
    Rook = 4,
    Pawn = 5,
    Count // Keep track of count in enum
};

struct PieceOnSquare {
    Color color;
    PieceType piece;
    int squareIdx; // 0 - 63
};
} // namespace chess::core
