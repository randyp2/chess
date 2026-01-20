#include "../../include/chess/core/MoveGenerator.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/Piece.hpp"

#include <cstdint>
#include <stdexcept>

namespace chess::core {

// Short namespace for the bitboard utility file
namespace bb = chess::core::BitBoard;

// Generate all pseudo-legal moves given a position
void MoveGenerator::generatePseudoLegal(const Position &pos, MoveList &moves) {}

// Generate all legal moves given a position
void generateLegal(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

/* ============= HELPERS TO GENERATE MOVES ============= */

void MoveGenerator::generatePawnMoves(const Position &pos, MoveList &moves) {

    const bool isWhiteMove = pos.getSideToMove() == Color::White;

    // Offsets for possible moves
    //  - Direction is upwards (+1) -> White perspective
    //  - Direction is downards (-1) -> Black perspective
    const int pushDir = isWhiteMove ? 1 : -1;

    // Types of moves
    const int pushOffset = pushDir * 8;
    const int captureOffsetLeft = pushDir * 7;  // Left-diagonal catpure
    const int captureOffsetRight = pushDir * 9; // Right-diagonal catpure

    // Validates enemy square to capture
    const std::uint64_t occupied = pos.getOccupied();

    // Validates free square to move on
    const std::uint64_t emptySquaresBB = ~occupied;

    const std::uint64_t pawnsBB =
        isWhiteMove ? pos.getPieces(Color::White, PieceType::Pawn)
                    : pos.getPieces(Color::Black, PieceType::Pawn);

    const std::uint64_t enemyPiecesBB = isWhiteMove
                                            ? pos.getOccupied(Color::Black)
                                            : pos.getOccupied(Color::White);

    // Start rank and promotion rank
    const std::uint64_t startRankMask = isWhiteMove ? bb::RANK_2 : bb::RANK_7;
    const std::uint64_t promotionRankMask =
        isWhiteMove ? bb::RANK_8 : bb::RANK_1;

    // --- Generate single pawn pushes ---
    std::uint64_t singlePawnPush =
        isWhiteMove ? bb::shift_up(pawnsBB) : bb::shift_down(pawnsBB);
    singlePawnPush &= emptySquaresBB; // Only push onto empty squares

    std::uint64_t quietPush = singlePawnPush & ~promotionRankMask;
    std::uint64_t promotionPush = singlePawnPush & promotionRankMask;

    // Add quiet moves to MoveList
    while (quietPush) {

        int targetSquare = bb::pop_lsb(quietPush);
        int startSquare = targetSquare - pushOffset;

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare),
                       MoveFlag::QUIET_MOVES));
    }

    // Add promotion moves to moves
    while (promotionPush) {

        int targetSquare = bb::pop_lsb(promotionPush);
        int startSquare = targetSquare - pushOffset;

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare),
                       MoveFlag::QUEEN_PROM));
    }

    // --- Generate double pawn pushes ---
    std::uint64_t nonMovedPawns = pawnsBB & startRankMask;
    std::uint64_t singlePushFromStart = singlePawnPush;
    if (isWhiteMove)
        singlePushFromStart &= (nonMovedPawns << 8);
    else
        singlePushFromStart &= (nonMovedPawns >> 8);

    std::uint64_t doublePawnPush =
        isWhiteMove ? (singlePushFromStart << 8) : (singlePushFromStart >> 8);
    doublePawnPush &= emptySquaresBB;

    // Add double pawn push to moves
    while (doublePawnPush) {
        int targetSquare = bb::pop_lsb(doublePawnPush);
        int startSquare = targetSquare - (pushOffset * 2);

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare),
                       MoveFlag::DOUBLE_PP));
    }

    // --- Generate diagonal captures ---
    // Create bitboard for left and right captures
    std::uint64_t leftCaptures = isWhiteMove ? (pawnsBB & bb::NOT_FILE_A) << 7
                                             : (pawnsBB & bb::NOT_FILE_A) >> 7;
    std::uint64_t rightCaptures = isWhiteMove ? (pawnsBB & bb::NOT_FILE_H) << 9
                                              : (pawnsBB & bb::NOT_FILE_H) >> 9;

    // Only valid squares are ones with enemy pieces
    leftCaptures &= enemyPiecesBB;
    rightCaptures &= enemyPiecesBB;

    while (leftCaptures) {
        int targetSquare = bb::pop_lsb(leftCaptures);
        int startSquare = targetSquare - captureOffsetLeft;

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare),
                       MoveFlag::CAPTURES));
    }

    while (rightCaptures) {
        int targetSquare = bb::pop_lsb(rightCaptures);
        int startSquare = targetSquare - captureOffsetRight;

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare),
                       MoveFlag::CAPTURES));
    }
}

void generateKnightMoves(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void generateBishopMoves(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void generateRookMoves(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void generateQueenMoves(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void generateKingMoves(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

// --- Attacking moves
std::uint64_t knightAttacks(std::uint64_t knights) {

    throw std::runtime_error("Function not implemented");
}

std::uint64_t kingAttacks(std::uint64_t kings) {

    throw std::runtime_error("Function not implemented");
}

// --- Sliding attacks
std::uint64_t bishopAttacks(std::uint64_t bishops, std::uint64_t occupied) {

    throw std::runtime_error("Function not implemented");
}

std::uint64_t rookAttacks(std::uint64_t rooks, std::uint64_t occupied) {

    throw std::runtime_error("Function not implemented");
}

} // namespace chess::core
