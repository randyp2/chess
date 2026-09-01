#include "chess/core/movegen/KingSafety.hpp"
#include "chess/core/BitBoard.hpp"
#include "chess/core/Piece.hpp"
#include "chess/core/Position.hpp"

namespace chess::core {

KingSafety getKingSafteyInfo(const Position &pos) {

    const Color curr_side = pos.getSideToMove();
    const Color enemy_side =
        curr_side == Color::White ? Color::Black : Color::White;

    const Bitboard king_bb = pos.getPieces(curr_side, PieceType::King);
    int king_square = bb::lsb(king_bb);

    // --- Retrieve enemy pieces
    const Bitboard enemy_pawns = pos.getPieces(enemy_side, PieceType::Pawn);
    const Bitboard enemy_knights = pos.getPieces(enemy_side, PieceType::Knight);
    const Bitboard enemy_kings = pos.getPieces(enemy_side, PieceType::King);
    const Bitboard enemy_queens = pos.getPieces(enemy_side, PieceType::Queen);
    const Bitboard enemy_bishops = pos.getPieces(enemy_side, PieceType::Bishop);
    const Bitboard enemy_rooks = pos.getPieces(enemy_side, PieceType::Rook);

    // --- Group into attack type
    const Bitboard diagonal_attackers = enemy_queens | enemy_bishops;
    const Bitboard orthogonal_attackers = enemy_queens | enemy_rooks;
}

} // namespace chess::core
