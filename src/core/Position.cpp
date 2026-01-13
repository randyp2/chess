#include "../../include/chess/core/Position.hpp"
#include <cstdint>
#include <cwctype>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace chess::core {

/* ======================= ANONYMOUS NAMESPACE ======================= */
// Anonymous namespace: Used for private helper functions local to this file
namespace {

constexpr std::size_t idx(Color c) { return static_cast<std::size_t>(c); }

constexpr std::size_t idx(PieceType p) { return static_cast<std::size_t>(p); }

// Mapping: refer to dcouments/board-setup.md
// Convert rank and file to square integers
// rank & file -> 0-7
constexpr int square_index(int rank, int file) { return rank * 8 + (7 - file); }

} // namespace
/* ======================= ANONYMOUS NAMESPACE ======================= */

/* ========= CONSTRUCTORS =========*/
Position::Position() {
    parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    // parse_fen("pppppppp/pppppppp/8/8/8/8/8/8 w");
}

Position::Position(const std::string &fen_string) { parse_fen(fen_string); }

/* ========= GETTERS =========*/
std::uint64_t Position::getPieces(Color color, PieceType piece) const {
    return bit_boards[idx(color)][idx(piece)];
}

std::uint64_t Position::getPieces(PieceType piece) const {
    return bit_boards[idx(Color::White)][idx(piece)] |
           bit_boards[idx(Color::Black)][idx(piece)];
}

std::uint64_t Position::getOccupied(Color color) const { return 0ULL; }

std::uint64_t Position::getOccupied() const { return 0ULL; }

std::vector<PieceOnSquare> Position::getAllPieces() const {
    std::vector<PieceOnSquare> returner;

    for (int color = 0; color < 2; ++color) {
        for (int piece = 0; piece < 6; ++piece) {
            std::uint64_t bit_board = bit_boards[color][piece];

            while (bit_board) {
                // Count the number of trailing zeroes to the right of the first
                // one
                int square = __builtin_ctzll(bit_board);

                /*
                 * Bit _board - 1
                 *      - flips lowest right 1 bit
                 *      - sets the bits to the right of it to 1
                 * &= removes the lowest set bit
                 *
                 * i.e.
                 * bb = 00101000
                 * bb - 1 = 001001111
                 * bb & bb - 1 = 00100000
                 */
                bit_board &= bit_board - 1;

                returner.push_back({static_cast<Color>(color),
                                    static_cast<PieceType>(piece), square});
            }
        }
    }

    return returner;
}

/* ========= HELPERS FOR FEN ========= */
void Position::clear() {
    for (auto &color_bit_board : bit_boards) {
        for (auto &bit_board : color_bit_board) {
            bit_board = 0ULL;
        }
    }
}

/**
 * Parse the fen string and populate the bit boards
 *
 * @params  fen - string representing a position
 */
void Position::parse_fen(const std::string &fen) {
    // Clear previous bit boards
    clear();

    // Read only string stream
    std::istringstream iss(fen);
    std::string positions;
    std::string current_move;

    iss >> positions >> current_move;

    // --- Initialize side to move
    side_to_move = current_move == "w" ? Color::White : Color::Black;

    // --- Parse fen string
    std::istringstream iss2(positions);

    int rank = 7; // Start at top most rank (index 7)
    int file = 0;
    std::string token;
    while (std::getline(iss2, token, '/')) {

        file = 0;

        // --- Parse characters
        for (const char c : token) {

            // Check for empty space -> check if digit
            if (std::isdigit(c)) {
                // Skip that many files (columns)
                file += c - '0';
                continue;
            }

            // Get color type
            // !!! Cast to unsigned char since signedness of char is impl
            // defined !!!
            Color color = std::isupper(static_cast<unsigned char>(c))
                              ? Color::White
                              : Color::Black;

            // Get piece type
            char normalized_fen_char = std::tolower(c);
            PieceType piece;

            switch (normalized_fen_char) {
            case 'p':
                piece = PieceType::Pawn;
                break;
            case 'n':
                piece = PieceType::Knight;
                break;
            case 'b':
                piece = PieceType::Bishop;
                break;
            case 'r':
                piece = PieceType::Rook;
                break;
            case 'q':
                piece = PieceType::Queen;
                break;
            case 'k':
                piece = PieceType::King;
                break;
            default:
                continue;
            }

            int squareIdx = square_index(rank, file);
            std::uint64_t temp = 1ULL; // 64 bit integer with LSB set to 1
            temp = temp << squareIdx;  // Shift over the 1 bit to its designated
                                       // bit representing the square index
            bit_boards[idx(color)][idx(piece)] |=
                temp; // Bitwise or to preserve past bits
            file++;
        }

        rank--;
    }
}

bool Position::findPieceAt(int squareIdx, Color &outColor,
                           PieceType &outPiece) const {
    std::uint64_t mask = 1ULL << squareIdx;

    for (int color = 0; color < 2; ++color) {
        for (int piece = 0; piece < 6; ++piece) {
            if (bit_boards[color][piece] & mask) {
                outColor = static_cast<Color>(color);
                outPiece = static_cast<PieceType>(piece);
                return true;
            }
        }
    }

    return false;
}

void Position::makeMove(int current_square, int final_square) {

    // Same square move
    if (current_square == final_square)
        return;

    Color currColor;
    PieceType currPiece;

    if (!findPieceAt(current_square, currColor, currPiece))
        return;

    // Represent from and to destinations with a bit board
    std::uint64_t fromBB = 1ULL << current_square;
    std::uint64_t toBB = 1ULL << final_square;

    // Remove moving piece from own square
    //  - Take ones complement and & it to the bit_boards
    //  - This will remove the piece from its current position
    //  i.e. bitboard = 00101000 , fromBB = 00101000, ~fromBB = 11010111
    //  bitboard & ~fromBB = 0010000
    bit_boards[idx(currColor)][idx(currPiece)] &= ~fromBB;

    // Clear final square
    for (int piece = 0; piece < 6; ++piece) {
        bit_boards[idx(Color::White)][piece] &= ~toBB;
        bit_boards[idx(Color::Black)][piece] &= ~toBB;
    }

    // Placing moving piece to final square
    bit_boards[idx(currColor)][idx(currPiece)] |= toBB;

    side_to_move = (side_to_move == Color::White) ? Color::Black : Color::White;
}

void Position::print_bitboard(std::uint64_t bb) {

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << std::setw(2) << std::to_string(rank + 1) << "  ";

        for (int file = 0; file < 8; ++file) {
            int squareIdx = square_index(rank, file);

            std::uint64_t temp = 1ULL << squareIdx; // Shift 1 bit to the bit
                                                    // reffering to square index

            // Perform bit-wise and and if non-zero then it is occupied
            bool occupied = (bb & temp) != 0;

            std::cout << std::setw(2) << (occupied ? '1' : '.');
        }

        std::cout << '\n';
    }

    // File labels
    std::cout << "\n   ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << std::setw(2) << file;
    }

    std::cout << "\n\n";
}

}; // namespace chess::core
