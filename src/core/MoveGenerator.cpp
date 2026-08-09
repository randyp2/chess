#include "../../include/chess/core/MoveGenerator.hpp"
#include "chess/config/DebugConfig.hpp"
#include "chess/core/Attacks.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/Piece.hpp"

#include <cstdint>
#include <stdexcept>

namespace chess::core {

namespace {

/// @brief Construct move and add it to movelist
void addMovesFromSquare(int from, Bitboard targets, MoveFlag flag,
                        MoveList &moves) {

    while (targets) {
        const int to = bb::pop_lsb(targets);

        moves.add(Move{static_cast<std::uint8_t>(from),
                       static_cast<std::uint8_t>(to), flag});
    }
}

/// @brief Add noncapture pawns moves to a move list
void addPawnMovesFromTargets(Bitboard targets, int offset, MoveFlag flag,
                             MoveList &moves) {
    while (targets) {
        const int targetSquare = bb::pop_lsb(targets);
        const int startSquare = targetSquare - offset;

        moves.add(Move{static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare), flag});
    }
}

/// @brief Add capturing pawn moves to a move list
void addPawnCapturesFromTargets(Bitboard targets, int offset,
                                Bitboard enPassantTarget, MoveList &moves) {
    while (targets) {
        const int targetSquare = bb::pop_lsb(targets);
        const int startSquare = targetSquare - offset;

        const MoveFlag flag =
            enPassantTarget && targetSquare == bb::lsb(enPassantTarget)
                ? MoveFlag::EP_CAPTURE
                : MoveFlag::CAPTURES;

        moves.add(Move{static_cast<std::uint8_t>(startSquare),
                       static_cast<std::uint8_t>(targetSquare), flag});
    }
}

} // namespace

void MoveGenerator::generatePseudoLegal(
    const Position &pos, MoveList &moves,
    const chess::config::DebugConfig &debug) {
    generateKnightMoves(pos, moves);
    generatePawnMoves(pos, moves, debug);
    generateKingMoves(pos, moves);
    generateRookMoves(pos, moves);
    generateBishopMoves(pos, moves);
    generateQueenMoves(pos, moves);
}

// Generate all legal moves given a position
void MoveGenerator::generateLegal(const Position &, MoveList &) {
    throw std::runtime_error("Function not implemented");
}

/* ============= GENERATE MOVES HELPERS ============= */
void MoveGenerator::generatePawnMoves(const Position &pos, MoveList &moves,
                                      const chess::config::DebugConfig &) {

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

    const Bitboard occupied = pos.getOccupied();

    // Validates free square to move on
    const Bitboard emptySquaresBB = ~occupied;

    const Bitboard pawnsBB = isWhiteMove
                                 ? pos.getPieces(Color::White, PieceType::Pawn)
                                 : pos.getPieces(Color::Black, PieceType::Pawn);

    const Bitboard enemyPiecesBB = isWhiteMove ? pos.getOccupied(Color::Black)
                                               : pos.getOccupied(Color::White);

    // Start rank and promotion rank
    const Bitboard startRankMask = isWhiteMove ? bb::RANK_2 : bb::RANK_7;
    const Bitboard promotionRankMask = isWhiteMove ? bb::RANK_8 : bb::RANK_1;

    // --- Generate single pawn pushes ---
    Bitboard singlePawnPush =
        isWhiteMove ? bb::shift_north(pawnsBB) : bb::shift_south(pawnsBB);
    singlePawnPush &= emptySquaresBB; // Only push onto empty squares

    Bitboard quietPush = singlePawnPush & ~promotionRankMask;
    Bitboard promotionPush = singlePawnPush & promotionRankMask;

    addPawnMovesFromTargets(quietPush, pushOffset, MoveFlag::QUIET_MOVES,
                            moves);
    addPawnMovesFromTargets(promotionPush, pushOffset, MoveFlag::QUEEN_PROM,
                            moves);

    // --- Generate double pawn pushes ---
    Bitboard nonMovedPawns = pawnsBB & startRankMask;
    Bitboard singlePushFromStart = singlePawnPush;
    if (isWhiteMove)
        singlePushFromStart &= (bb::shift_north(nonMovedPawns));
    else
        singlePushFromStart &= (bb::shift_south(nonMovedPawns));

    Bitboard doublePawnPush = isWhiteMove
                                  ? (bb::shift_north(singlePushFromStart))
                                  : (bb::shift_south(singlePushFromStart));
    doublePawnPush &= emptySquaresBB;

    addPawnMovesFromTargets(doublePawnPush, pushOffset * 2, MoveFlag::DOUBLE_PP,
                            moves);

    // --- Generate diagonal captures ---
    // Create bitboard for left and right captures
    Bitboard leftCaptures = isWhiteMove ? (bb::shift_north_west(pawnsBB))
                                        : (bb::shift_south_west(pawnsBB));
    Bitboard rightCaptures = isWhiteMove ? (bb::shift_north_east(pawnsBB))
                                         : (bb::shift_south_east(pawnsBB));
    Bitboard en_passant_bb = pos.getEnPassantSquareBB();
    leftCaptures &= (enemyPiecesBB | en_passant_bb);
    rightCaptures &= (enemyPiecesBB | en_passant_bb);

    int leftOffset = isWhiteMove ? captureOffsetNW : captureOffsetSW;
    int rightOffset = isWhiteMove ? captureOffsetNE : captureOffsetSE;

    addPawnCapturesFromTargets(leftCaptures, leftOffset, en_passant_bb, moves);
    addPawnCapturesFromTargets(rightCaptures, rightOffset, en_passant_bb,
                               moves);
}

