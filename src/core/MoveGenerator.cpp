#include "../../include/chess/core/MoveGenerator.hpp"
#include "chess/config/DebugConfig.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/Piece.hpp"

#include <cstdint>
#include <stdexcept>

namespace chess::core {
namespace bb = chess::core::BitBoard;

std::uint64_t MoveGenerator::knightAttacks[64]{};
std::uint64_t MoveGenerator::kingAttacks[64]{};

namespace {

/// @brief Calculate all possible knight moves given the knights current pos
std::uint64_t calculateKnightAttacks(const std::uint64_t square) {
    std::uint64_t knight_attacks_bb = 0ULL;

    knight_attacks_bb |=
        bb::shift_left(bb::shift_up(square, 2), 1) & (bb::NOT_FILE_H);
    knight_attacks_bb |=
        bb::shift_right(bb::shift_up(square, 2), 1) & (bb::NOT_FILE_A);

    knight_attacks_bb |= bb::shift_left(bb::shift_up(square, 1), 2) &
                         (bb::NOT_FILE_G & bb::NOT_FILE_H);
    knight_attacks_bb |= bb::shift_right(bb::shift_up(square, 1), 2) &
                         (bb::NOT_FILE_A & bb::NOT_FILE_B);

    knight_attacks_bb |=
        bb::shift_right(bb::shift_down(square, 2), 1) & (bb::NOT_FILE_A);
    knight_attacks_bb |=
        bb::shift_left(bb::shift_down(square, 2), 1) & (bb::NOT_FILE_H);

    knight_attacks_bb |= bb::shift_left(bb::shift_down(square, 1), 2) &
                         (bb::NOT_FILE_G & bb::NOT_FILE_H);
    knight_attacks_bb |= bb::shift_right(bb::shift_down(square, 1), 2) &
                         (bb::NOT_FILE_B & bb::NOT_FILE_A);

    return knight_attacks_bb;
}

/// @brief Calcualte all possible moves of a king from starting square
std::uint64_t calculateKingAttacks(const std::uint64_t square) {
    std::uint64_t king_attacks_bb = 0ULL;

    king_attacks_bb |= bb::shift_north(square);
    king_attacks_bb |= bb::shift_south(square);
    king_attacks_bb |= bb::shift_east(square);
    king_attacks_bb |= bb::shift_west(square);
    king_attacks_bb |= bb::shift_north_west(square);
    king_attacks_bb |= bb::shift_north_east(square);
    king_attacks_bb |= bb::shift_south_west(square);
    king_attacks_bb |= bb::shift_south_east(square);

    return king_attacks_bb;
}

/// @brief Construct move and add it to movelist
void addMovesFromSquare(int from, std::uint64_t targets, MoveFlag flag,
                        MoveList &moves) {

    while (targets) {
        const int to = bb::pop_lsb(targets);

        moves.add(Move{static_cast<std::uint8_t>(from),
                       static_cast<std::uint8_t>(to), flag});
    }
}

} // namespace

void MoveGenerator::generatePseudoLegal(
    const Position &pos, MoveList &moves,
    const chess::config::DebugConfig &debug) {
    generateKnightMoves(pos, moves);
    generatePawnMoves(pos, moves, debug);
}

// Generate all legal moves given a position
void generateLegal(const Position &pos, MoveList &moves) {

    throw std::runtime_error("Function not implemented");
}

void MoveGenerator::initAttackTables() {
    for (int square = 0; square < 64; ++square) {
        const std::uint64_t bit_board = 1ULL << square;

        knightAttacks[square] = calculateKnightAttacks(bit_board);
        kingAttacks[square] = calculateKingAttacks(bit_board);
    }
}

/* ============= GENERATE MOVES HELPERS ============= */
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
    std::uint64_t en_passant_bb = pos.getEnPassantSquareBB();
    leftCaptures &= (enemyPiecesBB | en_passant_bb);
    rightCaptures &= (enemyPiecesBB | en_passant_bb);

    int leftOffset = isWhiteMove ? captureOffsetNW : captureOffsetSW;
    int rightOffset = isWhiteMove ? captureOffsetNE : captureOffsetSE;

    parse_pawn_capture(leftCaptures, moves, leftOffset, en_passant_bb);
    parse_pawn_capture(rightCaptures, moves, rightOffset, en_passant_bb);
}

void MoveGenerator::generateKnightMoves(const Position &pos, MoveList &moves) {
    const Color side = pos.getSideToMove();
    const Color enemy =
        pos.getSideToMove() == Color::White ? Color::Black : Color::White;

    std::uint64_t knights = pos.getPieces(side, PieceType::Knight);

    std::uint64_t occupied = pos.getOccupied();
    std::uint64_t friendly_pieces = pos.getOccupied(side);
    std::uint64_t enemy_pieces = pos.getOccupied(enemy);

    while (knights) {
        int from_square = bb::pop_lsb(knights);

        const std::uint64_t attacks =
            knightAttacks[from_square] & ~friendly_pieces;

        const std::uint64_t quiet_moves = attacks & ~occupied;
        const std::uint64_t captures = attacks & enemy_pieces;

        addMovesFromSquare(from_square, quiet_moves, MoveFlag::QUIET_MOVES,
                           moves);
        addMovesFromSquare(from_square, captures, MoveFlag::CAPTURES, moves);
    }
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
