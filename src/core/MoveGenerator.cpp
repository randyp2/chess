#include "../../include/chess/core/MoveGenerator.hpp"
#include "chess/config/DebugConfig.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/Piece.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace chess::core {

// Short namespace for the bitboard utility file
namespace bb = chess::core::BitBoard;

// Generate all pseudo-legal moves given a position
void MoveGenerator::generatePseudoLegal(
    const Position &pos, MoveList &moves,
    const chess::config::DebugConfig &debug) {
    initAttackTables(pos, debug);
    generatePawnMoves(pos, moves, debug);
    generateKnightMoves(pos, moves);
}

// Generate all legal moves given a position
void generateLegal(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void MoveGenerator::initAttackTables(const Position &pos,
                                     const chess::config::DebugConfig &debug) {
    // for (int square = 0; square < 64; ++square) {
    const std::uint64_t bit_board = 1ULL << 7;

    std::cout << "Current square: \n";
    pos.print_bitboard(bit_board);
    std::cout << "\n";

    // --- Generate knight attacks
    std::uint64_t knight_attacks_bb = 0ULL;

    knight_attacks_bb |= bit_board;
    knight_attacks_bb |=
        bb::shift_left(bb::shift_up(bit_board, 2), 1) & (bb::NOT_FILE_H);
    knight_attacks_bb |=
        bb::shift_left(bb::shift_up(bit_board, 1), 2) & (bb::NOT_FILE_G);
    knight_attacks_bb |=
        bb::shift_right(bb::shift_up(bit_board, 2), 1) & (bb::NOT_FILE_A);
    knight_attacks_bb |=
        bb::shift_right(bb::shift_up(bit_board, 1), 2) & (bb::NOT_FILE_B);

    knight_attacks_bb |= bb::shift_left(bb::shift_down(bit_board, 1), 2);
    knight_attacks_bb |= bb::shift_right(bb::shift_down(bit_board, 1), 2);
    knight_attacks_bb |= bb::shift_left(bb::shift_down(bit_board, 2), 1);
    knight_attacks_bb |= bb::shift_right(bb::shift_down(bit_board, 2), 1);

    // knight_attacks_bb |= bb::shift_left(bit_board, 17) & bb::NOT_FILE_A;
    // knight_attacks_bb |= bb::shift_left(bit_board, 15) & bb::NOT_FILE_A;
    // knight_attacks_bb |= bb::shift_left(bit_board, 10) & bb::NOT_FILE_A;
    // knight_attacks_bb |=
    //     bb::shift_left(bit_board, 6) & (bb::FILE_G | bb::FILE_H);
    // knight_attacks_bb |= bb::shift_right(bit_board, 17) & bb::NOT_FILE_H;
    // knight_attacks_bb |= bb::shift_right(bit_board, 15) & bb::NOT_FILE_H;
    // knight_attacks_bb |= bb::shift_right(bit_board, 10) & bb::NOT_FILE_H;
    // knight_attacks_bb |= bb::shift_right(bit_board, 6) & bb::NOT_FILE_H;

    std::cout << "Attack square: \n";
    pos.print_bitboard(knight_attacks_bb);
    std::cout << "\n";
    // knight_attacks_bb |= bit_board <<
    // }
}

/* ============= HELPERS TO GENERATE MOVES ============= */
void MoveGenerator::generatePawnMoves(const Position &pos, MoveList &moves,
                                      const chess::config::DebugConfig &debug) {

    const bool isWhiteMove = pos.getSideToMove() == Color::White;

    // Offsets for possible moves
    //  - Direction is upwards (+1) -> White perspective
    //  - Direction is downards (-1) -> Black perspective
    const int pushDir = isWhiteMove ? 1 : -1;

    // Types of moves
    const int pushOffset = pushDir * 8;
    const int captureOffsetNW = 9;
    const int captureOffsetNE = 7;
    const int captureOffsetSW = -7;
    const int captureOffsetSE = -9;

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
        isWhiteMove ? bb::shift_north(pawnsBB) : bb::shift_south(pawnsBB);
    singlePawnPush &= emptySquaresBB; // Only push onto empty squares

    std::uint64_t quietPush = singlePawnPush & ~promotionRankMask;
    std::uint64_t promotionPush = singlePawnPush & promotionRankMask;

    parse_move(quietPush, moves, pushOffset, MoveFlag::QUIET_MOVES);
    parse_move(promotionPush, moves, pushOffset, MoveFlag::QUEEN_PROM);

    // --- Generate double pawn pushes ---
    std::uint64_t nonMovedPawns = pawnsBB & startRankMask;
    std::uint64_t singlePushFromStart = singlePawnPush;
    if (isWhiteMove)
        singlePushFromStart &= (bb::shift_north(nonMovedPawns));
    else
        singlePushFromStart &= (bb::shift_south(nonMovedPawns));

    std::uint64_t doublePawnPush = isWhiteMove
                                       ? (bb::shift_north(singlePushFromStart))
                                       : (bb::shift_south(singlePushFromStart));
    doublePawnPush &= emptySquaresBB;

    parse_move(doublePawnPush, moves, pushOffset * 2, MoveFlag::DOUBLE_PP);

    // --- Generate diagonal captures ---
    // Create bitboard for left and right captures
    std::uint64_t leftCaptures = isWhiteMove ? (bb::shift_north_west(pawnsBB))
                                             : (bb::shift_south_west(pawnsBB));
    std::uint64_t rightCaptures = isWhiteMove ? (bb::shift_north_east(pawnsBB))
                                              : (bb::shift_south_east(pawnsBB));

    if (debug.print_bitboards()) {
        std::cout << "Panws BB: \n";
        pos.print_bitboard(pawnsBB);
        // Only valid squares are ones with enemy pieces
        std::cout << "Left captures: \n";
        pos.print_bitboard(leftCaptures);
        std::cout << "\n";

        std::cout << "Right captures: \n";
        pos.print_bitboard(rightCaptures);
        std::cout << "\n";

        std::cout << "Enemy pieces: \n";
        pos.print_bitboard(enemyPiecesBB);
        std::cout << "\n";

        std::cout << "En passant bb: \n";
        pos.print_bitboard(pos.getEnPassantSquareBB());
    }

    std::uint64_t en_passant_bb = pos.getEnPassantSquareBB();
    leftCaptures &= (enemyPiecesBB | en_passant_bb);
    rightCaptures &= (enemyPiecesBB | en_passant_bb);

    int leftOffset = isWhiteMove ? captureOffsetNW : captureOffsetSW;
    int rightOffset = isWhiteMove ? captureOffsetNE : captureOffsetSE;

    parse_pawn_capture(leftCaptures, moves, leftOffset, en_passant_bb);
    parse_pawn_capture(rightCaptures, moves, rightOffset, en_passant_bb);
}

void MoveGenerator::generateKnightMoves(const Position &pos, MoveList &moves) {
    const bool isWhiteMove = pos.getSideToMove() == Color::White;

    const std::uint64_t occupied = pos.getOccupied();

    // Validates free square to move on
    const std::uint64_t emptySquaresBB = ~occupied;

    const std::uint64_t knightsBB =
        isWhiteMove ? pos.getPieces(Color::White, PieceType::Knight)
                    : pos.getPieces(Color::Black, PieceType::Knight);
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

// --- Sliding attacks
std::uint64_t bishopAttacks(std::uint64_t bishops, std::uint64_t occupied) {

    throw std::runtime_error("Function not implemented");
}

std::uint64_t rookAttacks(std::uint64_t rooks, std::uint64_t occupied) {

    throw std::runtime_error("Function not implemented");
}

void MoveGenerator::parse_move(std::uint64_t bit_board, MoveList &moves,
                               int offset, MoveFlag flag) {
    // Add quiet moves to MoveList
    while (bit_board) {
        int targetSquare = bb::pop_lsb(bit_board);
        int startSquare = targetSquare - offset;

        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare), flag));
    }
}

void MoveGenerator::parse_pawn_capture(std::uint64_t bit_board, MoveList &moves,
                                       int offset,
                                       std::uint64_t en_passant_bb) {
    while (bit_board) {
        int targetSquare = bb::pop_lsb(bit_board);
        int startSquare = targetSquare - offset;

        MoveFlag flag =
            en_passant_bb && (targetSquare == bb::lsb(en_passant_bb))
                ? MoveFlag::EP_CAPTURE
                : MoveFlag::CAPTURES;
        moves.add(Move(static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare), flag));
    }
}

} // namespace chess::core