void MoveGenerator::generateKnightMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy =
        pos.getSideToMove() == Color::White ? Color::Black : Color::White;

    Bitboard knights = pos.getPieces(side, PieceType::Knight);

    const Bitboard occupied = pos.getOccupied();
    const Bitboard friendly_pieces = pos.getOccupied(side);
    const Bitboard enemy_pieces = pos.getOccupied(enemy);

    while (knights) {
        const int from_square = bb::pop_lsb(knights);

        const Bitboard attacks =
            attacks::knight(from_square) & ~friendly_pieces;

        const Bitboard quiet_moves = attacks & ~occupied;
        const Bitboard captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
}

void MoveGenerator::generateKingMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy = side == Color::White ? Color::Black : Color::White;

    Bitboard kings = pos.getPieces(side, PieceType::King);

    const Bitboard occupied = pos.getOccupied();
    const Bitboard friendly_pieces = pos.getOccupied(side);
    const Bitboard enemy_pieces = pos.getOccupied(enemy);

    while (kings) {
        const int from_square = bb::pop_lsb(kings);

        const Bitboard attacks = attacks::king(from_square) & ~friendly_pieces;

        const Bitboard quiet_moves = attacks & ~occupied;
        const Bitboard captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
}

void MoveGenerator::generateBishopMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy = side == Color::White ? Color::Black : Color::White;

    Bitboard bishops = pos.getPieces(side, PieceType::Bishop);

    const Bitboard occupied = pos.getOccupied();
    const Bitboard friendly_pieces = pos.getOccupied(side);
    const Bitboard enemy_pieces = pos.getOccupied(enemy);

    while (bishops) {
        const int from_square = bb::pop_lsb(bishops);

        const Bitboard attacks =
            attacks::bishop(from_square, occupied) & ~friendly_pieces;

        const Bitboard quiet_moves = attacks & ~occupied;
        const Bitboard captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
}

void MoveGenerator::generateRookMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy = side == Color::White ? Color::Black : Color::White;

    Bitboard rooks = pos.getPieces(side, PieceType::Rook);

    const Bitboard occupied = pos.getOccupied();
    const Bitboard friendly_pieces = pos.getOccupied(side);
    const Bitboard enemy_pieces = pos.getOccupied(enemy);

    while (rooks) {
        const int from_square = bb::pop_lsb(rooks);

        const Bitboard attacks =
            attacks::rook(from_square, occupied) & ~friendly_pieces;

        const Bitboard quiet_moves = attacks & ~occupied;
        const Bitboard captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
}

void MoveGenerator::generateQueenMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy = side == Color::White ? Color::Black : Color::White;

    Bitboard queens = pos.getPieces(side, PieceType::Queen);

    const Bitboard occupied = pos.getOccupied();
    const Bitboard friendly_pieces = pos.getOccupied(side);
    const Bitboard enemy_pieces = pos.getOccupied(enemy);

    while (queens) {
        const int from_square = bb::pop_lsb(queens);

        const Bitboard attacks =
            attacks::queen(from_square, occupied) & ~friendly_pieces;

        const Bitboard quiet_moves = attacks & ~occupied;
        const Bitboard captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
}

} // namespace chess::core
