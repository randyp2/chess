#pragma once
#include <cstdint>
#include <iomanip>
#include <iostream>

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

// Inline to prevent redefinition error acrross multiple translation units/files
inline const char *to_string(MoveFlag flag) {
    switch (flag) {
    case MoveFlag::QUIET_MOVES:
        return "QUIET";
    case MoveFlag::DOUBLE_PP:
        return "DOUBLE_PAWN_PUSH";
    case MoveFlag::KING_CASTLE:
        return "KING_CASTLE";
    case MoveFlag::QUEEN_CASTLE:
        return "QUEEN_CASTLE";
    case MoveFlag::CAPTURES:
        return "CAPTURE";
    case MoveFlag::EP_CAPTURE:
        return "EN_PASSANT";
    case MoveFlag::KNIGHT_PROM:
        return "KNIGHT_PROMOTION";
    case MoveFlag::BISHOP_PROM:
        return "BISHOP_PROMOTION";
    case MoveFlag::ROOK_PROM:
        return "ROOK_PROMOTION";
    case MoveFlag::QUEEN_PROM:
        return "QUEEN_PROMOTION";
    case MoveFlag::KNIGHT_PROMO_CAPTURE:
        return "KNIGHT_PROMO_CAPTURE";
    case MoveFlag::BISHOP_PROMO_CAPTURE:
        return "BISHOP_PROMO_CAPTURE";
    case MoveFlag::ROOK_PROMO_CAPTURE:
        return "ROOK_PROMO_CAPTURE";
    case MoveFlag::QUEEN_PROMO_CAPTURE:
        return "QUEEN_PROMO_CAPTURE";
    default:
        return "UNKNOWN";
    }
}
class Move {

  public:
    constexpr Move() : move_(0) {}
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
    constexpr Move(std::uint8_t from, std::uint8_t to, MoveFlag flags) {
        std::uint16_t flags_encoded = (static_cast<std::uint8_t>(flags) & 0xf)
                                      << 12;
        std::uint16_t to_encoded = (static_cast<std::uint8_t>(to) & 0x3f) << 6;
        std::uint16_t from_encoded = (static_cast<std::uint8_t>(from) & 0x3f);

        move_ = flags_encoded | to_encoded | from_encoded;
    }

    /* =========== GETTERS =========== */
    constexpr MoveFlag getFlags() const {
        return static_cast<MoveFlag>((move_ >> 12) & 0x0f);
    }

    constexpr std::uint16_t getFrom() const { return move_ & 0x3f; }

    constexpr std::uint16_t getTo() const { return (move_ >> 6) & 0x3f; }

    constexpr std::uint16_t getRawValue() const { return move_; }

    // Return from and to bits
    // Will be used to record from -> to moves to help with chess engine
    // Used in searching best from -> to moves given a position
    constexpr std::uint16_t getButterflyIndex() const { return move_ & 0x0fff; }

    /* =========== SETTERS =========== */
    void setFrom(std::uint16_t from) {
        // Zero out the last 6 bits
        //  - Take ones compliment of 0x3f = 0b0011 1111
        move_ &= ~0x3f;
        move_ |= (from & 0x3f);
    }

    void setTo(std::uint16_t to) {
        // Zero out the middle 6 bits
        //  - Take ones compliment of 0xFC0 =  0b0000 1111 1100 0000
        move_ &= ~0xfc0;
        move_ |= ((to & 0x3f) << 6);
    }

    /* =========== HELPERS =========== */
    constexpr bool isDoublePP() const {
        return getFlags() == MoveFlag::DOUBLE_PP;
    }

    constexpr bool isKingCastle() const {
        return getFlags() == MoveFlag::KING_CASTLE;
    }

    constexpr bool isQueenCastle() const {
        return getFlags() == MoveFlag::QUEEN_CASTLE;
    }

    constexpr bool isCapture() const {
        return getFlags() == MoveFlag::CAPTURES;
    }

    constexpr bool isEPCapture() const {
        return getFlags() == MoveFlag::EP_CAPTURE;
    }

    /* =========== OPERATOR OVERLADS =========== */
    bool operator==(Move other) const {
        return (move_ & 0xffff) == (other.move_ & 0xffff);
    }

    bool operator!=(Move other) const {
        return (move_ & 0xffff) != (other.move_ & 0xffff);
    }

    friend std::ostream &operator<<(std::ostream &os, const Move &move) {
        MoveFlag flag = static_cast<MoveFlag>((move.move_ & 0xf000) >> 12);
        std::uint16_t to = move.getFrom();
        std::uint16_t from = move.getTo();

        os << "============================\n";
        os << std::left << std::setw(10) << "MOVE TYPE " << ":"
           << to_string(flag) << "\n";
        os << std::left << std::setw(10) << "FROM: " << ":"
           << std::to_string(from) << "\n";
        os << std::left << std::setw(10) << "TO: " << ":" << std::to_string(to)
           << "\n";
        os << std::left << std::setw(10) << "RAW" << " : 0x" << std::hex
           << std::setw(4) << std::setfill('0') << move.getRawValue()
           << std::dec << std::setfill(' ') << '\n';
        os << "============================\n";
        return os;
    }

  private:
    std::uint16_t move_;
};

} // namespace chess::core
