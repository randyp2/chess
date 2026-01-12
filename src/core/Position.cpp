#include "../../include/chess/core/Position.hpp" #include < cctype>
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
}

Position::Position(const std::string &fen_string) { parse_fen(fen_string); }

/* ========= GETTERS =========*/
std::uint64_t Position::pieces(Color color, PieceType piece) const {
    return bit_boards[idx(color)][idx(piece)];
}

std::uint64_t Position::pieces(PieceType piece) const {
    return bit_boards[idx(Color::White)][idx(piece)] |
           bit_boards[idx(Color::Black)][idx(piece)];
}

std::uint64_t Position::occupied(Color color) const { return 0ULL; }

std::uint64_t Position::occupied() const { return 0ULL; }

/* -------------- Helper Functions -------------- */
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
